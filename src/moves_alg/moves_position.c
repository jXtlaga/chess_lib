//
// Created by Jan Tlaga on 31/08/2024.
//

#include "../../include/chess/moves_alg/moves_position.h"
Position create_start_position(){
    Position pos;
    pos.white_pieces.pawn = 0x000000000000FF00;
    pos.white_pieces.knight = 0x0000000000000042;
    pos.white_pieces.bishop = 0x0000000000000024;
    pos.white_pieces.rook = 0x0000000000000081;
    pos.white_pieces.queen = 0x0000000000000010;
    pos.white_pieces.king = 0x0000000000000008;

    pos.black_pieces.pawn = 0x00FF000000000000;
    pos.black_pieces.knight = 0x4200000000000000;
    pos.black_pieces.bishop = 0x2400000000000000;
    pos.black_pieces.rook = 0x8100000000000000;
    pos.black_pieces.queen = 0x1000000000000000;
    pos.black_pieces.king = 0x0800000000000000;

    pos.move_number = 0;
    pos.half_move_clock = 0;

    pos.en_passant_white_sq = 0;
    pos.en_passant_black_sq = 0;
    return pos;
}