#pragma once
#include "definitions.h"
#include "MoveGenerator.h"
#include "ZobristHash.h"
#include "BitBoard.h"

using namespace BitBoard;

typedef enum
{
    EXACT,
    LOWER_BOUND,
    UPPER_BOUND
} BoundType;

struct TranspositionEntry
{
    Hash hash;
    Score score;
    Move best_move;
    int depth;
    BoundType type;
    // ? alpha beta
};

class TranspositionTable
{
public:
    TranspositionTable(int megabytes = TRANSPOSITION_TABLE_SIZE);
    TranspositionEntry *probe(Hash hash);
    void store(Hash hash, Score score, Move best_move, int depth, BoundType type);

private:
    uint64 table_size;
    TranspositionEntry *table;
};
