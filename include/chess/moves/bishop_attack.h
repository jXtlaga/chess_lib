//
// Created by Jan Tlaga on 27/08/2024.
//

#ifndef CHESS_C_LIB_BISHOP_ATTACK_H
#define CHESS_C_LIB_BISHOP_ATTACK_H
#include "../define_types.h"
#include "../combinatorics_mask.h"
U16 get_bishop_attack_feature(U64 occ, int sq);
U64 get_bishop_attack(U64 occ, int sq);
void init_bishop();
void init_bishop_feature();
#endif //CHESS_C_LIB_BISHOP_ATTACK_H
