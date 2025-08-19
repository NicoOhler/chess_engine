#include "Engine.h"

void Engine::startConsoleGame(std::string fen)
{
    Board board = generateBoardFromFEN(fen);
    log(CHESS_BOARD, "Initialized board with FEN: " + fen);
    printGameState(board);
    MoveList moves = move_generator.generateLegalMoves(board);

    do
    {
        Move move = getLegalMoveFromUser(moves);
        if (move.promotion)
            move.promotion = board.white_to_move ? toupper(move.promotion) : tolower(move.promotion);
        applyAndTrackMove(board, move);
        printGameState(board);
        moves = move_generator.generateLegalMoves(board);
    } while (getGameState(board, moves) == IN_PROGRESS);
    std::cout << (getGameState(board, moves) == CHECKMATE ? "Checkmate" : "Draw") << std::endl;
}

Move Engine::getLegalMoveFromUser(MoveList legal_moves)
{
    std::string input;
    while (true)
    {
        std::cout << "Enter your move: ";
        std::getline(std::cin, input);

        // print legal moves
        if (input[0] == 'p')
        {
            std::cout << "Legal moves: " << legal_moves.size << std::endl;
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
            if (!move_history.empty())
                return UNDO_MOVE;
            std::cout << "No move to undo" << std::endl;
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

Piece Engine::getPromotionChoice()
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

void Engine::applyAndTrackMove(Board &board, Move move)
{
    if (move.piece == UNDO)
    {
        assert(!move_history.empty(), "No move to undo");
        move = move_history.top();
        move_history.pop();
        move_generator.unmakeMove(board, move);
        return;
    }
    move_history.push(move);
    move_generator.makeMove(board, move);
}

GameState Engine::getGameState(Board &board, MoveList moves)
{
    if (!moves.empty() && board.half_move_clock < HALF_MOVE_CLOCK_LIMIT)
        return IN_PROGRESS;
    Position king_square = getRightmostSetBit(board.white_to_move ? board.white_king : board.black_king);
    return move_generator.squareUnderAttack(board, king_square, board.white_to_move) ? CHECKMATE : DRAW;
}

uint64 Engine::startPerft(int depth, std::string fen, bool divide, uint64 expected)
{
    assert(depth >= 1 && depth <= 10, "Perft depth must be between 1 and 10");
    Board board = generateBoardFromFEN(fen);
    log(PERFT, "Starting perft with depth: " + std::to_string(depth) + " and FEN: " + fen);
    uint64 start_time = startTimeMeasure();
    uint64 nodes = perft(depth, board, divide);
    endTimeMeasure(start_time, PERFT);
    log(PERFT, "Nodes searched: " + std::to_string(nodes));
    if (expected)
    {
        uint64 diff = nodes > expected ? nodes - expected : expected - nodes;
        assert(nodes == expected, "Expected: " + std::to_string(expected) + "\nDiff: " + std::to_string(diff));
    }
    return nodes;
}

void Engine::startUCI()
{
    std::cout << "ChessEngine started in UCI mode" << std::endl;

    std::string command;
    while (std::getline(std::cin, command))
    {
        if (command == "uci")
        {
            std::cout << "id name Cumshot Chess Engine" << std::endl;
            std::cout << "id author Nico Ohler" << std::endl;
            std::cout << "uciok" << std::endl;
        }
        else if (command == "isready")
            std::cout << "readyok" << std::endl;
        // position [ fen fenstring | startpos ] moves move1 ... movei
        else if (command.substr(0, 8) == "position")
        {
            std::string fen = command.substr(9);
            if (fen == "startpos")
                fen = START_FEN;
            Board board = generateBoardFromFEN(fen);
            log(CHESS_BOARD, "Initialized board with FEN: " + fen);
        }
        else if (command.substr(0, 4) == "go ")
        {
            // parse depth or other parameters if needed
            int depth = 1; // default depth
            startPerft(depth);
        }
        else if (command == "quit")
            exit(0);
    }
}

uint64 Engine::perft(int depth, Board board, bool divide)
{
    // if (depth == 0)
    //     return 1;
    MoveList legal_moves = move_generator.generateLegalMoves(board);
    if (depth == 1)
        return legal_moves.size;

    uint64 total_nodes = 0;
    for (int i = 0; i < legal_moves.size; i++)
    {
        Move move = legal_moves.moves[i];
        move_generator.makeMove(board, move);
        uint64 nodes = perft(depth - 1, board);
        move_generator.unmakeMove(board, move);
        if (divide)
            log(PERFT, getSquareName(move.from) + getSquareName(move.to) + ": " + std::to_string(nodes));
        total_nodes += nodes;
    }
    return total_nodes;
}

void Engine::startSearch(int depth, std::string fen)
{
    Board board = generateBoardFromFEN(fen);
    log(CHESS_BOARD, "Testing search with FEN: " + fen);
    counter = 0;
    uint64 start_time = startTimeMeasure();
    iterativeDeepening(board, depth);
    endTimeMeasure(start_time, SEARCH);
    log(SEARCH, "Search score: " + std::to_string(best_score));
    log(SEARCH, "Evaluated nodes: " + std::to_string(counter));
}

void Engine::iterativeDeepening(Board &board, int max_depth)
{
    best_score = 0;
    best_move = NULL_MOVE; // no previous best move for depth 1

    for (int depth = 1; depth <= max_depth; depth++)
    {
        best_score = search(board, depth, NEG_INFINITY, POS_INFINITY, true);
        log(SEARCH, "Depth " + std::to_string(depth) + " best move " + best_move.toString() +
                        " score " + std::to_string(best_score));
    }
}

void Engine::calculateMoveScores(MoveList &moves, Board &board)
{
    for (int i = 0; i < moves.size; i++)
    {
        Move move = moves.moves[i];
        // search previously best move first
        if (move == best_move)
            moves.scores[i] = POS_INFINITY;

        // Most Valuable Victim – Least Valuable Aggressor
        // i.e., prioritize captures of high value with low value pieces
        else if (move.captured_piece != EMPTY)
            moves.scores[i] = getPieceValue(move.captured_piece) - getPieceValue(move.piece) + 10000;

        // todo add killer moves, history later on
        else
            moves.scores[i] = 0;
    }
}

Move Engine::pickBestMove(MoveList &moves)
{
    int best_move_index = -1;
    Score best_score = NEG_INFINITY;

    for (int i = 0; i < moves.size; i++)
        if (moves.scores[i] > best_score)
        {
            best_score = moves.scores[i];
            best_move_index = i;
        }

    // mark selected move as used => ensure it is not picked again
    if (best_move_index != -1)
        moves.scores[best_move_index] = NEG_INFINITY;

    return moves.moves[best_move_index];
}

// negamax with alpha beta pruning and quiescence
Score Engine::search(Board board, int depth, Score alpha, Score beta, bool root)
{
    // todo quiescence + iterative deepening
    // immediately return evaluation for leaf nodes (max depth, draw, stalemate or checkmate)
    if (depth == 0)
        return evaluateBoard(board);

    MoveList legal_moves = move_generator.generateLegalMoves(board);
    GameState game_state = getGameState(board, legal_moves);
    if (game_state == CHECKMATE)
        return NEG_INFINITY; // todo add ply to prefer slow losses
    if (game_state == DRAW)
        return 0;

    // evaluate moves until pruning possible
    calculateMoveScores(legal_moves, board);
    Score max = NEG_INFINITY;
    for (int i = 0; i < legal_moves.size; i++)
    {
        // Move move = legal_moves.moves[i];
        Move move = pickBestMove(legal_moves);
        move_generator.makeMove(board, move);
        // alpha beta are swapped since the opponent tries to minimize our score
        Score score = -search(board, depth - 1, -beta, -alpha);
        move_generator.unmakeMove(board, move);
        if (score > max)
        {
            max = score;
            if (score > alpha)
            {
                alpha = score;
                if (root)
                    best_move = move;
            }
        }
        // prune since score of current subtree is already worse than beta
        // beta = value of the best move found so far by the parent
        if (beta <= alpha)
            break;
    }
    return max;
}

// todo replace with actual evaluation function
Score Engine::evaluateBoard(Board board)
{
    counter++;
    Score material = 0;
    material += countSetBits(board.white_pawns) * PAWN_VALUE;
    material += countSetBits(board.white_knights) * KNIGHT_VALUE;
    material += countSetBits(board.white_bishops) * BISHOP_VALUE;
    material += countSetBits(board.white_rooks) * ROOK_VALUE;
    material += countSetBits(board.white_queens) * QUEEN_VALUE;
    material += countSetBits(board.white_king) * KING_VALUE;

    material -= countSetBits(board.black_pawns) * PAWN_VALUE;
    material -= countSetBits(board.black_knights) * KNIGHT_VALUE;
    material -= countSetBits(board.black_bishops) * BISHOP_VALUE;
    material -= countSetBits(board.black_rooks) * ROOK_VALUE;
    material -= countSetBits(board.black_queens) * QUEEN_VALUE;
    material -= countSetBits(board.black_king) * KING_VALUE;

    Score score = material;
    // return score from perspective of current player
    if (!board.white_to_move)
        score = -score;
    return score;
}

void printHelp(std::string executable_name)
{
    std::cout << "Usage: " << executable_name << " [-m mode] [-f FEN] [-p ply] [-d] [-h]\n"
              << "  -m mode: Set the engine mode (u for UCI, c for console, p for perft)\n"
              << "  -f FEN: Start the game with the given FEN string\n"
              << "  -p ply: Run perft/search with the given ply/depth/number of half moves)\n"
              << "  -d: Divide perft results\n"
              << "  -h: Show this help message" << std::endl;
}

int main(int argc, char *argv[])
{
    // Initialize engine parameters based on command line arguments
    Mode mode = M_UCI;
    std::string start_fen = START_FEN;
    int depth = 0;
    bool divide = false;
    uint64 expected_perft = 0;
    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "-m" && i + 1 < argc)
            mode = argv[++i][0];
        else if (std::string(argv[i]) == "-f" && i + 1 < argc)
            start_fen = argv[++i];
        else if (std::string(argv[i]) == "-p" && i + 1 < argc)
            depth = std::stoi(argv[++i]);
        else if (std::string(argv[i]) == "-e" && i + 1 < argc)
            expected_perft = std::stoull(argv[++i]);
        else if (std::string(argv[i]) == "-d")
            divide = true;
        else if (std::string(argv[i]) == "-h")
        {
            printHelp(argv[0]);
            exit(0);
        }
        else
        {
            std::cout << "Unknown option: " << argv[i] << std::endl;
            printHelp(argv[0]);
            exit(1);
        }
    }

    // start engine with given parameters
    Engine engine;
    switch (mode)
    {
    case M_UCI:
        engine.startUCI();
        break;
    case M_CONSOLE:
        engine.startConsoleGame(start_fen);
        break;
    case M_PERFT:
        engine.startPerft(depth, start_fen, divide, expected_perft);
        break;
    case M_SEARCH:
        engine.startSearch(depth, start_fen);
        break;
    default:
        std::cout << "Invalid mode. Use 'u' for UCI, 'c' for console or 'p' for perft." << std::endl;
        exit(1);
        break;
    }
    exit(0);
}
