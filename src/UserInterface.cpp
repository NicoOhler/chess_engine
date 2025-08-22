#include "UserInterface.h"

void UserInterface::start()
{
    switch (mode)
    {
    case UCI_MODE:
        startUCI();
        break;
    case PLAY_MODE:
        startGameLoop();
        break;
    case PERFT_MODE:
        startPerft();
        break;
    case SEARCH_MODE:
        startSearch();
        break;
    default:
        std::cout << "Invalid mode. Use 'u' for UCI, 'c' for console, 'p' for perft or 's' for search." << std::endl;
        exit(1);
        break;
    }
}

void UserInterface::startSearch()
{
    engine.initializeStartPosition(fen);
    log(CHESS_BOARD, "Starting search with a time limit of " + std::to_string(search_time) + "ms");
    engine.search();
}

void UserInterface::startPerft()
{
    engine.initializeStartPosition(fen);
    log(PERFT, "Starting perft with a depth of" + std::to_string(depth));
    timer.start();
    uint64 nodes = engine.perft(depth > 0 ? depth : MAX_SEARCH_DEPTH, divide);
    timer.stop(PERFT);
    log(PERFT, "Nodes searched: " + std::to_string(nodes));
    if (expected_perft)
    {
        uint64 difference = nodes > expected_perft ? nodes - expected_perft : expected_perft - nodes;
        assert(nodes == expected_perft, "Expected: " + std::to_string(expected_perft) + "\nDiff: " + std::to_string(difference));
    }
}

void UserInterface::startUCI()
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
            engine.initializeStartPosition(fen);
        }
        else if (command.substr(0, 4) == "go ")
        {
            // parse depth or other parameters if needed
            startPerft();
        }
        else if (command == "quit")
            exit(0);
    }
}

void UserInterface::startGameLoop()
{
    engine.initializeStartPosition(fen);
    printGameState(engine.getBoard());
    MoveList moves = engine.getLegalMoves();
    bool ai_turn = true;

    do
    {
        Move move = ai_turn ? engine.search() : getLegalMoveFromUser(moves);
        if (play_vs_ai)
            ai_turn = !ai_turn;
        applyAndTrackMove(move);
        printGameState(engine.getBoard());
        moves = engine.getLegalMoves();
    } while (engine.getGameState(moves) == IN_PROGRESS);
    std::cout << (engine.getGameState(moves) == CHECKMATE ? "Checkmate" : "Draw") << std::endl;
}

void UserInterface::applyAndTrackMove(Move move)
{
    if (move.piece == UNDO)
    {
        assert(!move_history.empty(), "No move to undo");
        move = move_history.top();
        move_history.pop();
        engine.unmakeMove(move);
        // also undo ai move
        if (play_vs_ai)
        {
            move = move_history.top();
            move_history.pop();
            engine.unmakeMove(move);
        }
        return;
    }
    move_history.push(move);
    engine.makeMove(move);
}

Move UserInterface::getLegalMoveFromUser(MoveList legal_moves)
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

Piece UserInterface::getPromotionChoice()
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
        switch (choice)
        {
        case WHITE_QUEEN_SYMBOL:
            return WHITE_QUEEN;
        case WHITE_ROOK_SYMBOL:
            return WHITE_ROOK;
        case WHITE_BISHOP_SYMBOL:
            return WHITE_BISHOP;
        case WHITE_KNIGHT_SYMBOL:
            return WHITE_KNIGHT;
        case BLACK_QUEEN_SYMBOL:
            return BLACK_QUEEN;
        case BLACK_ROOK_SYMBOL:
            return BLACK_ROOK;
        case BLACK_BISHOP_SYMBOL:
            return BLACK_BISHOP;
        case BLACK_KNIGHT_SYMBOL:
            return BLACK_KNIGHT;
        default:
            break;
        }
        std::cout << "Invalid choice. Please enter Q, R, B or N." << std::endl;
    }
}

void UserInterface::parseParameters(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "-m" && i + 1 < argc)
            mode = argv[++i][0];
        else if (std::string(argv[i]) == "-f" && i + 1 < argc)
            fen = argv[++i];
        else if (std::string(argv[i]) == "-p" && i + 1 < argc)
            depth = std::stoi(argv[++i]);
        else if (std::string(argv[i]) == "-e" && i + 1 < argc)
            expected_perft = std::stoull(argv[++i]);
        else if (std::string(argv[i]) == "-d")
            divide = true;
        else if (std::string(argv[i]) == "-t" && i + 1 < argc)
        {
            search_time = std::stoull(argv[++i]);
            engine.setTimeLimit(search_time);
        }
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
}

void UserInterface::printHelp(std::string executable_name)
{
    std::cout << "Usage: " << executable_name << " [-m mode] [-f FEN] [-p ply] [-d] [-h]\n"
              << "  -m mode: Set the engine mode (u for UCI, c for console, p for perft)\n"
              << "  -f FEN: Start the game with the given FEN string\n"
              << "  -p ply: Run perft with the given ply/depth/number of half moves)\n"
              << "  -d: Divide perft results\n"
              << "  -t: Set search time (in milliseconds)\n"
              << "  -h: Show this help message" << std::endl;
}

int main(int argc, char *argv[])
{
    UserInterface ui;
    ui.parseParameters(argc, argv);
    ui.start();
    exit(0);
}
