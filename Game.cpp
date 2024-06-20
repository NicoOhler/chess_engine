#include "Game.h"

void Game::loop()
{
    do
    {
        placePiecesOnBoard();
        printBoard();
        std::vector<Move> moves = moveGenerator.generateMoves(board);
        Move move = getLegalMoveFromUser(moves);
        applyMove(move);
    } while (!isGameOver());

    printBoard();
    std::cout << "GAME OVER" << std::endl;
}

void Game::printBoard()
{
    std::cout << (board.white_to_move ? "White" : "Black") << "'s turn" << std::endl
              << std::endl;
    std::cout << "  a b c d e f g h" << std::endl;
    for (char i = 0; i < 8; i++)
    {
        std::cout << 8 - i << " ";
        for (char j = 0; j < 8; j++)
            std::cout << board_to_print[i][j] << " ";
        std::cout << 8 - i << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl
              << std::endl;
}

void Game::placePieceTypeOnBoard(Bitboard piece_type, char piece)
{
    while (piece_type)
    {
        Position position = BitBoard::clearRightmostSetBit(piece_type);
        board_to_print[7 - position / 8][position % 8] = piece;
    }
}

void Game::placePiecesOnBoard()
{
    for (char i = 0; i < 8; i++)
        for (char j = 0; j < 8; j++)
            board_to_print[i][j] = ' ';

    placePieceTypeOnBoard(board.white_pawns, WHITE_PAWN);
    placePieceTypeOnBoard(board.white_rooks, WHITE_ROOK);
    placePieceTypeOnBoard(board.white_knights, WHITE_KNIGHT);
    placePieceTypeOnBoard(board.white_bishops, WHITE_BISHOP);
    placePieceTypeOnBoard(board.white_queens, WHITE_QUEEN);
    placePieceTypeOnBoard(board.white_king, WHITE_KING);
    placePieceTypeOnBoard(board.black_pawns, BLACK_PAWN);
    placePieceTypeOnBoard(board.black_rooks, BLACK_ROOK);
    placePieceTypeOnBoard(board.black_knights, BLACK_KNIGHT);
    placePieceTypeOnBoard(board.black_bishops, BLACK_BISHOP);
    placePieceTypeOnBoard(board.black_queens, BLACK_QUEEN);
    placePieceTypeOnBoard(board.black_king, BLACK_KING);
}

Move Game::getLegalMoveFromUser(std::vector<Move> legal_moves)
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
        for (Move legal_move : legal_moves)
            if (legal_move.from == from && legal_move.to == to)
                return legal_move;
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
    bool white_to_move = board.white_to_move;
    char captured_piece = board_to_print[7 - move.to / 8][move.to % 8];
    Bitboard *opponent_pieces = white_to_move ? &board.black_pieces : &board.white_pieces;
    if (captured_piece != ' ') // remove any captured piece
    {
        BitBoard::clear(*getBitboardByPiece(captured_piece), move.to);
        BitBoard::clear(*opponent_pieces, move.to);
    }

    if (white_to_move ? move.piece == 'P' : move.piece == 'p')
    {
        // handle en passant
        if (move.to == board.en_passant + (white_to_move ? ONE_ROW_UP : ONE_ROW_DOWN)) // remove captured pawn
        {
            Bitboard *opponent_pawns = white_to_move ? &board.black_pawns : &board.white_pawns;
            BitBoard::clear(*opponent_pawns, board.en_passant);
            BitBoard::clear(*opponent_pieces, board.en_passant);
        }

        // detect double pawn push for en passant in next move
        if (white_to_move && move.from / 8 == ROW_2 && move.to / 8 == ROW_4)
            board.en_passant = move.to;
        else if (!white_to_move && move.from / 8 == ROW_7 && move.to / 8 == ROW_5)
            board.en_passant = move.to;
        else
            board.en_passant = NO_EN_PASSANT;
    }

    Bitboard *own_pieces = white_to_move ? &board.white_pieces : &board.black_pieces;
    Bitboard *piece = getBitboardByPiece(move.piece);
    BitBoard::movePiece(*piece, move.from, move.to);
    BitBoard::movePiece(*own_pieces, move.from, move.to);
    BitBoard::movePiece(board.occupied, move.from, move.to);

    if (move.promotion)
    {
        Piece promotion = getPromotionChoice();
        BitBoard::clear(*piece, move.to);
        BitBoard::set(*getBitboardByPiece(promotion), move.to);
    }

    Bitboard king_side = white_to_move ? WHITE_KING_SIDE_CASTLING : BLACK_KING_SIDE_CASTLING;
    Bitboard queen_side = white_to_move ? WHITE_QUEEN_SIDE_CASTLING : BLACK_QUEEN_SIDE_CASTLING;
    if (move.castling & queen_side)
    {
        // clear castling rights
        board.castling_rights &= ~queen_side & ~king_side;

        // move rook, king is already moved
        Bitboard *rook = white_to_move ? &board.white_rooks : &board.black_rooks;
        BitBoard::movePiece(*rook, move.from - 4, move.from - 1);
    }

    board.white_to_move = !board.white_to_move;
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