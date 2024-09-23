//
// Created by Jan Tlaga on 31/08/2024.
//

#ifndef CHESS_C_LIB_CONSOLE_VISUALIZATION_H
#define CHESS_C_LIB_CONSOLE_VISUALIZATION_H
#include "define_types.h"

void print_position(Position *position, U64 attack);
void print_positions(Position *positions, U64 *attacks, int num_positions);
void print_position_with_info(Position *position);
#endif //CHESS_C_LIB_CONSOLE_VISUALIZATION_H
