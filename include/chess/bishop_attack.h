//
// Created by Jan Tlaga on 27/08/2024.
//

#ifndef CHESS_C_LIB_BISHOP_ATTACK_H
#define CHESS_C_LIB_BISHOP_ATTACK_H
#include "define_types.h"
#include "combinatorics_mask.h"
void bishop_init();
U64 get_bishop_attack(U64 occ, int sq);
#endif //CHESS_C_LIB_BISHOP_ATTACK_H
