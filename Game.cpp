#include "Game.h"

void Game::startGame(std::string FEN)
{
    Board board = generateBoardFromFEN(FEN);
    log(CHESS_BOARD, "Initialized board with FEN: " + FEN);
    printGameState(board);
    MoveList moves = move_generator.generateLegalMoves(board);

    do
    {
        Move move = getLegalMoveFromUser(moves);
        applyMove(board, move);
        printGameState(board);
        moves = move_generator.generateLegalMoves(board);
    } while (!isGameOver(board, moves));

    std::cout << "GAME OVER" << std::endl;
}

void Game::applyMove(Board &board, Move move)
{
    move_generator.applyMove(board, move);
}

Move Game::getLegalMoveFromUser(MoveList legal_moves)
{
    std::string input;
    while (true)
    {
        std::cout << "Enter your move: ";
        std::getline(std::cin, input);
        if (input[0] < 'a' || input[0] > 'h' || input[1] < '1' || input[1] > '8' || input[3] < 'a' || input[3] > 'h' || input[4] < '1' || input[4] > '8')
        {
            std::cout << "Invalid move. Please enter a move in the format: 'a2 c3'" << std::endl;
            continue;
        }

        Position from = 8 * (input[1] - '1') + (input[0] - 'a');
        Position to = 8 * (input[4] - '1') + (input[3] - 'a');
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
    Bitboard king = whites_turn ? board.white_king : board.black_king;
    bool king_in_check = move_generator.squaresThreatened(board, king, true);
    std::cout << (king_in_check ? "Checkmate" : "Stalemate") << std::endl;
    return true;
}

uint64 Game::perft(int depth, std::string FEN)
{
    Board board = generateBoardFromFEN(FEN);
    uint64 nodes = perft(depth, board);
    log(PERFT, "Perft with depth: " + std::to_string(depth));
    uint64 diff = nodes > PERFT_RESULTS[depth] ? nodes - PERFT_RESULTS[depth] : PERFT_RESULTS[depth] - nodes;
    log(PERFT, "Found " + std::to_string(nodes) + " nodes");
    assert(nodes == PERFT_RESULTS[depth], "Expected: " + std::to_string(PERFT_RESULTS[depth]) + "\nDiff: " + std::to_string(diff));
    return nodes;
}

uint64 Game::perft(int depth, Board board)
{
    if (depth == 0)
        return 1ULL;

    MoveList legal_moves = move_generator.generateLegalMoves(board);
    uint64 nodes = 0;

    for (int i = 0; i < legal_moves.size; i++)
    {
        Board board_copy = board;
        applyMove(board_copy, legal_moves.moves[i]);
        Bitboard king = whites_turn ? board_copy.white_king : board_copy.black_king;
        if (!move_generator.squaresThreatened(board_copy, king, false))
            nodes += perft(depth - 1, board_copy);
    }
    return nodes;
}

int main()
{
    Game game;
    game.startGame(START_FEN);
    game.perft(5, START_FEN);
    return 0;
}