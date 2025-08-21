#pragma once
#include "definitions.h"
#include "MoveGenerator.h"
#include "BitBoard.h"
#include <random>

using namespace BitBoard;

typedef uint64 Hash;
const uint64 RAND_SEED = 42; // for reproducibility

class ZobristHash
{
public:
    ZobristHash() { initialize(); }

    uint64 computeInitialHash(Board &board);
    uint64 updateHash(uint64 hash, Move move, Board &board);

private:
    uint64 whites_turn;
    uint64 piece_at_square[64][12];
    uint64 castling_rights[16];
    uint64 en_passant_file[8];

    void initialize();
};
