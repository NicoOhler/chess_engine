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
    log(MOVE_GENERATOR, "Initializing pawn capture masks.");
    for (Position square = 0; square < NUM_SQUARES; square++)
    {
        Position row = square / 8;
        Position col = square % 8;

        if (col != COL_A)
        {
            if (row != ROW_8)
                set(white_pawn_attack_left[square], square + ONE_ROW_UP_ONE_COL_LEFT);
            if (row != ROW_1)
                set(black_pawn_attack_left[square], square + ONE_ROW_DOWN_ONE_COL_LEFT);
        }

        if (col != COL_H)
        {
            if (row != ROW_8)
                set(white_pawn_attack_right[square], square + ONE_ROW_UP_ONE_COL_RIGHT);
            if (row != ROW_1)
                set(black_pawn_attack_right[square], square + ONE_ROW_DOWN_ONE_COL_RIGHT);
        }
    }
}

void MoveGenerator::initializeKnightMoves()
{
    log(MOVE_GENERATOR, "Initializing knight moves.");
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

void MoveGenerator::initializeBishopBlockers()
{
    log(MOVE_GENERATOR, "Initializing bishop blockers.");
    for (Position square = 0; square < NUM_SQUARES; square++)
    {
        Position row = square / 8;
        Position col = square % 8;

        if (row < ROW_8)
        {
            if (col < COL_H)
                for (Position i = square + ONE_ROW_UP_ONE_COL_RIGHT; (i < NUM_SQUARES + ONE_ROW_DOWN && (i % 8) != COL_H); i += ONE_ROW_UP_ONE_COL_RIGHT)
                    set(bishop_blockers[square], i);

            if (col > COL_A)
                for (Position i = square + ONE_ROW_UP_ONE_COL_LEFT; (i < NUM_SQUARES + ONE_ROW_DOWN && (i % 8) != COL_A); i += ONE_ROW_UP_ONE_COL_LEFT)
                    set(bishop_blockers[square], i);
        }

        if (row > ROW_1)
        {
            if (col < COL_H)
                for (Position i = square + ONE_ROW_DOWN_ONE_COL_RIGHT; (i >= ONE_ROW_UP && (i % 8) != COL_H); i += ONE_ROW_DOWN_ONE_COL_RIGHT)
                    set(bishop_blockers[square], i);

            if (col > COL_A)
                for (Position i = square + ONE_ROW_DOWN_ONE_COL_LEFT; (i >= ONE_ROW_UP && (i % 8) != COL_A); i += ONE_ROW_DOWN_ONE_COL_LEFT)
                    set(bishop_blockers[square], i);
        }

        assert(countSetBits(bishop_blockers[square]) == BISHOP_RELEVANT_SQUARES[square], "Bishop blockers count does not match relevant squares.");
    }
}

void MoveGenerator::initializeRookBlockers()
{
    log(MOVE_GENERATOR, "Initializing rook blockers.");
    for (Position square = 0; square < NUM_SQUARES; square++)
    {
        Position row = square / 8;
        Position col = square % 8;

        if (col < COL_H)
            for (Position i = square + ONE_COL_RIGHT; (i % 8) != COL_H; i += ONE_COL_RIGHT)
                set(rook_blockers[square], i);

        if (col > COL_A)
            for (Position i = square + ONE_COL_LEFT; (i % 8) != COL_A; i += ONE_COL_LEFT)
                set(rook_blockers[square], i);

        for (Position i = square + ONE_ROW_UP; i < NUM_SQUARES + ONE_ROW_DOWN; i += ONE_ROW_UP)
            set(rook_blockers[square], i);

        for (Position i = square + ONE_ROW_DOWN; i >= ONE_ROW_UP; i += ONE_ROW_DOWN)
            set(rook_blockers[square], i);

        assert(countSetBits(rook_blockers[square]) == ROOK_RELEVANT_SQUARES[square], "Rook blockers count does not match relevant squares.");
    }
}

void MoveGenerator::initializeKingMoves()
{
    log(MOVE_GENERATOR, "Initializing king moves.");
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

void MoveGenerator::addPawnMove(std::vector<Move> &moves, Move move, Board &board)
{
    // regular move
    if (move.to >= ONE_ROW_UP && move.to < NUM_SQUARES + ONE_ROW_DOWN)
    {
        moves.push_back(move);
        log(PAWN_MOVE, "Found pawn move from " + getSquareName(move.from) + " to " + getSquareName(move.to) + ".");
        return;
    }

    // pawn promotion
    bool white_to_move = board.white_to_move;
    move.promotion = white_to_move ? WHITE_QUEEN : BLACK_QUEEN;
    moves.push_back(move);
    move.promotion = white_to_move ? WHITE_ROOK : BLACK_ROOK;
    moves.push_back(move);
    move.promotion = white_to_move ? WHITE_BISHOP : BLACK_BISHOP;
    moves.push_back(move);
    move.promotion = white_to_move ? WHITE_KNIGHT : BLACK_KNIGHT;
    moves.push_back(move);
    log(PAWN_MOVE, "Found pawn promotion from " + getSquareName(move.from) + " to " + getSquareName(move.to) + ".");
}

void MoveGenerator::addCastlingMoves(std::vector<Move> &moves, Board &board)
{
    Piece piece = board.white_to_move ? WHITE_KING : BLACK_KING;
    Position king_start = board.white_to_move ? WHITE_KING_START_POSITION : BLACK_KING_START_POSITION;
    Bitboard king_side_castling = board.white_to_move ? WHITE_KING_SIDE_CASTLING : BLACK_KING_SIDE_CASTLING;
    if (board.castling_rights & king_side_castling) // not possible if king or rook has moved
    {
        Bitboard king_side_blocked = board.occupied & king_side_castling;              // squares between king and rook must be empty
        Bitboard king_side_attacked = board.squares_under_attack & king_side_castling; // squares in between must not be attacked
        if (!king_side_blocked && !king_side_attacked)
        {
            Position to = king_start + ONE_COL_RIGHT * 2;
            moves.push_back(Move{king_start, to, piece, 0, king_side_castling});
            log(KING_MOVE, "Found king side castling from " + getSquareName(king_start) + " to " + getSquareName(to) + ".");
        }
    }

    Bitboard queen_side_castling = board.white_to_move ? WHITE_QUEEN_SIDE_CASTLING : BLACK_QUEEN_SIDE_CASTLING;
    if (board.castling_rights & queen_side_castling)
    {
        Bitboard queen_side_blocked = board.occupied & queen_side_castling;
        Bitboard queen_side_attacked = board.squares_under_attack & queen_side_castling;
        if (!queen_side_blocked && !queen_side_attacked)
        {
            Position to = king_start + ONE_COL_LEFT * 2;
            moves.push_back(Move{king_start, to, piece, 0, queen_side_castling});
            log(KING_MOVE, "Found queen side castling from " + getSquareName(king_start) + " to " + getSquareName(to) + ".");
        }
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
    log(MOVE_GENERATOR, "Initializing rook and bishop attacks.");
    for (Position square = 0; square < NUM_SQUARES; square++)
    {
        int8 rook_relevant_squares = ROOK_RELEVANT_SQUARES[square];
        int8 bishop_relevant_squares = BISHOP_RELEVANT_SQUARES[square];
        int number_of_rook_attacks = 1 << rook_relevant_squares; // 2^rook_relevant_squares, number of occupancy variations for a square
        int number_of_bishop_attacks = 1 << bishop_relevant_squares;
        int8 rook_bits = NUM_SQUARES - rook_relevant_squares; // number of bits to shift to get magic index
        int8 bishop_bits = NUM_SQUARES - bishop_relevant_squares;

        for (int i = 0; i < number_of_rook_attacks; i++)
        {
            // ? is it possible to compute all occupancy variations for a square at once?
            Bitboard occupancy = getOccupancyVariation(i, rook_relevant_squares, rook_blockers[square]);
            int magic_index = (occupancy * ROOK_MAGICS[square]) >> rook_bits;
            rook_attacks[square][magic_index] = precomputeSlidingRookAttacks(square, occupancy);
        }

        for (int i = 0; i < number_of_bishop_attacks; i++)
        {
            Bitboard occupancy = getOccupancyVariation(i, bishop_relevant_squares, bishop_blockers[square]);
            int magic_index = (occupancy * BISHOP_MAGICS[square]) >> bishop_bits;
            bishop_attacks[square][magic_index] = precomputeSlidingBishopAttacks(square, occupancy);
        }
    }
}

Bitboard MoveGenerator::precomputeSlidingRookAttacks(Position square, Bitboard occupied)
{
    Bitboard attacks = 0;
    for (Position i = square + ONE_COL_RIGHT; (i % 8) != COL_A; i += ONE_COL_RIGHT)
    {
        BitBoard::set(attacks, i);
        if (BitBoard::isSet(occupied, i))
            break;
    }

    // need 1 >= 0 since modulo of negative numbers are negative in C
    for (Position i = square + ONE_COL_LEFT; i >= 0 && (i % 8) != COL_H; i += ONE_COL_LEFT)
    {
        BitBoard::set(attacks, i);
        if (BitBoard::isSet(occupied, i))
            break;
    }

    for (Position i = square + ONE_ROW_UP; i < NUM_SQUARES; i += ONE_ROW_UP)
    {
        BitBoard::set(attacks, i);
        if (BitBoard::isSet(occupied, i))
            break;
    }

    for (Position i = square + ONE_ROW_DOWN; i >= ROW_1; i += ONE_ROW_DOWN)
    {
        BitBoard::set(attacks, i);
        if (BitBoard::isSet(occupied, i))
            break;
    }

    return attacks;
}

Bitboard MoveGenerator::precomputeSlidingBishopAttacks(Position square, Bitboard occupied)
{
    Bitboard attacks = 0;
    for (Position i = square + ONE_ROW_UP_ONE_COL_RIGHT; (i < NUM_SQUARES && (i % 8) != COL_A); i += ONE_ROW_UP_ONE_COL_RIGHT)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    for (Position i = square + ONE_ROW_UP_ONE_COL_LEFT; (i < NUM_SQUARES && (i % 8) != COL_H); i += ONE_ROW_UP_ONE_COL_LEFT)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    for (Position i = square + ONE_ROW_DOWN_ONE_COL_RIGHT; (i >= ROW_1 && (i % 8) != COL_A); i += ONE_ROW_DOWN_ONE_COL_RIGHT)
    {
        attacks |= (1ULL << i);
        if (occupied & (1ULL << i))
            break;
    }

    for (Position i = square + ONE_ROW_DOWN_ONE_COL_LEFT; (i >= ROW_1 && (i % 8) != COL_H); i += ONE_ROW_DOWN_ONE_COL_LEFT)
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
    Bitboard own_pieces = white_to_move ? board.white_pieces : board.black_pieces;
    Position from = clearRightmostSetBit(king);
    Bitboard attacks = king_moves[from] & ~own_pieces & ~board.squares_under_attack;

    while (attacks)
    {
        Position to = clearRightmostSetBit(attacks);
        moves.push_back(Move{from, to, piece});
        log(KING_MOVE, "Found king move from " + getSquareName(from) + " to " + getSquareName(to) + ".");
    }

    if (!(board.squares_under_attack & king)) // king must not be in check
        addCastlingMoves(moves, board);

    board.squares_under_attack = attacks; // ? does king safety mechanism work?
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
    Bitboard enemies = white_to_move ? board.black_pieces : board.white_pieces;
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
        addPawnMove(moves, Move{to - direction, to, piece}, board);
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
        log(PAWN_MOVE, "Found pawn move from " + getSquareName(from) + " to " + getSquareName(to) + ".");
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
            addPawnMove(moves, Move{from, to_right, piece}, board);

        // attack left
        Position to_left = from + direction_left;
        attack = attack_left[from] & enemies;
        board.squares_under_attack |= attack;
        if (attack)
            addPawnMove(moves, Move{from, to_left, piece}, board);

        // right en passant
        if (from % 8 != COL_H && board.en_passant == from + ONE_COL_RIGHT)
        {
            moves.push_back(Move{from, to_right, piece});
            log(PAWN_MOVE, "Found en passant move from " + getSquareName(from) + " to " + getSquareName(to_right) + ".");
        }

        // left en passant
        if (from % 8 != COL_A && board.en_passant == from + ONE_COL_LEFT)
        {
            moves.push_back(Move{from, to_left, piece});
            log(PAWN_MOVE, "Found en passant move from " + getSquareName(from) + " to " + getSquareName(to_left) + ".");
        }
    }

    return moves;
}

// todo duplicate code
std::vector<Move> MoveGenerator::generateKnightMoves(Board &board)
{
    std::vector<Move> moves;
    bool white_to_move = board.white_to_move;
    Piece piece = white_to_move ? WHITE_KNIGHT : BLACK_KNIGHT;
    Bitboard knights = white_to_move ? board.white_knights : board.black_knights;
    Bitboard own_pieces = white_to_move ? board.white_pieces : board.black_pieces;

    while (knights)
    {
        Position square = clearRightmostSetBit(knights);
        Bitboard attacks = knight_moves[square] & ~own_pieces;
        board.squares_under_attack |= attacks;
        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.push_back(Move{square, to, piece});
            log(KNIGHT_MOVE, "Found knight move from " + getSquareName(square) + " to " + getSquareName(to) + ".");
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
    Bitboard own_pieces = white_to_move ? board.white_pieces : board.black_pieces;

    // for each bishop add each attacked square to moves
    while (bishops)
    {
        Position square = clearRightmostSetBit(bishops);
        // i would love to write an insightful comment here, but i have no idea how magic bitboards work ¯\_(ツ)_/¯
        int magic_index = ((bishop_blockers[square] & board.occupied) * BISHOP_MAGICS[square]) >> (NUM_SQUARES - BISHOP_RELEVANT_SQUARES[square]);
        Bitboard attacks = bishop_attacks[square][magic_index] & ~own_pieces;
        board.squares_under_attack |= attacks;
        // add each square attacked by current bishop to moves
        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.push_back(Move{square, to, piece});
            log(BISHOP_MOVE, "Found bishop move from " + getSquareName(square) + " to " + getSquareName(to) + ".");
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
    Bitboard own_pieces = white_to_move ? board.white_pieces : board.black_pieces;

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
            log(ROOK_MOVE, "Found rook move from " + getSquareName(square) + " to " + getSquareName(to) + ".");
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
    Bitboard own_pieces = white_to_move ? board.white_pieces : board.black_pieces;

    // for each queen add each attacked square to moves
    while (queens)
    {
        Position square = clearRightmostSetBit(queens);
        int rook_magic_index = ((rook_blockers[square] & board.occupied) * ROOK_MAGICS[square]) >> (NUM_SQUARES - ROOK_RELEVANT_SQUARES[square]);
        int bishop_magic_index = ((bishop_blockers[square] & board.occupied) * BISHOP_MAGICS[square]) >> (NUM_SQUARES - BISHOP_RELEVANT_SQUARES[square]);
        Bitboard attacks = rook_attacks[square][rook_magic_index] | bishop_attacks[square][bishop_magic_index];
        attacks &= ~own_pieces;
        board.squares_under_attack |= attacks;
        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.push_back(Move{square, to, piece});
            log(QUEEN_MOVE, "Found queen move from " + getSquareName(square) + " to " + getSquareName(to) + ".");
        }
    }

    return moves;
}
