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
