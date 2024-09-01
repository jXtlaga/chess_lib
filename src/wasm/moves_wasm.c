//
// Created by Jan Tlaga on 31/08/2024.
//
#include "../../include/chess/define_types.h"
#include "../../include/chess/moves/rook_attack.h"
#include "../../include/chess/moves/bishop_attack.h"
#include "../../include/chess/moves/knight_attack.h"
#include "../../include/chess/moves/pawn_attack.h"
#include "../../include/chess/moves/king_attack.h"
#include "../../include/chess/moves/moves_pieces.h"

U64 combineU32(U32 low, U32 high){
    return ((U64) high << 32) | low;
}
U32 rookMovesLow(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh,  U32 enemyBoardLow, U32 enemyBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    U64 enemyBoard = combineU32(enemyBoardLow, enemyBoardHigh);
    return get_rook_attack(pieceBoard | enemyBoard, sq) & ~pieceBoard;
}
U32 rookMovesHigh(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh,  U32 enemyBoardLow, U32 enemyBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    U64 enemyBoard = combineU32(enemyBoardLow, enemyBoardHigh);
    return (get_rook_attack(pieceBoard | enemyBoard, sq) & ~pieceBoard) >> 32;
}
U32 bishopMovesLow(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh,  U32 enemyBoardLow, U32 enemyBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    U64 enemyBoard = combineU32(enemyBoardLow, enemyBoardHigh);
    return get_bishop_attack(pieceBoard | enemyBoard, sq) & ~pieceBoard;
}
U32 bishopMovesHigh(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh,  U32 enemyBoardLow, U32 enemyBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    U64 enemyBoard = combineU32(enemyBoardLow, enemyBoardHigh);
    return (get_bishop_attack(pieceBoard | enemyBoard, sq) & ~pieceBoard) >> 32;
}
U32 queenMovesLow(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh,  U32 enemyBoardLow, U32 enemyBoardHigh){
    return rookMovesLow(sq, pieceBoardLow, pieceBoardHigh, enemyBoardLow, enemyBoardHigh) |
           bishopMovesLow(sq, pieceBoardLow, pieceBoardHigh, enemyBoardLow, enemyBoardHigh);
}
U32 queenMovesHigh(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh,  U32 enemyBoardLow, U32 enemyBoardHigh){
    return rookMovesHigh(sq, pieceBoardLow, pieceBoardHigh, enemyBoardLow, enemyBoardHigh) |
           bishopMovesHigh(sq, pieceBoardLow, pieceBoardHigh, enemyBoardLow, enemyBoardHigh);
}
U32 knightMovesLow(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    return get_knight_attack(sq) & ~pieceBoard;
}
U32 knightMovesHigh(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    return (get_knight_attack(sq) & ~pieceBoard) >> 32;
}
U32 kingMovesLow(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    return get_king_attack(sq) & ~pieceBoard;
}
U32 kingMovesHigh(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    return (get_king_attack(sq) & ~pieceBoard) >> 32;
}
U64 whitePawnMovesLowStart(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh,  U32 enemyBoardLow, U32 enemyBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    U64 enemyBoard = combineU32(enemyBoardLow, enemyBoardHigh);
    U64 move = get_rook_attack(get_white_pawn_move(sq) & ~pieceBoard, sq) & ~pieceBoard;
    U64 attack = get_white_pawn_attack(sq) & ~pieceBoard;
    return move | attack;
}
U64 whitePawnMovesHighStart(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh,  U32 enemyBoardLow, U32 enemyBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    U64 enemyBoard = combineU32(enemyBoardLow, enemyBoardHigh);
    U64 move = get_rook_attack(get_white_pawn_move(sq) & ~pieceBoard, sq) & ~pieceBoard;
    U64 attack = get_white_pawn_attack(sq) & ~pieceBoard;
    return (move | attack) >> 32;
}
U64 blackPawnMovesLowStart(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh,  U32 enemyBoardLow, U32 enemyBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    U64 enemyBoard = combineU32(enemyBoardLow, enemyBoardHigh);
    U64 move = get_rook_attack(get_black_pawn_move(sq) & ~pieceBoard, sq) & ~pieceBoard;
    U64 attack = get_black_pawn_attack(sq) & ~pieceBoard;
    return move | attack;
}
U64 blackPawnMovesHighStart(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh,  U32 enemyBoardLow, U32 enemyBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    U64 enemyBoard = combineU32(enemyBoardLow, enemyBoardHigh);
    U64 move = get_rook_attack(get_black_pawn_move(sq) & ~pieceBoard, sq) & ~pieceBoard;
    U64 attack = get_black_pawn_attack(sq) & ~pieceBoard;
    return (move | attack) >> 32;
}
U64 whitePawnMovesLow(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh,  U32 enemyBoardLow, U32 enemyBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    U64 enemyBoard = combineU32(enemyBoardLow, enemyBoardHigh);
    U64 move = get_white_pawn_attack(sq) & ~pieceBoard;
    U64 attack = get_white_pawn_attack(sq) & ~pieceBoard;
    return move | attack;
}
U64 whitePawnMovesHigh(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh,  U32 enemyBoardLow, U32 enemyBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    U64 enemyBoard = combineU32(enemyBoardLow, enemyBoardHigh);
    U64 move = get_white_pawn_attack(sq) & ~pieceBoard;
    U64 attack = get_white_pawn_attack(sq) & ~pieceBoard;
    return (move | attack) >> 32;
}
U64 blackPawnMovesLow(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh,  U32 enemyBoardLow, U32 enemyBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    U64 enemyBoard = combineU32(enemyBoardLow, enemyBoardHigh);
    U64 move = get_black_pawn_attack(sq) & ~pieceBoard;
    U64 attack = get_black_pawn_attack(sq) & ~pieceBoard;
    return move | attack;
}
U64 blackPawnMovesHigh(unsigned sq, U32 pieceBoardLow, U32 pieceBoardHigh,  U32 enemyBoardLow, U32 enemyBoardHigh){
    U64 pieceBoard = combineU32(pieceBoardLow, pieceBoardHigh);
    U64 enemyBoard = combineU32(enemyBoardLow, enemyBoardHigh);
    U64 move = get_black_pawn_attack(sq) & ~pieceBoard;
    U64 attack = get_black_pawn_attack(sq) & ~pieceBoard;
    return (move | attack) >> 32;
}