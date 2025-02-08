//
// Created by Jan Tlaga on 29/08/2024.
//

#ifndef CHESS_C_LIB_SQUARES_ALGORITHMS_H
#define CHESS_C_LIB_SQUARES_ALGORITHMS_H
#include "../define_types.h"
int get_distance_right(int sq);
int get_distance_down(int sq);
U64 flip_board_diag_A8_H1(U64 x);
void flip_vertical_position_half(Pieces_position *pos);
U64 rotate_board_90_clockwise(U64 x);
U64 get_occ(Pieces_position *position);
#endif //CHESS_C_LIB_SQUARES_ALGORITHMS_H
