#include "Game.h"

void Game::loop()
{
    do
    {
        printBoard();
        std::vector<Move> moves = moveGenerator.generateMoves(board);
        moves = removeIllegalMoves(moves, board);
        Move move = getLegalMoveFromUser(moves);
        applyMove(board, move);
    } while (!isGameOver());

    printBoard();
    std::cout << "GAME OVER" << std::endl;
}

void Game::printBoard()
{
    placePiecesOnBoard();
    std::cout << (white_to_move ? "White" : "Black") << "'s turn" << std::endl
              << std::endl;
    std::cout << "  a b c d e f g h" << std::endl;
    for (char i = 0; i < 8; i++)
    {
        std::cout << 8 - i << " ";
        for (char j = 0; j < 8; j++)
            std::cout << board_to_print[i][j] << " ";
        std::cout << 8 - i << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl
              << std::endl;
}

void Game::placePiecesOnBoard()
{
    for (char i = 0; i < 8; i++)
        for (char j = 0; j < 8; j++)
            board_to_print[i][j] = ' ';

    placePieceTypeOnBoard(board.white_pawns, WHITE_PAWN);
    placePieceTypeOnBoard(board.white_rooks, WHITE_ROOK);
    placePieceTypeOnBoard(board.white_knights, WHITE_KNIGHT);
    placePieceTypeOnBoard(board.white_bishops, WHITE_BISHOP);
    placePieceTypeOnBoard(board.white_queens, WHITE_QUEEN);
    placePieceTypeOnBoard(board.white_king, WHITE_KING);
    placePieceTypeOnBoard(board.black_pawns, BLACK_PAWN);
    placePieceTypeOnBoard(board.black_rooks, BLACK_ROOK);
    placePieceTypeOnBoard(board.black_knights, BLACK_KNIGHT);
    placePieceTypeOnBoard(board.black_bishops, BLACK_BISHOP);
    placePieceTypeOnBoard(board.black_queens, BLACK_QUEEN);
    placePieceTypeOnBoard(board.black_king, BLACK_KING);
}

void Game::placePieceTypeOnBoard(Bitboard piece_type, char piece)
{
    while (piece_type)
    {
        Position position = BitBoard::clearRightmostSetBit(piece_type);
        board_to_print[7 - position / 8][position % 8] = piece;
    }
}

Move Game::getLegalMoveFromUser(std::vector<Move> legal_moves)
{
    std::string input;
    while (true)
    {
        std::cout << "Enter your move: ";
        std::getline(std::cin, input);
        if (input[0] < 'a' || input[0] > 'h' || input[1] < '1' || input[1] > '8' || input[3] < 'a' || input[3] > 'h' || input[4] < '1' || input[4] > '8')
        {
            std::cout << "Invalid move. Please enter a move in the format: 'a2 c3'" << std::endl;
            continue;
        }

        Position from = 8 * (input[1] - '1') + (input[0] - 'a');
        Position to = 8 * (input[4] - '1') + (input[3] - 'a');
        for (Move legal_move : legal_moves)
            if (legal_move.from == from && legal_move.to == to)
                return legal_move;
        std::cout << "Illegal move. Please enter a legal move." << std::endl;
    }
}

void Game::applyMove(Board &board, Move move)
{
    char captured_piece_type = board_to_print[7 - move.to / 8][move.to % 8];
    if (captured_piece_type != EMPTY)
    {
        Bitboard *opponent_pieces = white_to_move ? &board.black_pieces : &board.white_pieces;
        Bitboard *captured_piece = board.getBitboardByPiece(captured_piece_type);
        BitBoard::clear(*captured_piece, move.to);
        BitBoard::clear(*opponent_pieces, move.to);
    }
    else if (white_to_move ? move.piece == WHITE_PAWN : move.piece == BLACK_PAWN)
    {
        applyEnPassantCapture(board, move);
        detectDoublePawnPushForEnPassant(board, move);
    }

    // move piece <=> update moved piece, own pieces and occupied squares
    Bitboard *piece = board.getBitboardByPiece(move.piece);
    BitBoard::movePiece(*piece, move.from, move.to);
    BitBoard::movePiece(*(white_to_move ? &board.white_pieces : &board.black_pieces), move.from, move.to);
    BitBoard::movePiece(board.occupied, move.from, move.to);

    applyPromotion(board, move);
    applyCastling(board, move);
    updateCastlingRights(board, move);
    white_to_move = !white_to_move;
}

Piece Game::getPromotionChoice()
{
    std::cout << "Choose promotion piece: " << std::endl;
    std::cout << "Q - Queen" << std::endl;
    std::cout << "R - Rook" << std::endl;
    std::cout << "B - Bishop" << std::endl;
    std::cout << "N - Knight" << std::endl;

    Piece choice;
    while (true)
    {
        std::cin >> choice;
        if (choice == 'Q' || choice == 'R' || choice == 'B' || choice == 'N' || choice == 'q' || choice == 'r' || choice == 'b' || choice == 'n')
            return white_to_move ? toupper(choice) : tolower(choice);
        std::cout << "Invalid choice. Please enter Q, R, B or N." << std::endl;
    }
}

void Game::applyPromotion(Board &board, Move &move)
{
    if (move.promotion)
    {
        Piece promotion = getPromotionChoice();
        BitBoard::clear(*board.getBitboardByPiece(move.piece), move.to);
        BitBoard::set(*board.getBitboardByPiece(promotion), move.to);
    }
}

void Game::applyCastling(Board &board, Move &move)
{
    Bitboard king_side = white_to_move ? WHITE_KING_SIDE_CASTLING : BLACK_KING_SIDE_CASTLING;
    Bitboard queen_side = white_to_move ? WHITE_QUEEN_SIDE_CASTLING : BLACK_QUEEN_SIDE_CASTLING;
    Bitboard *rook = white_to_move ? &board.white_rooks : &board.black_rooks;

    if (move.castling & queen_side)
    {
        board.castling_rights &= ~queen_side & ~king_side; // clear castling rights
        Position rook_from = move.from + 4 * ONE_COL_RIGHT;
        Position rook_to = move.from + ONE_COL_RIGHT;
        BitBoard::movePiece(*rook, rook_from, rook_to);
        return;
    }

    if (move.castling & king_side)
    {
        board.castling_rights &= ~queen_side & ~king_side; // clear castling rights
        Position rook_from = move.from + 3 * ONE_COL_LEFT;
        Position rook_to = move.from + ONE_COL_LEFT;
        BitBoard::movePiece(*rook, rook_from, rook_to);
    }
}

void Game::updateCastlingRights(Board &board, Move &move)
{
    Piece king = white_to_move ? WHITE_KING : BLACK_KING;
    Piece rook = white_to_move ? WHITE_ROOK : BLACK_ROOK;
    Bitboard castling_rights = white_to_move ? WHITE_CASTLING : BLACK_CASTLING;

    if (move.piece == king)
        board.castling_rights &= ~castling_rights;
    else if (move.piece == rook)
    {
        Position king_side_rook_start_position = white_to_move ? WHITE_KING_SIDE_ROOK_START_POSITION : BLACK_KING_SIDE_ROOK_START_POSITION;
        Position queen_side_rook_start_position = white_to_move ? WHITE_QUEEN_SIDE_ROOK_START_POSITION : BLACK_QUEEN_SIDE_ROOK_START_POSITION;
        if (move.from == king_side_rook_start_position)
            board.castling_rights &= ~WHITE_KING_SIDE_CASTLING;
        else if (move.from == queen_side_rook_start_position)
            board.castling_rights &= ~WHITE_QUEEN_SIDE_CASTLING;
    }
}

void Game::applyEnPassantCapture(Board &board, Move &move)
{
    if (move.to == board.en_passant + (white_to_move ? ONE_ROW_UP : ONE_ROW_DOWN))
    {
        // remove captured pawn
        Bitboard *opponent_pieces = white_to_move ? &board.black_pieces : &board.white_pieces;
        Bitboard *opponent_pawns = white_to_move ? &board.black_pawns : &board.white_pawns;
        BitBoard::clear(*opponent_pawns, board.en_passant);
        BitBoard::clear(*opponent_pieces, board.en_passant);
    }
}

void Game::detectDoublePawnPushForEnPassant(Board &board, Move &move)
{
    Position from_row = move.from / 8;
    Position to_row = move.to / 8;
    if (white_to_move && from_row == ROW_2 && to_row == ROW_4)
        board.en_passant = move.to;
    else if (!white_to_move && from_row == ROW_7 && to_row == ROW_5)
        board.en_passant = move.to;
    else
        board.en_passant = NO_EN_PASSANT;
}

std::vector<Move> Game::removeIllegalMoves(std::vector<Move> moves, Board board)
{
    // simulate each move and check if king is in check
    std::vector<Move> legal_moves;
    Bitboard king = white_to_move ? board.white_king : board.black_king;
    for (Move move : moves)
    {
        Board board_copy = board;
        applyMove(board_copy, move);
        if (moveGenerator.areSquaresUnderAttack(board_copy, king))
            log(REMOVE_ILLEGAL_MOVES, "Removed illegal move from " + getSquareName(move.from) + " to " + getSquareName(move.to) + ".");
        else
            legal_moves.push_back(move);
    }
    return legal_moves;
}

bool Game::isGameOver()
{
    // todo
    return false;
}

int main()
{
    Game game;
    game.loop();
    return 0;
}