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

void MoveGenerator::addPawnMoveWithPossiblePromotion(Board &board, MoveList &moves, Move move)
{
    // regular move
    if (move.to >= ONE_ROW_UP && move.to < NUM_SQUARES + ONE_ROW_DOWN)
    {
        moves.append(move);
        log(PAWN_MOVE, "Found pawn move from " + getSquareName(move.from) + " to " + getSquareName(move.to) + ".");
        return;
    }

    // pawn promotion
    bool white_to_move = board.white_to_move;
    move.promotion = white_to_move ? WHITE_QUEEN : BLACK_QUEEN;
    moves.append(move);
    move.promotion = white_to_move ? WHITE_ROOK : BLACK_ROOK;
    moves.append(move);
    move.promotion = white_to_move ? WHITE_BISHOP : BLACK_BISHOP;
    moves.append(move);
    move.promotion = white_to_move ? WHITE_KNIGHT : BLACK_KNIGHT;
    moves.append(move);
    log(PAWN_MOVE, "Found pawn promotion from " + getSquareName(move.from) + " to " + getSquareName(move.to) + ".");
}

void MoveGenerator::addCastlingMoves(Board &board, MoveList &moves)
{
    Piece piece = board.white_to_move ? WHITE_KING : BLACK_KING;
    Position king_start = board.white_to_move ? WHITE_KING_START_POSITION : BLACK_KING_START_POSITION;
    Bitboard king_side_castling = board.white_to_move ? WHITE_KING_SIDE_CASTLING : BLACK_KING_SIDE_CASTLING;
    if (board.castling_rights & king_side_castling) // not possible if king or rook has moved
    {
        Bitboard king_side_blocked = board.occupied & king_side_castling;             // squares between king and rook must be empty
        bool king_side_attacked = squaresThreatened(board, king_side_castling, true); // squares in between must not be attacked
        if (!king_side_blocked && !king_side_attacked)
        {
            Position to = king_start + ONE_COL_RIGHT * 2;
            moves.append(Move{king_start, to, piece, 0, king_side_castling});
            log(KING_MOVE, "Found king side castling from " + getSquareName(king_start) + " to " + getSquareName(to) + ".");
        }
    }

    Bitboard queen_side_castling = board.white_to_move ? WHITE_QUEEN_SIDE_CASTLING : BLACK_QUEEN_SIDE_CASTLING;
    if (board.castling_rights & queen_side_castling)
    {
        Bitboard queen_side_blocked = board.occupied & queen_side_castling;
        bool queen_side_attacked = squaresThreatened(board, queen_side_castling, true);
        if (!queen_side_blocked && !queen_side_attacked)
        {
            Position to = king_start + ONE_COL_LEFT * 2;
            moves.append(Move{king_start, to, piece, 0, queen_side_castling});
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

MoveList MoveGenerator::generateLegalMoves(Board board)
{
    // todo optimize, https://www.youtube.com/watch?v=U4ogK0MIzqk&t=435s, 8:00
    MoveList legal_moves;
    MoveList pseudo_legal_moves = generatePseudoLegalMoves(board);

    // simulate each move and ensure king (of player making a move) is not in check
    for (int i = 0; i < pseudo_legal_moves.size; i++)
    {
        Board board_copy = board;
        Move move = pseudo_legal_moves.moves[i];
        applyMove(board_copy, move);
        Bitboard king = board.white_to_move ? board.white_king : board.black_king;

        if (squaresThreatened(board_copy, king, false))
            log(REMOVE_ILLEGAL_MOVES, "Removed illegal move from " + getSquareName(move.from) + " to " + getSquareName(move.to) + ".");
        else
        {
            legal_moves.append(move);
            log(REMAINING_MOVES, "Found legal move from " + getSquareName(move.from) + " to " + getSquareName(move.to) + ".");
        }
    }

    return legal_moves;
}

MoveList MoveGenerator::generatePseudoLegalMoves(Board &board)
{
    MoveList moves;
    generateKingMoves(board, moves);
    generatePawnMoves(board, moves);
    generateKnightMoves(board, moves);
    generateBishopMoves(board, moves);
    generateRookMoves(board, moves);
    generateQueenMoves(board, moves);
    return moves;
}

void MoveGenerator::generateKingMoves(Board &board, MoveList &moves)
{
    bool white_to_move = board.white_to_move;
    Piece piece = white_to_move ? WHITE_KING : BLACK_KING;
    Bitboard king = white_to_move ? board.white_king : board.black_king;
    Bitboard own_pieces = white_to_move ? board.white_pieces : board.black_pieces;
    Position from = clearRightmostSetBit(king);
    Bitboard attacks = king_moves[from] & ~own_pieces;

    while (attacks)
    {
        Position to = clearRightmostSetBit(attacks);
        if (squaresThreatened(board, 1ULL << to, true))
            continue;
        moves.append(Move{from, to, piece});
        log(KING_MOVE, "Found king move from " + getSquareName(from) + " to " + getSquareName(to) + ".");
    }

    if (!squaresThreatened(board, king, true))
        addCastlingMoves(board, moves);
}

void MoveGenerator::generatePawnMoves(Board &board, MoveList &moves)
{
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

    // single move
    // right shift for white, left shift for black since negative shift is undefined
    Bitboard single_moves = white_to_move ? (pawns << ONE_ROW_UP) : (pawns >> ONE_ROW_UP);
    single_moves &= ~board.occupied;
    while (single_moves)
    {
        Position to = clearRightmostSetBit(single_moves);
        Position from = to - direction;
        addPawnMoveWithPossiblePromotion(board, moves, Move{from, to, piece});
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
        moves.append(Move{from, to, piece});
        log(PAWN_MOVE, "Found pawn move from " + getSquareName(from) + " to " + getSquareName(to) + ".");
    }

    // for each pawn check attack masks
    while (pawns)
    {
        Position from = clearRightmostSetBit(pawns);

        // attack right
        Position to_right = from + direction_right;
        Bitboard attack = attack_right[from] & enemies;
        if (attack)
            addPawnMoveWithPossiblePromotion(board, moves, Move{from, to_right, piece});

        // attack left
        Position to_left = from + direction_left;
        attack = attack_left[from] & enemies;
        if (attack)
            addPawnMoveWithPossiblePromotion(board, moves, Move{from, to_left, piece});

        // right en passant
        if (from % 8 != COL_H && board.en_passant == from + ONE_COL_RIGHT)
        {
            moves.append(Move{from, to_right, piece});
            log(PAWN_MOVE, "Found en passant move from " + getSquareName(from) + " to " + getSquareName(to_right) + ".");
        }

        // left en passant
        if (from % 8 != COL_A && board.en_passant == from + ONE_COL_LEFT)
        {
            moves.append(Move{from, to_left, piece});
            log(PAWN_MOVE, "Found en passant move from " + getSquareName(from) + " to " + getSquareName(to_left) + ".");
        }
    }
}

// todo duplicate code
void MoveGenerator::generateKnightMoves(Board &board, MoveList &moves)
{
    bool white_to_move = board.white_to_move;
    Piece piece = white_to_move ? WHITE_KNIGHT : BLACK_KNIGHT;
    Bitboard knights = white_to_move ? board.white_knights : board.black_knights;
    Bitboard own_pieces = white_to_move ? board.white_pieces : board.black_pieces;

    while (knights)
    {
        Position square = clearRightmostSetBit(knights);
        Bitboard attacks = knight_moves[square] & ~own_pieces;
        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.append(Move{square, to, piece});
            log(KNIGHT_MOVE, "Found knight move from " + getSquareName(square) + " to " + getSquareName(to) + ".");
        }
    }
}

void MoveGenerator::generateBishopMoves(Board &board, MoveList &moves)
{
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
        // add each square attacked by current bishop to moves
        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.append(Move{square, to, piece});
            log(BISHOP_MOVE, "Found bishop move from " + getSquareName(square) + " to " + getSquareName(to) + ".");
        }
    }
}

void MoveGenerator::generateRookMoves(Board &board, MoveList &moves)
{
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
        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.append(Move{square, to, piece});
            log(ROOK_MOVE, "Found rook move from " + getSquareName(square) + " to " + getSquareName(to) + ".");
        }
    }
}

void MoveGenerator::generateQueenMoves(Board &board, MoveList &moves)
{
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
        while (attacks)
        {
            Position to = clearRightmostSetBit(attacks);
            moves.append(Move{square, to, piece});
            log(QUEEN_MOVE, "Found queen move from " + getSquareName(square) + " to " + getSquareName(to) + ".");
        }
    }
}

// todo remove duplicate code
bool MoveGenerator::squaresThreatened(Board &board, Bitboard squares, bool opponent)
{
    bool white_to_move = opponent ? !board.white_to_move : board.white_to_move;
    Bitboard pawn = white_to_move ? board.white_pawns : board.black_pawns;
    Bitboard knight = white_to_move ? board.white_knights : board.black_knights;
    Bitboard rook = white_to_move ? board.white_rooks : board.black_rooks;
    Bitboard bishop = white_to_move ? board.white_bishops : board.black_bishops;
    Bitboard queen = white_to_move ? board.white_queens : board.black_queens;
    Bitboard king = white_to_move ? board.white_king : board.black_king;

    while (queen)
    {
        Position from = BitBoard::clearRightmostSetBit(queen);
        int rook_magic_index = ((rook_blockers[from] & board.occupied) * ROOK_MAGICS[from]) >> (NUM_SQUARES - ROOK_RELEVANT_SQUARES[from]);
        int bishop_magic_index = ((bishop_blockers[from] & board.occupied) * BISHOP_MAGICS[from]) >> (NUM_SQUARES - BISHOP_RELEVANT_SQUARES[from]);
        Bitboard attacks = rook_attacks[from][rook_magic_index] | bishop_attacks[from][bishop_magic_index];
        if (attacks & squares)
            return true;
    }

    while (rook)
    {
        Position from = BitBoard::clearRightmostSetBit(rook);
        int magic_index = ((rook_blockers[from] & board.occupied) * ROOK_MAGICS[from]) >> (NUM_SQUARES - ROOK_RELEVANT_SQUARES[from]);
        Bitboard attacks = rook_attacks[from][magic_index];
        if (attacks & squares)
            return true;
    }

    while (bishop)
    {
        Position from = BitBoard::clearRightmostSetBit(bishop);
        int magic_index = ((bishop_blockers[from] & board.occupied) * BISHOP_MAGICS[from]) >> (NUM_SQUARES - BISHOP_RELEVANT_SQUARES[from]);
        Bitboard attacks = bishop_attacks[from][magic_index];
        if (attacks & squares)
            return true;
    }

    while (knight)
    {
        Position from = BitBoard::clearRightmostSetBit(knight);
        Bitboard attacks = knight_moves[from];
        if (attacks & squares)
            return true;
    }

    while (pawn)
    {
        Position from = BitBoard::clearRightmostSetBit(pawn);
        Bitboard attack_right = white_to_move ? white_pawn_attack_right[from] : black_pawn_attack_right[from];
        Bitboard attack_left = white_to_move ? white_pawn_attack_left[from] : black_pawn_attack_left[from];
        if (attack_right & squares || attack_left & squares)
            return true;
    }

    if (king)
    {
        Position from = BitBoard::clearRightmostSetBit(king);
        Bitboard attacks = king_moves[from];
        if (attacks & squares)
            return true;
    }

    return false;
}

void MoveGenerator::applyMove(Board &board, Move move)
{
    board.capturePiece(move.to);
    board.en_passant = NO_EN_PASSANT;
    if (board.white_to_move ? move.piece == WHITE_PAWN : move.piece == BLACK_PAWN)
    {
        if (move.to == board.en_passant + (board.white_to_move ? ONE_ROW_UP : ONE_ROW_DOWN))
            board.capturePiece(board.en_passant);
        detectDoublePawnPushForEnPassant(board, move);
    }

    // move piece <=> update moved piece, own pieces and occupied squares
    Bitboard *piece = board.getBitboardByPiece(move.piece);
    BitBoard::movePiece(*piece, move.from, move.to);
    BitBoard::movePiece(*(board.white_to_move ? &board.white_pieces : &board.black_pieces), move.from, move.to);
    BitBoard::movePiece(board.occupied, move.from, move.to);

    handleCastling(board, move);
    if (move.promotion)
    {
        move.promotion = board.white_to_move ? toupper(move.promotion) : tolower(move.promotion);
        BitBoard::clear(*board.getBitboardByPiece(move.piece), move.to);
        BitBoard::set(*board.getBitboardByPiece(move.promotion), move.to);
    }
    board.white_to_move = !board.white_to_move;
}

void MoveGenerator::handleCastling(Board &board, Move &move)
{
    // todo simplify constant fetching, maybe two constant classes one for white and one for black
    Bitboard king_side = board.white_to_move ? WHITE_KING_SIDE_CASTLING : BLACK_KING_SIDE_CASTLING;
    Bitboard queen_side = board.white_to_move ? WHITE_QUEEN_SIDE_CASTLING : BLACK_QUEEN_SIDE_CASTLING;
    Bitboard queen_side_rook_start_position = board.white_to_move ? WHITE_QUEEN_SIDE_ROOK_START_POSITION : BLACK_QUEEN_SIDE_ROOK_START_POSITION;
    Bitboard king_side_rook_start_position = board.white_to_move ? WHITE_KING_SIDE_ROOK_START_POSITION : BLACK_KING_SIDE_ROOK_START_POSITION;
    Bitboard *rooks = board.white_to_move ? &board.white_rooks : &board.black_rooks;

    // apply castling move
    if (move.castling & queen_side)
    {
        board.castling_rights &= ~queen_side & ~king_side; // clear castling rights
        Position rook_from = move.from + 4 * ONE_COL_RIGHT;
        Position rook_to = move.from + ONE_COL_RIGHT;
        BitBoard::movePiece(*rooks, rook_from, rook_to);
        return;
    }

    if (move.castling & king_side)
    {
        board.castling_rights &= ~queen_side & ~king_side; // clear castling rights
        Position rook_from = move.from + 3 * ONE_COL_LEFT;
        Position rook_to = move.from + ONE_COL_LEFT;
        BitBoard::movePiece(*rooks, rook_from, rook_to);
    }

    // update castling rights if king or rook moved
    bool king_moved = move.piece == (board.white_to_move ? WHITE_KING : BLACK_KING);
    bool king_side_rook_gone = !(*rooks & king_side_rook_start_position);
    bool queen_side_rook_gone = !(*rooks & queen_side_rook_start_position);

    if (king_moved)
        board.castling_rights &= ~king_side & ~queen_side;
    if (king_side_rook_gone)
        board.castling_rights &= ~king_side;
    if (queen_side_rook_gone)
        board.castling_rights &= ~queen_side;
}

void MoveGenerator::detectDoublePawnPushForEnPassant(Board &board, Move &move)
{
    Position from_row = move.from / 8;
    Position to_row = move.to / 8;
    if (board.white_to_move && from_row == ROW_2 && to_row == ROW_4)
        board.en_passant = move.to;
    else if (!board.white_to_move && from_row == ROW_7 && to_row == ROW_5)
        board.en_passant = move.to;
}
