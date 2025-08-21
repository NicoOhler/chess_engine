#include "Engine.h"

GameState Engine::getGameState(MoveList moves)
{
    if (!moves.empty() && board.half_move_clock < HALF_MOVE_CLOCK_LIMIT)
        return IN_PROGRESS;
    Position king_square = getRightmostSetBit(board.white_to_move ? board.white_king : board.black_king);
    return move_generator.squareUnderAttack(board, king_square, board.white_to_move) ? CHECKMATE : DRAW;
}

uint64 Engine::perft(int depth, bool divide)
{
    MoveList legal_moves = move_generator.generateLegalMoves(board);
    if (depth == 1)
        return legal_moves.size;

    uint64 total_nodes = 0;
    for (int i = 0; i < legal_moves.size; i++)
    {
        Move move = legal_moves.moves[i];
        move_generator.makeMove(board, move);
        uint64 nodes = perft(depth - 1);
        move_generator.unmakeMove(board, move);
        if (divide)
            log(PERFT, getSquareName(move.from) + getSquareName(move.to) + ": " + std::to_string(nodes));
        total_nodes += nodes;
    }
    return total_nodes;
}

// iterative deepening
Move Engine::search(int max_depth)
{
    evaluated_nodes = 0;
    best_score = 0;
    best_move = NULL_MOVE; // no previous best move for depth 1

    timer.start();
    for (int depth = 1; depth <= max_depth && timer.timeLeft(); depth++)
    {
        best_score = negamax_search(depth, NEG_INFINITY, POS_INFINITY, true);
        log(SEARCH, "Depth " + std::to_string(depth) + " best move " + best_move.toString() +
                        " score " + std::to_string(best_score));
    }
    if (timer.timeLeft())
        timer.stop(SEARCH);
    else
        log(SEARCH, "Search interrupted due to time limit.");

    log(SEARCH, "Search score: " + std::to_string(best_score));
    log(SEARCH, "Evaluated nodes: " + std::to_string(evaluated_nodes));
    return best_move;
}

void Engine::calculateMoveScores(MoveList &moves, Move best_move)
{
    for (int i = 0; i < moves.size; i++)
    {
        Move move = moves.moves[i];
        // search previously best move first
        if (move == best_move)
            moves.scores[i] = POS_INFINITY;

        // Most Valuable Victim – Least Valuable Aggressor
        // i.e., prioritize captures of high value with low value pieces
        else if (move.captured_piece != EMPTY)
            moves.scores[i] = getPieceValue(move.captured_piece) - getPieceValue(move.piece) + 10000;

        // promotions
        else if (move.promotion != EMPTY)
            moves.scores[i] = getPieceValue(move.promotion) + 10000;

        // todo add killer moves, history later on
        else
            moves.scores[i] = 0;
    }

    if (!ENABLE_MOVE_SORTING)
        return;

    // sort moves by score
    // insertion sort seems best for small arrays
    for (int i = 1; i < moves.size; i++)
    {
        Move key_move = moves.moves[i];
        Score key_score = moves.scores[i];
        int j = i - 1;

        while (j >= 0 && moves.scores[j] < key_score)
        {
            moves.moves[j + 1] = moves.moves[j];
            moves.scores[j + 1] = moves.scores[j];
            j--;
        }
        moves.moves[j + 1] = key_move;
        moves.scores[j + 1] = key_score;
    }
}

Move Engine::pickBestMove(MoveList &moves)
{
    int best_move_index = -1;
    Score best_score = NEG_INFINITY;

    for (int i = 0; i < moves.size; i++)
        if (moves.scores[i] > best_score)
        {
            best_score = moves.scores[i];
            best_move_index = i;
        }

    // mark selected move as used => ensure it is not picked again
    if (best_move_index != -1)
        moves.scores[best_move_index] = NEG_INFINITY;

    return moves.moves[best_move_index];
}

// negamax with alpha beta pruning and quiescence
Score Engine::negamax_search(int depth, Score alpha, Score beta, bool root)
{
    // immediately return evaluation for leaf nodes (max depth, draw, stalemate or checkmate)
    if (depth == 0)
        return ENABLE_QUIESCENCE ? quiescence(alpha, beta) : evaluateBoard();

    MoveList legal_moves = move_generator.generateLegalMoves(board);
    GameState game_state = getGameState(legal_moves);
    // add depth to prefer fast games
    if (game_state == CHECKMATE)
        return NEG_INFINITY + depth;
    // treat draws as slightly negative
    if (game_state == DRAW)
        return DRAW_VALUE + depth;

    // evaluate moves until pruning possible
    calculateMoveScores(legal_moves, best_move);
    Score max = NEG_INFINITY;
    for (int i = 0; i < legal_moves.size; i++)
    {
        Move move = ENABLE_MOVE_SORTING ? legal_moves.moves[i] : pickBestMove(legal_moves);
        move_generator.makeMove(board, move);
        // alpha beta are swapped since the opponent tries to minimize our score
        Score score = -negamax_search(depth - 1, -beta, -alpha);
        move_generator.unmakeMove(board, move);
        if (score > max)
        {
            max = score;
            if (score > alpha)
            {
                alpha = score;
                if (root)
                    best_move = move;
            }
        }
        // prune since score of current subtree is already worse than beta
        // beta = value of the best move found so far by the parent
        if (beta <= alpha)
            break;

        // interrupt search if time is up
        if (!timer.timeLeft())
            break;
    }
    return max;
}

Score Engine::quiescence(Score alpha, Score beta, int max_depth)
{
    // use static evaluation as baseline
    // in case no further captures are possible
    Score stand_pat = evaluateBoard();
    if (stand_pat >= beta)
        return beta;
    if (stand_pat > alpha)
        alpha = stand_pat;

    // return if max depth exceeded or if current position is really bad (delta pruning)
    if (max_depth <= 0 || stand_pat + DELTA < alpha)
        return stand_pat;

    // further examine captures, checks and promotions
    // ? maybe create generateInterestingMoves(board) (checks, captures, promotions)
    MoveList legal_moves = move_generator.generateLegalMoves(board, true);
    calculateMoveScores(legal_moves);
    for (int i = 0; i < legal_moves.size; i++)
    {
        Move move = pickBestMove(legal_moves);
        // todo also consider checks
        if (move.captured_piece == EMPTY && move.promotion == EMPTY)
            continue;

        move_generator.makeMove(board, move);
        Score score = -quiescence(-beta, -alpha, max_depth - 1);
        move_generator.unmakeMove(board, move);

        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;

        if (!timer.timeLeft())
            return alpha;
    }
    return alpha;
}

// todo replace with actual evaluation function
Score Engine::evaluateBoard()
{
    evaluated_nodes++;
    Score material = 0;
    material += countSetBits(board.white_pawns) * PAWN_VALUE;
    material += countSetBits(board.white_knights) * KNIGHT_VALUE;
    material += countSetBits(board.white_bishops) * BISHOP_VALUE;
    material += countSetBits(board.white_rooks) * ROOK_VALUE;
    material += countSetBits(board.white_queens) * QUEEN_VALUE;
    material += countSetBits(board.white_king) * KING_VALUE;

    material -= countSetBits(board.black_pawns) * PAWN_VALUE;
    material -= countSetBits(board.black_knights) * KNIGHT_VALUE;
    material -= countSetBits(board.black_bishops) * BISHOP_VALUE;
    material -= countSetBits(board.black_rooks) * ROOK_VALUE;
    material -= countSetBits(board.black_queens) * QUEEN_VALUE;
    material -= countSetBits(board.black_king) * KING_VALUE;

    Score score = material;
    // return score from perspective of current player
    if (!board.white_to_move)
        score = -score;
    return score;
}

void Engine::initializeStartPosition(std::string fen)
{
    board = generateBoardFromFEN(fen);
    log(CHESS_BOARD, "Initialized board with FEN: " + fen);
}

void Engine::makeMove(Move move)
{
    move_generator.makeMove(board, move);
}

void Engine::unmakeMove(Move move)
{
    move_generator.unmakeMove(board, move);
}

// getter and setter
void Engine::setTimeLimit(Milliseconds time_limit)
{
    timer.limit = time_limit;
}

Board Engine::getBoard()
{
    return board; // copy
}

MoveList Engine::getLegalMoves()
{
    return move_generator.generateLegalMoves(board);
}