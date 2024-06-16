#pragma once
#include "definitions.h"
#include "BitBoard.h"
#include "Logger.h"
#include "Magics.h"
#include "utils.h"
#include <vector>

using namespace BitBoard;

struct Move
{
    Position from;
    Position to;
    Piece piece;

    bool operator==(const Move &rhs)
    {
        return from == rhs.from && to == rhs.to && piece == rhs.piece;
    }
};

class MoveGenerator
{
private:
    // attack masks
    Bitboard white_pawn_attack_right[64];
    Bitboard white_pawn_attack_left[64];
    Bitboard black_pawn_attack_right[64];
    Bitboard black_pawn_attack_left[64];
    Bitboard knight_moves[64];
    Bitboard bishop_moves[64];
    Bitboard rook_moves[64];
    Bitboard king_moves[64];

    // precomputed attacks for sliding pieces
    Bitboard bishop_attacks[64][512];
    Bitboard rook_attacks[64][4096];

    void initializePawnCaptureMasks();
    void initializeKnightMoves();
    void initializeBishopMoves();
    void initializeRookMoves();
    void initializeKingMoves();

    std::vector<Move> generatePawnMoves(Board board);
    std::vector<Move> generateKnightMoves(Board board);
    std::vector<Move> generateBishopMoves(Board board);
    std::vector<Move> generateRookMoves(Board board);
    std::vector<Move> generateQueenMoves(Board board);
    std::vector<Move> generateKingMoves(Board board);
    // ? void generateCastlingMoves(Board board, Move moves[]);

    Bitboard getOccupancyVariation(int index, int relevant_bits, Bitboard moves); // returns index-th occupancy variation of moves
    void initializeRookBishopAttacks();                                           // precomputes squares under attack for every position for every relevant occupancy
    Bitboard precomputeSlidingRookAttacks(Position square, Bitboard occupied);
    Bitboard precomputeSlidingBishopAttacks(Position square, Bitboard occupied);

public:
    MoveGenerator();
    std::vector<Move> generateMoves(Board board);
};