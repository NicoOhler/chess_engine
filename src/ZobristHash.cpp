#include "ZobristHash.h"

void ZobristHash::initialize()
{
    std::mt19937 mt{RAND_SEED};
    for (int square = 0; square < 64; square++)
        for (int piece = 0; piece < 12; piece++)
            piece_at_square[square][piece] = mt();
    whites_turn = mt();
    for (int i = 0; i < 4; i++)
        castling_rights[i] = mt();
    for (int i = 0; i < 8; i++)
        en_passant_file[i] = mt();
}

uint64 ZobristHash::getHash(Board &board)
{
    uint64 hash = 0;
    // add all pieces
    for (int square = 0; square < 64; square++)
    {
        Piece piece = board.getPieceAt(square);
        if (piece != EMPTY)
            hash ^= piece_at_square[square][piece];
    }

    // indicate whose turn it is
    if (board.white_to_move)
        hash ^= whites_turn;

    // add en passant
    if (board.en_passant != NO_EN_PASSANT)
        hash ^= en_passant_file[board.en_passant % 8];

    // add castling rights
    if (board.castling_rights & WHITE_KING_SIDE_CASTLING)
        hash ^= castling_rights[0];
    if (board.castling_rights & WHITE_QUEEN_SIDE_CASTLING)
        hash ^= castling_rights[1];
    if (board.castling_rights & BLACK_KING_SIDE_CASTLING)
        hash ^= castling_rights[2];
    if (board.castling_rights & BLACK_QUEEN_SIDE_CASTLING)
        hash ^= castling_rights[3];
    std::cout << hash << std::endl;
    return hash;
}

uint64 ZobristHash::updateHash(uint64 hash, Move move, Board &board)
{
    // toggle whose turn it is
    hash ^= whites_turn;
    bool white_to_move = move.piece <= WHITE_KING;

    // remove piece from previous position
    hash ^= piece_at_square[move.from][move.piece];

    // remove previous en passant
    if (move.previous_en_passant)
    {
        hash ^= en_passant_file[move.previous_en_passant % 8];
        bool pawn_move = move.piece == white_to_move ? WHITE_PAWN : BLACK_PAWN;
        // apply en passant capture
        if (move.to == move.previous_en_passant && pawn_move)
            hash ^= piece_at_square[move.to + (white_to_move ? DOWN : UP)][white_to_move ? BLACK_PAWN : WHITE_PAWN];
    }

    // add new en passant square
    if (board.en_passant != NO_EN_PASSANT)
        hash ^= en_passant_file[board.en_passant % 8];

    // remove captured piece
    if (move.captured_piece != EMPTY)
        hash ^= piece_at_square[move.to][move.captured_piece];

    // add piece to new position
    if (move.promotion)
        hash ^= piece_at_square[move.to][move.promotion];
    else
        hash ^= piece_at_square[move.to][move.piece];

    // handle castling moves by moving the rook
    if (move.castling)
    {
        // remove old rook position and add new rook position
        bool king_side_castling = move.to > move.from;
        Piece rook = white_to_move ? WHITE_ROOK : BLACK_ROOK;
        Position old_rook_position = move.from + (king_side_castling ? 3 * RIGHT : 4 * LEFT);
        hash ^= piece_at_square[old_rook_position][rook];
        Position new_rook_position = move.from + (king_side_castling ? RIGHT : LEFT);
        hash ^= piece_at_square[new_rook_position][rook];
    }

    // remove castling rights on change
    if (move.previous_castling_rights != board.castling_rights)
    {
        if ((move.previous_castling_rights ^ board.castling_rights) & WHITE_KING_SIDE_CASTLING)
            hash ^= castling_rights[0];
        if ((move.previous_castling_rights ^ board.castling_rights) & WHITE_QUEEN_SIDE_CASTLING)
            hash ^= castling_rights[1];
        if ((move.previous_castling_rights ^ board.castling_rights) & BLACK_KING_SIDE_CASTLING)
            hash ^= castling_rights[2];
        if ((move.previous_castling_rights ^ board.castling_rights) & BLACK_QUEEN_SIDE_CASTLING)
            hash ^= castling_rights[3];
    }
    std::cout << hash << std::endl;
    return hash;
}
