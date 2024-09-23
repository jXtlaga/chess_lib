//
// Created by Jan Tlaga on 27/08/2024.
//

#ifndef CHESS_C_LIB_BOARD_VISUALISATION_H
#define CHESS_C_LIB_BOARD_VISUALISATION_H
#include <stdio.h>
#include "define_types.h"

void print_bitboard(uint64_t n);
void print_type(TYPE_PIECE type);
void print_type_move(TYPE_MOVE type);

#endif //CHESS_C_LIB_BOARD_VISUALISATION_H
