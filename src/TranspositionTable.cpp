#include "TranspositionTable.h"

TranspositionTable::TranspositionTable(int megabytes)
{
    table_size = megabytes * 1024 * 1024 / sizeof(TranspositionEntry);
    table = new TranspositionEntry[table_size];
}

// todo handle indexing and collisions
TranspositionEntry *TranspositionTable::probe(Hash hash)
{
    TranspositionEntry &entry = table[hash % table_size];
    if (entry.hash != hash)
        return nullptr;
    return &entry;
}

void TranspositionTable::store(Hash hash, Score score, Move best_move, int depth, BoundType type)
{
    // ? current replacement strategy: always overwrite
    TranspositionEntry &entry = table[hash % table_size];
    entry.hash = hash;
    entry.score = score;
    entry.best_move = best_move;
    entry.depth = depth;
    entry.type = type;
}
