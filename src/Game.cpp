#include "Game.h"

void Game::startGame(std::string FEN)
{
    Board board = generateBoardFromFEN(FEN);
    if (history_enabled)
        initializeGameHistory(board);
    log(CHESS_BOARD, "Initialized board with FEN: " + FEN);
    printGameState(board);
    MoveList moves = move_generator.generateLegalMoves(board);

    do
    {
        Move move = getLegalMoveFromUser(moves);
        if (move.piece == UNDO)
            board = undoMove();
        else
            applyMove(board, move);
        printGameState(board);
        moves = move_generator.generateLegalMoves(board);
    } while (!isGameOver(board, moves));

    std::cout << "GAME OVER" << std::endl;
}

void Game::applyMove(Board &board, Move move)
{
    move_generator.applyMove(board, move);
    if (history_enabled)
        addBoardToHistory(board);
}

Board Game::undoMove()
{
    assert(history_enabled, "History is not enabled");
    game_history = game_history->prev;
    delete game_history->next;
    game_history->next = nullptr;
    return game_history->board;
}

Move Game::getLegalMoveFromUser(MoveList legal_moves)
{
    std::string input;
    while (true)
    {
        std::cout << "Enter your move: ";
        std::getline(std::cin, input);

        // print legal moves
        if (input[0] == 'p')
        {
            std::cout << "Legal moves:" << std::endl;
            for (int i = 0; i < legal_moves.size; i++)
            {
                Move legal_move = legal_moves.moves[i];
                std::cout << "\t" << getSquareName(legal_move.from) << getSquareName(legal_move.to);
                if (legal_move.promotion)
                    std::cout << " (" << legal_move.promotion << ")";
                std::cout << std::endl;
            }
            continue;
        }

        // undo previous move
        if (input[0] == 'u')
        {
            if (game_history->prev)
                return Move{0, 0, UNDO};
            std::cout << "No previous move to undo." << std::endl;
            continue;
        }

        // skip invalid moves
        if (input[0] < 'a' || input[0] > 'h' || input[1] < '1' || input[1] > '8' || input[2] < 'a' || input[2] > 'h' || input[3] < '1' || input[3] > '8')
        {
            std::cout << "Invalid move. Please enter a move in the format: 'a2c3'" << std::endl;
            continue;
        }

        // return move if legal else skip
        Position from = 8 * (input[1] - '1') + (input[0] - 'a');
        Position to = 8 * (input[3] - '1') + (input[2] - 'a');
        for (int i = 0; i < legal_moves.size; i++)
        {
            Move legal_move = legal_moves.moves[i];
            if (legal_move.from == from && legal_move.to == to)
            {
                if (legal_move.promotion)
                    legal_move.promotion = getPromotionChoice();
                return legal_move;
            }
        }
        std::cout << "Illegal move. Please enter a legal move." << std::endl;
    }
}

Piece Game::getPromotionChoice()
{
    std::cout << "Choose promotion piece: " << std::endl;
    std::cout << "Q - Queen" << std::endl;
    std::cout << "R - Rook" << std::endl;
    std::cout << "B - Bishop" << std::endl;
    std::cout << "N - Knight" << std::endl;

    Piece choice;
    while (true)
    {
        std::cin >> choice;
        if (choice == 'Q' || choice == 'R' || choice == 'B' || choice == 'N' || choice == 'q' || choice == 'r' || choice == 'b' || choice == 'n')
            return choice;
        std::cout << "Invalid choice. Please enter Q, R, B or N." << std::endl;
    }
}

bool Game::isGameOver(Board &board, MoveList moves)
{
    if (!moves.empty())
        return false;
    Bitboard king = board.white_to_move ? board.white_king : board.black_king;
    bool king_in_check = move_generator.squaresThreatened(board, king, true);
    std::cout << (king_in_check ? "Checkmate" : "Stalemate") << std::endl;
    return true;
}

uint64 Game::perft(int depth, std::string FEN, bool divide, uint64 expected)
{
    assert(depth >= 1 && depth <= 10, "Perft depth must be between 1 and 10");
    history_enabled = false;
    Board board = generateBoardFromFEN(FEN);
    log(PERFT, "Starting perft with depth: " + std::to_string(depth) + " and FEN: " + FEN);
    uint64 start_time = getCurrentTimeSeconds();
    uint64 nodes = perft(depth, board, divide);
    uint64 end_time = getCurrentTimeSeconds();
    log(PERFT, "Nodes searched: " + std::to_string(nodes));
    log(PERFT, "Time taken: " + convertSecondsToString(end_time - start_time));
    if (expected)
    {
        uint64 diff = nodes > expected ? nodes - expected : expected - nodes;
        assert(nodes == expected, "Expected: " + std::to_string(expected) + "\nDiff: " + std::to_string(diff));
    }
    return nodes;
}

uint64 Game::perft(int depth, Board board, bool divide)
{
    // if (depth == 0)
    //     return 1;
    MoveList legal_moves = move_generator.generateLegalMoves(board);
    if (depth == 1)
        return legal_moves.size;

    uint64 total_nodes = 0;
    for (int i = 0; i < legal_moves.size; i++)
    {
        Board board_copy = board;
        applyMove(board_copy, legal_moves.moves[i]);
        uint64 nodes = perft(depth - 1, board_copy);
        if (divide)
            log(PERFT, getSquareName(legal_moves.moves[i].from) + getSquareName(legal_moves.moves[i].to) + ": " + std::to_string(nodes));
        total_nodes += nodes;
    }
    return total_nodes;
}

void Game::initializeGameHistory(Board board)
{
    assert(game_history == nullptr, "Game history already initialized.");
    Board copy = board;
    game_history = new Node();
    game_history->board = copy;
    game_history->next = nullptr;
    game_history->prev = nullptr;
}

void Game::addBoardToHistory(Board board)
{
    assert(game_history != nullptr, "Game history not initialized.");
    Node *new_node = new Node();
    new_node->board = board;
    new_node->next = nullptr;
    new_node->prev = game_history;
    game_history->next = new_node;
    game_history = new_node;
}

int main(int argc, char *argv[])
{
    std::string start_fen = START_FEN;
    int perft_depth = 0;
    bool divide = false;
    int expected_perft = 0;
    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "-f" && i + 1 < argc)
            start_fen = argv[++i];
        else if (std::string(argv[i]) == "-p" && i + 1 < argc)
            perft_depth = std::stoi(argv[++i]);
        else if (std::string(argv[i]) == "-e" && i + 1 < argc)
            expected_perft = std::stoull(argv[++i]);
        else if (std::string(argv[i]) == "-d")
            divide = true;
        else
        {
            std::cout << "Usage: " << argv[0] << " [-f FEN] [-p depth] [-d] [-h]\n"
                      << "  -f FEN: Start the game with the given FEN string\n"
                      << "  -p depth: Run perft with the given depth\n"
                      << "  -d: Divide perft results\n"
                      << "  -h: Show this help message" << std::endl;
            if (std::string(argv[i]) != "-h")
                exit(1);
        }
    }

    Game game;
    if (perft_depth > 0)
        game.perft(perft_depth, start_fen, divide, expected_perft);
    else
        game.startGame(start_fen);

    return 0;
}
