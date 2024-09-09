//
// Created by Jan Tlaga on 29/08/2024.
//

#ifndef CHESS_C_LIB_MOVES_CALCULATION_H
#define CHESS_C_LIB_MOVES_CALCULATION_H
#include "define_types.h"
U64 generate_rook_attack(U64 occ, int sq);
U64 generate_bishop_attack(U64 occ, int sq);
#endif //CHESS_C_LIB_MOVES_CALCULATION_H
