#include "MoveGenerator.h"

MoveGenerator::MoveGenerator()
{
    initializePawnCaptureMasks();
    initializeKnightMoves();
    initializeBishopMoves();
    initializeRookMoves();
    initializeKingMoves();
    initializeRookBishopAttacks();
}

void MoveGenerator::initializePawnCaptureMasks()
{
    log(MOVE_GENERATOR, "Initializing pawn capture masks.\n");
    for (Position square = 0; square < 64; square++)
    {
        Position row = square / 8;
        Position col = square % 8;

        if (col != COL_A) // attack left
        {
            if (row != ROW_8)
                set(white_pawn_attack_left[square], square + 7);
            if (row != ROW_1)
                set(black_pawn_attack_left[square], square - 9);
        }

        if (col != COL_H) // attack right
        {
            if (row != ROW_8)
                set(white_pawn_attack_right[square], square + 9);
            if (row != ROW_1)
                set(black_pawn_attack_right[square], square - 7);
        }
    }
}

void MoveGenerator::initializeKnightMoves()
{
    log(MOVE_GENERATOR, "Initializing knight moves.\n");
    for (Position square = 0; square < 64; square++)
    {
        Position row = square / 8;
        Position col = square % 8;

        if (col > COL_B && row < ROW_8) // 2 right 1 up
            set(knight_moves[square], square + 6);

        if (col < COL_G && row < ROW_8) // 2 left 1 up
            set(knight_moves[square], square + 10);

        if (col > COL_A && row < ROW_7) // 1 right 2 up
            set(knight_moves[square], square + 15);

        if (col < COL_H && row < ROW_7) // 1 left 2 up
            set(knight_moves[square], square + 17);

        if (col > COL_B && row > ROW_1) // 2 right 1 down
            set(knight_moves[square], square - 10);

        if (col < COL_G && row > ROW_1) // 2 left 1 down
            set(knight_moves[square], square - 6);

        if (col > COL_A && row > ROW_2) // 1 right 2 down
            set(knight_moves[square], square - 17);

        if (col < COL_H && row > ROW_2) // 1 left 2 down
            set(knight_moves[square], square - 15);
    }
}

// todo optimize + clean up
// todo rename, since it generates squares which may block bishop/rook
void MoveGenerator::initializeBishopMoves()
{
    log(MOVE_GENERATOR, "Initializing bishop moves.\n");
    for (Position square = 0; square < 64; square++)
    {
        Position row = square / 8;
        Position col = square % 8;

        for (Position i = square + 9; (i < 64 && (i % 8) != 7); i += 9) // top right
            set(bishop_moves[square], i);

        for (Position i = square + 7; (i < 64 && (i % 8) != 0); i += 7) // top left
            set(bishop_moves[square], i);

        for (Position i = square - 7; (i >= 0 && (i % 8) != 7); i -= 7) // bottom right
            set(bishop_moves[square], i);

        for (Position i = square - 9; (i >= 0 && (i % 8) != 0); i -= 9) // bottom left
            set(bishop_moves[square], i);
    }
}

void MoveGenerator::initializeRookMoves()
{
    log(MOVE_GENERATOR, "Initializing rook moves.\n");
    for (Position square = 0; square < 64; square++)
    {
        Position row = square / 8;
        Position col = square % 8;

        for (Position i = square + 1; (i < 64 && (i % 8) != 7); i++) // right
            set(rook_moves[square], i);

        for (Position i = square - 1; (i >= 0 && (i % 8) != 0); i--) // left
            set(rook_moves[square], i);

        for (Position i = square + 8; i < 56; i += 8) // up
            set(rook_moves[square], i);

        for (Position i = square - 8; i >= 8; i -= 8) // down
            set(rook_moves[square], i);
    }
}

void MoveGenerator::initializeKingMoves()
{
    log(MOVE_GENERATOR, "Initializing king moves.\n");
    for (Position square = 0; square < 64; square++)
    {
        Position row = square / 8;
        Position col = square % 8;

        if (col != COL_H) // right
        {
            set(king_moves[square], square + 1);
            if (row != ROW_8) // top right
                set(king_moves[square], square + 9);
            if (row != ROW_1) // bottom right
                set(king_moves[square], square - 7);
        }

        if (col != COL_A) // left
        {
            set(king_moves[square], square - 1);
            if (row != ROW_8) // top left
                set(king_moves[square], square + 7);
            if (row != ROW_1) // bottom left
                set(king_moves[square], square - 9);
        }
        if (row != ROW_8) // up
            set(king_moves[square], square + 8);

        if (row != ROW_1) // down
            set(king_moves[square], square - 8);
    }
}

Bitboard MoveGenerator::getOccupancyVariation(int index, int relevant_bits, Bitboard moves)
{
    Bitboard occupancy = 0;
    for (int i = 0; i < relevant_bits; i++)
    {
        Position square = clearRightmostSetBit(moves);
        // if i-th bit is set in index
        // e.g 3 = 0011 => set 0th and 1st bit
        if (index & (1 << i))
            occupancy |= (1ULL << square);
    }

    return occupancy;
}

void MoveGenerator::initializeRookBishopAttacks()
{
    log(MOVE_GENERATOR, "Initializing rook and bishop attacks.\n");
    for (Position square = 0; square < 64; square++)
    {
        uint8 rook_relevant_squares = ROOK_RELEVANT_SQUARES[square];
        uint8 bishop_relevant_squares = BISHOP_RELEVANT_SQUARES[square];
        int number_of_rook_attacks = 1 << rook_relevant_squares; // 2^rook_relevant_squares, number of occupancy variations for a square
        int number_of_bishop_attacks = 1 << bishop_relevant_squares;
        int rook_bits = 64 - rook_relevant_squares; // number of bits to shift to get magic index
        int bishop_bits = 64 - bishop_relevant_squares;

        for (int i = 0; i < number_of_rook_attacks; i++)
        {
            // ? is it possible to compute all occupancy variations for a square at once?
            Bitboard occupancy = getOccupancyVariation(i, number_of_rook_attacks, rook_moves[square]);
            int magic_index = (occupancy * ROOK_MAGICS[square]) >> rook_bits;
            rook_attacks[square][magic_index] = precomputeSlidingRookAttacks(square, occupancy);
        }

        for (int i = 0; i < number_of_bishop_attacks; i++)
        {
            Bitboard occupancy = getOccupancyVariation(i, number_of_bishop_attacks, bishop_moves[square]);
            int magic_index = (occupancy * BISHOP_MAGICS[square]) >> bishop_bits;
            bishop_attacks[square][magic_index] = precomputeSlidingBishopAttacks(square, occupancy);
        }
    }
}

// todo clean up
// todo check if i can remove i < 64, since i%8 != 0 should be enough
Bitboard MoveGenerator::precomputeSlidingRookAttacks(Position square, Bitboard occupied)
{
    Bitboard attacks = 0;

    // right
    for (Position i = square + 1; (i < 64 && (i % 8) != 0); i++)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    // left
    for (Position i = square - 1; (i >= 0 && (i % 8) != 7); i--)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    // up
    for (Position i = square + 8; i < 64; i += 8)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    // down
    for (Position i = square - 8; i >= 0; i -= 8)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    return attacks;
}

Bitboard MoveGenerator::precomputeSlidingBishopAttacks(Position square, Bitboard occupied)
{
    Bitboard attacks = 0;

    // top right
    for (Position i = square + 9; (i < 64 && (i % 8) != 0); i += 9)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    // top left
    for (Position i = square + 7; (i < 64 && (i % 8) != 7); i += 7)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    // bottom right
    for (Position i = square - 7; (i >= 0 && (i % 8) != 0); i -= 7)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    // bottom left
    for (Position i = square - 9; (i >= 0 && (i % 8) != 7); i -= 9)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    return attacks;
}

std::vector<Move> MoveGenerator::generateMoves(Board board)
{
    // todo vectors are slow
    std::vector<Move> moves = generatePawnMoves(board);
    std::vector<Move> knight_moves = generateKnightMoves(board);
    std::vector<Move> bishop_moves = generateBishopMoves(board);
    std::vector<Move> rook_moves = generateRookMoves(board);
    std::vector<Move> queen_moves = generateQueenMoves(board);
    std::vector<Move> king_moves = generateKingMoves(board);

    moves.insert(moves.end(), knight_moves.begin(), knight_moves.end());
    moves.insert(moves.end(), bishop_moves.begin(), bishop_moves.end());
    moves.insert(moves.end(), rook_moves.begin(), rook_moves.end());
    moves.insert(moves.end(), queen_moves.begin(), queen_moves.end());
    moves.insert(moves.end(), king_moves.begin(), king_moves.end());

    return moves;
}

std::vector<Move> MoveGenerator::generatePawnMoves(Board board)
{
    std::vector<Move> moves;
    bool white_to_move = board.white_to_move;
    Bitboard pawns = white_to_move ? board.white_pawns : board.black_pawns;
    Bitboard *attack_right = white_to_move ? white_pawn_attack_right : black_pawn_attack_right;
    Bitboard *attack_left = white_to_move ? white_pawn_attack_left : black_pawn_attack_left;
    Position direction = white_to_move ? 8 : -8;
    Bitboard start_row = white_to_move ? WHITE_PAWNS_START : BLACK_PAWNS_START;
    Bitboard enemies = white_to_move ? board.occupied_by_black : board.occupied_by_white;
    // todo Bitboard promotion_row = white_to_move ? 0x000000000000FF00 : 0x00FF000000000000;

    // single move
    Bitboard single_moves = pawns << direction;
    single_moves &= ~board.occupied;
    while (single_moves)
    {
        Position square = clearRightmostSetBit(single_moves);
        moves.push_back(Move{square - direction, square, PAWN});
        log(PAWN_MOVE, "Found pawn move from " + getSquareName(square - direction) + " to " + getSquareName(square) + ".\n");
    }
    // double move
    Bitboard double_moves = pawns & start_row;
    double_moves = (double_moves << direction) & ~board.occupied;
    double_moves = (double_moves << direction) & ~board.occupied;
    while (double_moves)
    {
        Position square = clearRightmostSetBit(double_moves);
        moves.push_back(Move{square - 2 * direction, square, PAWN});
        log(PAWN_MOVE, "Found pawn move from " + getSquareName(square - 2 * direction) + " to " + getSquareName(square) + ".\n");
    }

    // for each pawn check attack masks
    while (pawns)
    {
        // count trailing zeros
        int8 square = clearRightmostSetBit(pawns);

        // attack right
        Bitboard attack = attack_right[square] & enemies;
        if (attack)
        {
            moves.push_back(Move{square, square + direction + 1, PAWN});
            log(PAWN_MOVE, "Found pawn move from " + getSquareName(square) + " to " + getSquareName(square + direction + 1) + ".\n");
        }

        // attack left
        attack = attack_left[square] & enemies;
        if (attack)
        {
            moves.push_back(Move{square, square + direction - 1, PAWN});
            log(PAWN_MOVE, "Found pawn move from " + getSquareName(square) + " to " + getSquareName(square + direction - 1) + ".\n");
        }
    }

    // todo promotion
    // todo en passant

    return moves;
}

// todo duplicate code
std::vector<Move> MoveGenerator::generateKnightMoves(Board board)
{
    std::vector<Move> moves;
    bool white_to_move = board.white_to_move;
    Bitboard knights = white_to_move ? board.white_knights : board.black_knights;
    Bitboard own_pieces = white_to_move ? board.occupied_by_white : board.occupied_by_black;

    while (knights)
    {
        Position square = clearRightmostSetBit(knights);
        Bitboard attacks = knight_moves[square] & ~own_pieces;
        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.push_back(Move{square, to, KNIGHT});
            log(KNIGHT_MOVE, "Found knight move from " + getSquareName(square) + " to " + getSquareName(to) + ".\n");
        }
    }

    return moves;
}

std::vector<Move> MoveGenerator::generateBishopMoves(Board board)
{
    std::vector<Move> moves;
    bool white_to_move = board.white_to_move;
    Bitboard bishops = white_to_move ? board.white_bishops : board.black_bishops;
    Bitboard own_pieces = white_to_move ? board.occupied_by_white : board.occupied_by_black;

    // for each bishop add each attacked square to moves
    while (bishops)
    {
        Position square = clearRightmostSetBit(bishops);
        // i would love to write an insightful comment here, but i have no idea how magic bitboards work ¯\_(ツ)_/¯
        int magic_index = ((bishop_moves[square] & board.occupied) * BISHOP_MAGICS[square]) >> (64 - BISHOP_RELEVANT_SQUARES[square]);
        Bitboard attacks = bishop_attacks[square][magic_index] & ~own_pieces;

        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.push_back(Move{square, to, BISHOP});
            log(BISHOP_MOVE, "Found bishop move from " + getSquareName(square) + " to " + getSquareName(to) + ".\n");
        }
    }

    return moves;
}

std::vector<Move> MoveGenerator::generateRookMoves(Board board)
{
    std::vector<Move> moves;
    bool white_to_move = board.white_to_move;
    Bitboard rooks = white_to_move ? board.white_rooks : board.black_rooks;
    Bitboard own_pieces = white_to_move ? board.occupied_by_white : board.occupied_by_black;

    // for each rook add each attacked square to moves
    while (rooks)
    {
        Position square = clearRightmostSetBit(rooks);
        int magic_index = ((rook_moves[square] & board.occupied) * ROOK_MAGICS[square]) >> (64 - ROOK_RELEVANT_SQUARES[square]);
        Bitboard attacks = rook_attacks[square][magic_index] & ~own_pieces;

        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.push_back(Move{square, to, ROOK});
            log(ROOK_MOVE, "Found rook move from " + getSquareName(square) + " to " + getSquareName(to) + ".\n");
        }
    }

    return moves;
}

std::vector<Move> MoveGenerator::generateQueenMoves(Board board)
{
    std::vector<Move> moves;
    bool white_to_move = board.white_to_move;
    Bitboard queens = white_to_move ? board.white_queens : board.black_queens;
    Bitboard own_pieces = white_to_move ? board.occupied_by_white : board.occupied_by_black;

    // for each queen add each attacked square to moves
    while (queens)
    {
        Position square = clearRightmostSetBit(queens);
        int rook_magic_index = (rook_moves[square] * ROOK_MAGICS[square]) >> (64 - ROOK_RELEVANT_SQUARES[square]);
        int bishop_magic_index = (bishop_moves[square] * BISHOP_MAGICS[square]) >> (64 - BISHOP_RELEVANT_SQUARES[square]);
        Bitboard attacks = rook_attacks[square][rook_magic_index] | bishop_attacks[square][bishop_magic_index];
        attacks &= ~own_pieces;

        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.push_back(Move{square, to, QUEEN});
            log(QUEEN_MOVE, "Found queen move from " + getSquareName(square) + " to " + getSquareName(to) + ".\n");
        }
    }

    return moves;
}

std::vector<Move> MoveGenerator::generateKingMoves(Board board)
{
    std::vector<Move> moves;
    bool white_to_move = board.white_to_move;
    Bitboard king = white_to_move ? board.white_king : board.black_king;
    Bitboard own_pieces = white_to_move ? board.occupied_by_white : board.occupied_by_black;

    while (king)
    {
        Position square = clearRightmostSetBit(king);
        Bitboard attacks = king_moves[square] & ~own_pieces;
        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.push_back(Move{square, to, KING});
            log(KING_MOVE, "Found king move from " + getSquareName(square) + " to " + getSquareName(to) + ".\n");
        }
    }

    // todo castling

    return moves;
}
