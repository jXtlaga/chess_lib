//
// Created by Jan Tlaga on 05/09/2024.
//

#ifndef CHESS_C_LIB_PIN_MASK_H
#define CHESS_C_LIB_PIN_MASK_H
#include "../define_types.h"

void get_pin_rook(int king_sq, U64 current_color_occ, U64 enemy_color_occ, U64 rook_queen_occ, U64 *pin_rook);
void get_pin_bishop(int king_sq, U64 current_color_occ, U64 enemy_color_occ, U64 bishop_queen_occ, U64 *pin_bishop);

#endif //CHESS_C_LIB_PIN_MASK_H
