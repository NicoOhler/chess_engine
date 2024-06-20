#include "Game.h"

void Game::loop()
{
    do
    {
        printBoard();
        std::vector<Move> moves = moveGenerator.generateMoves(board);
        Move move = getLegalMoveFromUser(moves);
        applyMove(move);
        board.white_to_move = !board.white_to_move;
    } while (!isGameOver());

    printBoard();
    std::cout << "GAME OVER" << std::endl;
}

// todo use logger
void Game::printBoard()
{
    std::cout << (board.white_to_move ? "White" : "Black") << "'s turn" << std::endl;
    std::cout << std::endl
              << "  a b c d e f g h" << std::endl;
    for (char i = 0; i < 8; i++)
    {
        std::cout << 8 - i << " ";
        for (char j = 0; j < 8; j++)
            std::cout << printable_board[i][j] << " ";
        std::cout << 8 - i << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl
              << std::endl;
}

Move Game::getLegalMoveFromUser(std::vector<Move> legal_moves)
{
    // allow only positions from a1 to h8
    // todo add support for proper chess notation
    // todo pawn promotion
    char from[2], to[2];
    Move move;
    while (true)
    {
        std::cout << "Enter your move: " << std::endl;
        std::cout << "From: ";
        std::cin >> from;
        std::cout << "To: ";
        std::cin >> to;

        if (from[0] < 'a' || from[0] > 'h' || from[1] < '1' || from[1] > '8' || to[0] < 'a' || to[0] > 'h' || to[1] < '1' || to[1] > '8')
        {
            std::cout << "Invalid move. Please enter a move in the format a1-h8" << std::endl;
            continue;
        }

        char from_row = from[1] - '1';
        char from_col = from[0] - 'a';
        char to_row = to[1] - '1';
        char to_col = to[0] - 'a';

        move.from = 8 * from_row + from_col;
        move.to = 8 * to_row + to_col;
        move.piece = printable_board[7 - from_row][from_col]; // printable_board is upside down

        std::cout << std::endl;

        // return move if legal
        for (Move legal_move : legal_moves)
            if (move == legal_move)
                return move;
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
            return board.white_to_move ? toupper(choice) : tolower(choice);
        std::cout << "Invalid choice. Please enter Q, R, B or N." << std::endl;
    }
}

Bitboard *Game::getBitboardByPiece(char piece)
{
    switch (piece)
    {
    case 'P':
        return &board.white_pawns;
    case 'R':
        return &board.white_rooks;
    case 'N':
        return &board.white_knights;
    case 'B':
        return &board.white_bishops;
    case 'Q':
        return &board.white_queens;
    case 'K':
        return &board.white_king;
    case 'p':
        return &board.black_pawns;
    case 'r':
        return &board.black_rooks;
    case 'n':
        return &board.black_knights;
    case 'b':
        return &board.black_bishops;
    case 'q':
        return &board.black_queens;
    case 'k':
        return &board.black_king;
    }
    exit(1);
}

void Game::applyMove(Move move)
{
    Bitboard *piece = getBitboardByPiece(move.piece);
    Bitboard *opponent_pieces = board.white_to_move ? &board.black_pieces : &board.white_pieces;
    Bitboard *own_pieces = board.white_to_move ? &board.white_pieces : &board.black_pieces;

    char captured_piece = printable_board[7 - move.to / 8][move.to % 8];
    if (captured_piece != ' ') // remove any captured piece
    {
        BitBoard::clear(*getBitboardByPiece(captured_piece), move.to);
        BitBoard::clear(*opponent_pieces, move.to);
    }

    bool white_to_move = board.white_to_move;

    if (white_to_move ? move.piece == 'P' : move.piece == 'p')
    {
        // handle en passant
        if (move.to == board.en_passant + (white_to_move ? ONE_ROW_UP : ONE_ROW_DOWN)) // remove captured pawn
        {
            Bitboard *opponent_pawns = white_to_move ? &board.black_pawns : &board.white_pawns;
            BitBoard::clear(*opponent_pawns, board.en_passant);
            BitBoard::clear(*opponent_pieces, board.en_passant);
            printable_board[7 - board.en_passant / 8][board.en_passant % 8] = ' ';
        }

        // detect double pawn push for en passant in next move
        if (white_to_move && move.from / 8 == ROW_2 && move.to / 8 == ROW_4)
            board.en_passant = move.to;
        else if (!white_to_move && move.from / 8 == ROW_7 && move.to / 8 == ROW_5)
            board.en_passant = move.to;
        else
            board.en_passant = NO_EN_PASSANT;
    }

    BitBoard::movePiece(*piece, move.from, move.to);
    BitBoard::movePiece(*own_pieces, move.from, move.to);
    BitBoard::movePiece(board.occupied, move.from, move.to);
    printable_board[ROW_8 - move.from / 8][move.from % 8] = ' ';
    printable_board[ROW_8 - move.to / 8][move.to % 8] = move.piece;

    if (move.promotion)
    {
        Piece promotion = getPromotionChoice();
        BitBoard::clear(*piece, move.to);
        BitBoard::set(*getBitboardByPiece(promotion), move.to);
        printable_board[ROW_8 - move.to / 8][move.to % 8] = promotion;
    }
}

bool Game::isGameOver()
{
    // todo
    return false;
}

int main()
{
    Game game;
    game.loop();
    return 0;
}