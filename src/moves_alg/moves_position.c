//
// Created by Jan Tlaga on 31/08/2024.
//

#include "../../include/chess/moves_position.h"
Position create_start_position(){
    Position pos;
    pos.white_pawns = 0x000000000000FF00;
    pos.white_knights = 0x0000000000000042;
    pos.white_bishops = 0x0000000000000024;
    pos.white_rooks = 0x0000000000000081;
    pos.white_queens = 0x0000000000000010;
    pos.white_king = 0x0000000000000008;

    pos.black_pawns = 0x00FF000000000000;
    pos.black_knights = 0x4200000000000000;
    pos.black_bishops = 0x2400000000000000;
    pos.black_rooks = 0x8100000000000000;
    pos.black_queens = 0x1000000000000000;
    pos.black_king = 0x0800000000000000;
    return pos;
}