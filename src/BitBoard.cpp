#include "BitBoard.h"

void BitBoard::movePiece(Bitboard &board, Position from, Position to)
{
    BitBoard::clear(board, from);
    BitBoard::set(board, to);
}

void BitBoard::set(Bitboard &board, Position position)
{
    board |= 1ULL << position;
}

void BitBoard::clear(Bitboard &board, Position position)
{
    board &= ~(1ULL << position);
}

bool BitBoard::isSet(Bitboard board, Position position)
{
    return board & (1ULL << position);
}

std::vector<bool> BitBoard::getBits(Bitboard board)
{
    std::vector<bool> bits(NUM_SQUARES);
    for (int i = 0; i < NUM_SQUARES; i++)
        bits[i] = board & (1ULL << i);
    return bits;
}

void BitBoard::printBitboard(Bitboard board)
{
    std::vector<bool> bits = getBits(board);
    std::cout << std::endl;
    for (Position row = NUM_ROWS - 1; row >= 0; row--)
    {
        std::cout << row + 1 << " ";
        for (Position col = 0; col < NUM_COLS; col++)
            std::cout << bits[row * NUM_COLS + col] ? "1" : "0";
        std::cout << std::endl;
    }
    std::cout << "  ";
    for (Position col = 0; col < NUM_COLS; col++)
        std::cout << (char)('a' + col);
    std::cout << std::endl
              << std::endl;
}
int8 BitBoard::countSetBits(Bitboard board)
{
    return __builtin_popcountll(board);
}
Position BitBoard::clearRightmostSetBit(Bitboard &board)
{
    Position index_of_LSB = __builtin_ctzll(board);
    board &= board - 1;
    return index_of_LSB;
}

Position BitBoard::getRightmostSetBit(Bitboard &board)
{
    return __builtin_ctzll(board);
}

BitBoard::Board BitBoard::generateBoardFromFEN(std::string FEN)
{
    Board board = Board(EMPTY);

    int i = 0;
    // parse board position
    for (int row = ROW_8; row >= ROW_1; row--)
    {
        for (int col = COL_A; col <= COL_H; col++)
        {
            Position square = 8 * row + col;
            char c = FEN[i++];

            if (c >= '1' && c <= '8')
            {
                col += c - '1';
                continue;
            }

            Bitboard *piece = board.getBitboardByPiece(c);
            BitBoard::set(*piece, square);
            BitBoard::set(board.occupied, square);
            BitBoard::set(isupper(c) ? board.white_pieces : board.black_pieces, square);
        }
        assert(FEN[i++] == (row != ROW_1 ? '/' : ' '), "Invalid FEN");
    }

    // parse side to move, castling rights, en passant square
    board.white_to_move = FEN[i++] == 'w';
    assert(FEN[i++] == ' ', "Invalid FEN");
    if (FEN[i] == '-')
    {
        board.castling_rights = 0;
        i++;
    }
    else
        for (int j = 0; j < 4; j++)
        {
            if (FEN[i] == ' ')
                break;
            switch (FEN[i++])
            {
            case WHITE_KING:
                board.castling_rights |= WHITE_KING_SIDE_CASTLING;
                break;
            case WHITE_QUEEN:
                board.castling_rights |= WHITE_QUEEN_SIDE_CASTLING;
                break;
            case BLACK_KING:
                board.castling_rights |= BLACK_KING_SIDE_CASTLING;
                break;
            case BLACK_QUEEN:
                board.castling_rights |= BLACK_QUEEN_SIDE_CASTLING;
                break;
            default:
                assert(false, "Invalid FEN");
            }
        }

    assert(FEN[i++] == ' ', "Invalid FEN");
    if (FEN[i] == '-')
        board.en_passant = NO_EN_PASSANT;
    else
    {
        std::string square = FEN.substr(i, 2);
        board.en_passant = getSquareIndex(square);
        i += 2;
    }
    // ignore halfmove clock and fullmove number for now
    return board;
}

Bitboard *BitBoard::Board::getBitboardByPiece(char piece)
{
    switch (piece)
    {
    case WHITE_PAWN:
        return &white_pawns;
    case WHITE_ROOK:
        return &white_rooks;
    case WHITE_KNIGHT:
        return &white_knights;
    case WHITE_BISHOP:
        return &white_bishops;
    case WHITE_QUEEN:
        return &white_queens;
    case WHITE_KING:
        return &white_king;
    case BLACK_PAWN:
        return &black_pawns;
    case BLACK_ROOK:
        return &black_rooks;
    case BLACK_KNIGHT:
        return &black_knights;
    case BLACK_BISHOP:
        return &black_bishops;
    case BLACK_QUEEN:
        return &black_queens;
    case BLACK_KING:
        return &black_king;
    default:
        assert(false, "Invalid piece");
    }
    return nullptr;
}

void BitBoard::Board::capturePiece(Position position)
{
    clear(white_to_move ? black_pieces : white_pieces, position);
    clear(white_to_move ? black_pawns : white_pawns, position);
    clear(white_to_move ? black_rooks : white_rooks, position);
    clear(white_to_move ? black_knights : white_knights, position);
    clear(white_to_move ? black_bishops : white_bishops, position);
    clear(white_to_move ? black_queens : white_queens, position);
    clear(occupied, position);
}

void BitBoard::printGameState(Board board)
{
    printBoard(board);
    if (board.en_passant != NO_EN_PASSANT)
        std::cout << "En passant square: " << getSquareName(board.en_passant) << std::endl;
    printCastlingRights(board);
    std::cout << (board.white_to_move ? "White" : "Black") << "'s turn" << std::endl
              << std::endl;
}

void BitBoard::printBoard(Board board)
{
    char board_to_print[8][8];
    placePiecesOnBoard(board, board_to_print);
    std::cout << std::endl
              << "  a b c d e f g h" << std::endl;
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

void BitBoard::printCastlingRights(Board board)
{
    std::cout << "Castling rights: ";
    if (board.castling_rights & WHITE_KING_SIDE_CASTLING)
        std::cout << "K";
    if (board.castling_rights & WHITE_QUEEN_SIDE_CASTLING)
        std::cout << "Q";
    if (board.castling_rights & BLACK_KING_SIDE_CASTLING)
        std::cout << "k";
    if (board.castling_rights & BLACK_QUEEN_SIDE_CASTLING)
        std::cout << "q";
    std::cout << std::endl;
}

void BitBoard::placePiecesOnBoard(Board board, char board_to_print[8][8])
{
    Bitboard pieces[12] = {board.white_pawns, board.white_rooks, board.white_knights, board.white_bishops, board.white_queens, board.white_king,
                           board.black_pawns, board.black_rooks, board.black_knights, board.black_bishops, board.black_queens, board.black_king};
    Piece piece_types[12] = {WHITE_PAWN, WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING,
                             BLACK_PAWN, BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING};
    for (char i = 0; i < 8; i++)
        for (char j = 0; j < 8; j++)
            board_to_print[i][j] = ' ';

    for (int i = 0; i < 12; i++)
    {
        while (pieces[i])
        {
            Position position = BitBoard::clearRightmostSetBit(pieces[i]);
            board_to_print[7 - position / 8][position % 8] = piece_types[i];
        }
    }
}
