//
// Created by Jan Tlaga on 9/8/24.
//

#ifndef CHESS_C_LIB_CASTLING_VALIDATION_H
#define CHESS_C_LIB_CASTLING_VALIDATION_H

#include "define_types.h"

bool is_castling_white_short_valid(U64 current_color_occ);
bool is_castling_white_long_valid(U64 current_color_occ);
bool is_castling_black_short_valid(U64 current_color_occ);
bool is_castling_black_long_valid(U64 current_color_occ);

bool is_castling_short_white_possible(U64 current_color_occ, U64 enemy_occ, U64 att_mask);
bool is_castling_long_white_possible(U64 current_color_occ, U64 enemy_occ, U64 att_mask);
bool is_castling_short_black_possible(U64 current_color_occ, U64 enemy_occ, U64 att_mask);
bool is_castling_long_black_possible(U64 current_color_occ, U64 enemy_occ, U64 att_mask);


#endif //CHESS_C_LIB_CASTLING_VALIDATION_H
