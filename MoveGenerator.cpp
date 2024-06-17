#include "MoveGenerator.h"

MoveGenerator::MoveGenerator()
{
    initializePawnCaptureMasks();
    initializeKnightMoves();
    initializeBishopBlockers();
    initializeRookBlockers();
    initializeKingMoves();
    initializeRookBishopAttacks();
}

void MoveGenerator::initializePawnCaptureMasks()
{
    log(MOVE_GENERATOR, "Initializing pawn capture masks.\n");
    for (Position square = 0; square < NUM_SQUARES; square++)
    {
        Position row = square / 8;
        Position col = square % 8;

        if (col != COL_A) // attack left
        {
            if (row != ROW_8)
                set(white_pawn_attack_left[square], square + ONE_ROW_UP + ONE_COL_LEFT);
            if (row != ROW_1)
                set(black_pawn_attack_left[square], square + ONE_ROW_DOWN + ONE_COL_LEFT);
        }

        if (col != COL_H) // attack right
        {
            if (row != ROW_8)
                set(white_pawn_attack_right[square], square + ONE_ROW_UP + ONE_COL_RIGHT);
            if (row != ROW_1)
                set(black_pawn_attack_right[square], square + ONE_ROW_DOWN + ONE_COL_RIGHT);
        }
    }
}

void MoveGenerator::initializeKnightMoves()
{
    log(MOVE_GENERATOR, "Initializing knight moves.\n");
    for (Position square = 0; square < NUM_SQUARES; square++)
    {
        Position row = square / 8;
        Position col = square % 8;

        if (col > COL_B && row < ROW_8)
            set(knight_moves[square], square + 2 * ONE_COL_LEFT + ONE_ROW_UP);

        if (col < COL_G && row < ROW_8)
            set(knight_moves[square], square + 2 * ONE_COL_RIGHT + ONE_ROW_UP);

        if (col > COL_A && row < ROW_7)
            set(knight_moves[square], square + ONE_COL_LEFT + 2 * ONE_ROW_UP);

        if (col < COL_H && row < ROW_7)
            set(knight_moves[square], square + ONE_COL_RIGHT + 2 * ONE_ROW_UP);

        if (col > COL_B && row > ROW_1)
            set(knight_moves[square], square + 2 * ONE_COL_LEFT + ONE_ROW_DOWN);

        if (col < COL_G && row > ROW_1)
            set(knight_moves[square], square + 2 * ONE_COL_RIGHT + ONE_ROW_DOWN);

        if (col > COL_A && row > ROW_2)
            set(knight_moves[square], square + ONE_COL_LEFT + 2 * ONE_ROW_DOWN);

        if (col < COL_H && row > ROW_2)
            set(knight_moves[square], square + ONE_COL_RIGHT + 2 * ONE_ROW_DOWN);
    }
}

// todo optimize + clean up
// todo rename, since it generates squares which may block bishop/rook
void MoveGenerator::initializeBishopBlockers()
{
    log(MOVE_GENERATOR, "Initializing bishop blockers.\n");
    for (Position square = 0; square < NUM_SQUARES; square++)
    {
        Position row = square / 8;
        Position col = square % 8;

        for (Position i = square + ONE_ROW_UP_ONE_COL_RIGHT; (i < NUM_SQUARES && (i % 8) != 7); i += ONE_ROW_UP_ONE_COL_RIGHT)
            set(bishop_blockers[square], i);

        for (Position i = square + ONE_ROW_UP_ONE_COL_LEFT; (i < NUM_SQUARES && (i % 8) != 0); i += ONE_ROW_UP_ONE_COL_LEFT)
            set(bishop_blockers[square], i);

        for (Position i = square - ONE_ROW_DOWN_ONE_COL_RIGHT; (i >= 0 && (i % 8) != 7); i += ONE_ROW_DOWN_ONE_COL_RIGHT)
            set(bishop_blockers[square], i);

        for (Position i = square - ONE_ROW_DOWN_ONE_COL_LEFT; (i >= 0 && (i % 8) != 0); i += ONE_ROW_DOWN_ONE_COL_LEFT)
            set(bishop_blockers[square], i);
    }
}

void MoveGenerator::initializeRookBlockers()
{
    log(MOVE_GENERATOR, "Initializing rook blockers.\n");
    for (Position square = 0; square < NUM_SQUARES; square++)
    {
        Position row = square / 8;
        Position col = square % 8;

        for (Position i = square + ONE_COL_RIGHT; (i < NUM_SQUARES && (i % 8) != 7); i += ONE_COL_RIGHT)
            set(rook_blockers[square], i);

        for (Position i = square + ONE_COL_LEFT; (i >= 0 && (i % 8) != 0); i += ONE_COL_LEFT)
            set(rook_blockers[square], i);

        for (Position i = square + ONE_ROW_UP; i < NUM_SQUARES + ONE_ROW_DOWN; i += ONE_ROW_UP)
            set(rook_blockers[square], i);

        for (Position i = square + ONE_ROW_DOWN; i >= ONE_ROW_UP; i += ONE_ROW_DOWN)
            set(rook_blockers[square], i);
    }
}

void MoveGenerator::initializeKingMoves()
{
    log(MOVE_GENERATOR, "Initializing king moves.\n");
    for (Position square = 0; square < NUM_SQUARES; square++)
    {
        Position row = square / 8;
        Position col = square % 8;

        if (col != COL_H)
        {
            set(king_moves[square], square + ONE_COL_RIGHT);
            if (row != ROW_8)
                set(king_moves[square], square + ONE_ROW_UP_ONE_COL_RIGHT);
            if (row != ROW_1)
                set(king_moves[square], square + ONE_ROW_DOWN_ONE_COL_RIGHT);
        }

        if (col != COL_A)
        {
            set(king_moves[square], square + ONE_COL_LEFT);
            if (row != ROW_8)
                set(king_moves[square], square + ONE_ROW_UP_ONE_COL_LEFT);
            if (row != ROW_1)
                set(king_moves[square], square + ONE_ROW_DOWN_ONE_COL_LEFT);
        }
        if (row != ROW_8)
            set(king_moves[square], square + ONE_ROW_UP);

        if (row != ROW_1)
            set(king_moves[square], square + ONE_ROW_DOWN);
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
    for (Position square = 0; square < NUM_SQUARES; square++)
    {
        uint8 rook_relevant_squares = ROOK_RELEVANT_SQUARES[square];
        uint8 bishop_relevant_squares = BISHOP_RELEVANT_SQUARES[square];
        int number_of_rook_attacks = 1 << rook_relevant_squares; // 2^rook_relevant_squares, number of occupancy variations for a square
        int number_of_bishop_attacks = 1 << bishop_relevant_squares;
        int rook_bits = NUM_SQUARES - rook_relevant_squares; // number of bits to shift to get magic index
        int bishop_bits = NUM_SQUARES - bishop_relevant_squares;

        for (int i = 0; i < number_of_rook_attacks; i++)
        {
            // ? is it possible to compute all occupancy variations for a square at once?
            Bitboard occupancy = getOccupancyVariation(i, number_of_rook_attacks, rook_blockers[square]);
            int magic_index = (occupancy * ROOK_MAGICS[square]) >> rook_bits;
            rook_attacks[square][magic_index] = precomputeSlidingRookAttacks(square, occupancy);
        }

        for (int i = 0; i < number_of_bishop_attacks; i++)
        {
            Bitboard occupancy = getOccupancyVariation(i, number_of_bishop_attacks, bishop_blockers[square]);
            int magic_index = (occupancy * BISHOP_MAGICS[square]) >> bishop_bits;
            bishop_attacks[square][magic_index] = precomputeSlidingBishopAttacks(square, occupancy);
        }
    }
}

// todo clean up
// todo check if i can remove i < NUM_SQUARES, since i%8 != 0 should be enough
Bitboard MoveGenerator::precomputeSlidingRookAttacks(Position square, Bitboard occupied)
{
    Bitboard attacks = 0;

    // right
    for (Position i = square + ONE_COL_RIGHT; (i < NUM_SQUARES && (i % 8) != 0); i += ONE_COL_RIGHT)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    // left
    for (Position i = square + ONE_COL_LEFT; (i >= 0 && (i % 8) != 7); i += ONE_COL_LEFT)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    // up
    for (Position i = square + ONE_ROW_UP; i < NUM_SQUARES; i += ONE_ROW_UP)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    // down
    for (Position i = square + ONE_ROW_DOWN; i >= 0; i += ONE_ROW_DOWN)
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

    for (Position i = square + ONE_ROW_UP_ONE_COL_RIGHT; (i < NUM_SQUARES && (i % 8) != 0); i += ONE_ROW_UP_ONE_COL_RIGHT)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    for (Position i = square + ONE_ROW_UP_ONE_COL_LEFT; (i < NUM_SQUARES && (i % 8) != 7); i += ONE_ROW_UP_ONE_COL_LEFT)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    for (Position i = square + ONE_ROW_DOWN_ONE_COL_RIGHT; (i >= 0 && (i % 8) != 0); i += ONE_ROW_DOWN_ONE_COL_RIGHT)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    for (Position i = square - ONE_ROW_DOWN_ONE_COL_LEFT; (i >= 0 && (i % 8) != 7); i += ONE_ROW_DOWN_ONE_COL_LEFT)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    return attacks;
}

std::vector<Move> MoveGenerator::generateMoves(Board &board)
{
    // todo vectors are slow
    std::vector<Move> king_moves = generateKingMoves(board); // generate king moves first, since they depend on squares under attack
    std::vector<Move> moves = generatePawnMoves(board);
    std::vector<Move> knight_moves = generateKnightMoves(board);
    std::vector<Move> bishop_moves = generateBishopMoves(board);
    std::vector<Move> rook_moves = generateRookMoves(board);
    std::vector<Move> queen_moves = generateQueenMoves(board);

    moves.insert(moves.end(), knight_moves.begin(), knight_moves.end());
    moves.insert(moves.end(), bishop_moves.begin(), bishop_moves.end());
    moves.insert(moves.end(), rook_moves.begin(), rook_moves.end());
    moves.insert(moves.end(), queen_moves.begin(), queen_moves.end());
    moves.insert(moves.end(), king_moves.begin(), king_moves.end());

    return moves;
}

std::vector<Move> MoveGenerator::generateKingMoves(Board &board)
{
    std::vector<Move> moves;
    bool white_to_move = board.white_to_move;
    Piece piece = white_to_move ? WHITE_KING : BLACK_KING;
    Bitboard king = white_to_move ? board.white_king : board.black_king;
    Bitboard own_pieces = white_to_move ? board.occupied_by_white : board.occupied_by_black;
    Position square = clearRightmostSetBit(king);
    Bitboard attacks = king_moves[square] & ~own_pieces & ~board.squares_under_attack;

    board.squares_under_attack = attacks; // ? does king safety mechanism work?
    while (attacks)
    {
        Position to = clearRightmostSetBit(attacks);
        moves.push_back(Move{square, to, piece});
        log(KING_MOVE, "Found king move from " + getSquareName(square) + " to " + getSquareName(to) + ".\n");
    }

    // todo castling

    return moves;
}

std::vector<Move> MoveGenerator::generatePawnMoves(Board &board)
{
    std::vector<Move> moves;
    bool white_to_move = board.white_to_move;
    Piece piece = white_to_move ? WHITE_PAWN : BLACK_PAWN;
    Bitboard pawns = white_to_move ? board.white_pawns : board.black_pawns;
    Bitboard *attack_right = white_to_move ? white_pawn_attack_right : black_pawn_attack_right;
    Bitboard *attack_left = white_to_move ? white_pawn_attack_left : black_pawn_attack_left;
    Bitboard start_row = white_to_move ? WHITE_PAWNS_START : BLACK_PAWNS_START;
    Bitboard enemies = white_to_move ? board.occupied_by_black : board.occupied_by_white;
    Direction direction = white_to_move ? ONE_ROW_UP : ONE_ROW_DOWN;
    Direction direction_right = white_to_move ? ONE_ROW_UP_ONE_COL_RIGHT : ONE_ROW_DOWN_ONE_COL_RIGHT;
    Direction direction_left = white_to_move ? ONE_ROW_UP_ONE_COL_LEFT : ONE_ROW_DOWN_ONE_COL_LEFT;
    // todo Bitboard promotion_row = white_to_move ? 0x000000000000FF00 : 0x00FF000000000000;

    // single move
    // right shift for white, left shift for black since negative shift is undefined
    Bitboard single_moves = white_to_move ? (pawns << ONE_ROW_UP) : (pawns >> ONE_ROW_UP);
    single_moves &= ~board.occupied;
    while (single_moves)
    {
        Position to = clearRightmostSetBit(single_moves);
        Position from = to - direction;
        moves.push_back(Move{from, to, piece});
        log(PAWN_MOVE, "Found pawn move from " + getSquareName(from) + " to " + getSquareName(to) + ".\n");
    }
    // double move
    Bitboard double_moves = pawns & start_row;
    double_moves = white_to_move ? (double_moves << ONE_ROW_UP) : (double_moves >> ONE_ROW_UP);
    double_moves &= ~board.occupied;
    double_moves = white_to_move ? (double_moves << ONE_ROW_UP) : (double_moves >> ONE_ROW_UP);
    double_moves &= ~board.occupied;
    while (double_moves)
    {
        Position to = clearRightmostSetBit(double_moves);
        Position from = to - 2 * direction;
        moves.push_back(Move{from, to, piece});
        log(PAWN_MOVE, "Found pawn move from " + getSquareName(from) + " to " + getSquareName(to) + ".\n");
    }

    // for each pawn check attack masks
    while (pawns)
    {
        Position from = clearRightmostSetBit(pawns);

        // attack right
        Position to_right = from + direction_right;
        Bitboard attack = attack_right[from] & enemies;
        board.squares_under_attack |= attack;
        if (attack)
        {
            moves.push_back(Move{from, to_right, piece});
            log(PAWN_MOVE, "Found pawn move from " + getSquareName(from) + " to " + getSquareName(to_right) + ".\n");
        }

        // attack left
        Position to_left = from + direction_left;
        attack = attack_left[from] & enemies;
        board.squares_under_attack |= attack;
        if (attack)
        {
            moves.push_back(Move{from, to_left, piece});
            log(PAWN_MOVE, "Found pawn move from " + getSquareName(from) + " to " + getSquareName(to_left) + ".\n");
        }
    }

    // todo promotion
    // todo en passant

    return moves;
}

// todo duplicate code
std::vector<Move> MoveGenerator::generateKnightMoves(Board &board)
{
    std::vector<Move> moves;
    bool white_to_move = board.white_to_move;
    Piece piece = white_to_move ? WHITE_KNIGHT : BLACK_KNIGHT;
    Bitboard knights = white_to_move ? board.white_knights : board.black_knights;
    Bitboard own_pieces = white_to_move ? board.occupied_by_white : board.occupied_by_black;

    while (knights)
    {
        Position square = clearRightmostSetBit(knights);
        Bitboard attacks = knight_moves[square] & ~own_pieces;
        board.squares_under_attack |= attacks;
        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.push_back(Move{square, to, piece});
            log(KNIGHT_MOVE, "Found knight move from " + getSquareName(square) + " to " + getSquareName(to) + ".\n");
        }
    }

    return moves;
}

std::vector<Move> MoveGenerator::generateBishopMoves(Board &board)
{
    std::vector<Move> moves;
    bool white_to_move = board.white_to_move;
    Piece piece = white_to_move ? WHITE_BISHOP : BLACK_BISHOP;
    Bitboard bishops = white_to_move ? board.white_bishops : board.black_bishops;
    Bitboard own_pieces = white_to_move ? board.occupied_by_white : board.occupied_by_black;

    // for each bishop add each attacked square to moves
    while (bishops)
    {
        Position square = clearRightmostSetBit(bishops);
        // i would love to write an insightful comment here, but i have no idea how magic bitboards work ¯\_(ツ)_/¯
        int magic_index = ((bishop_blockers[square] & board.occupied) * BISHOP_MAGICS[square]) >> (NUM_SQUARES - BISHOP_RELEVANT_SQUARES[square]);
        Bitboard attacks = bishop_attacks[square][magic_index] & ~own_pieces;
        board.squares_under_attack |= attacks;
        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.push_back(Move{square, to, piece});
            log(BISHOP_MOVE, "Found bishop move from " + getSquareName(square) + " to " + getSquareName(to) + ".\n");
        }
    }

    return moves;
}

std::vector<Move> MoveGenerator::generateRookMoves(Board &board)
{
    std::vector<Move> moves;
    bool white_to_move = board.white_to_move;
    Piece piece = white_to_move ? WHITE_ROOK : BLACK_ROOK;
    Bitboard rooks = white_to_move ? board.white_rooks : board.black_rooks;
    Bitboard own_pieces = white_to_move ? board.occupied_by_white : board.occupied_by_black;

    // for each rook add each attacked square to moves
    while (rooks)
    {
        Position square = clearRightmostSetBit(rooks);
        int magic_index = ((rook_blockers[square] & board.occupied) * ROOK_MAGICS[square]) >> (NUM_SQUARES - ROOK_RELEVANT_SQUARES[square]);
        Bitboard attacks = rook_attacks[square][magic_index] & ~own_pieces;
        board.squares_under_attack |= attacks;
        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.push_back(Move{square, to, piece});
            log(ROOK_MOVE, "Found rook move from " + getSquareName(square) + " to " + getSquareName(to) + ".\n");
        }
    }

    return moves;
}

std::vector<Move> MoveGenerator::generateQueenMoves(Board &board)
{
    std::vector<Move> moves;
    bool white_to_move = board.white_to_move;
    Piece piece = white_to_move ? WHITE_QUEEN : BLACK_QUEEN;
    Bitboard queens = white_to_move ? board.white_queens : board.black_queens;
    Bitboard own_pieces = white_to_move ? board.occupied_by_white : board.occupied_by_black;

    // for each queen add each attacked square to moves
    while (queens)
    {
        Position square = clearRightmostSetBit(queens);
        int rook_magic_index = (rook_blockers[square] * ROOK_MAGICS[square]) >> (NUM_SQUARES - ROOK_RELEVANT_SQUARES[square]);
        int bishop_magic_index = (bishop_blockers[square] * BISHOP_MAGICS[square]) >> (NUM_SQUARES - BISHOP_RELEVANT_SQUARES[square]);
        Bitboard attacks = rook_attacks[square][rook_magic_index] | bishop_attacks[square][bishop_magic_index];
        attacks &= ~own_pieces;
        board.squares_under_attack |= attacks;
        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.push_back(Move{square, to, piece});
            log(QUEEN_MOVE, "Found queen move from " + getSquareName(square) + " to " + getSquareName(to) + ".\n");
        }
    }

    return moves;
}
