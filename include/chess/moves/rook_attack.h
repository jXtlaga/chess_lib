//
// Created by Jan Tlaga on 30/08/2024.
//

#ifndef CHESS_C_LIB_ROOK_ATTACK_H_H
#define CHESS_C_LIB_ROOK_ATTACK_H_H
#include "../define_types.h"
U16  get_rook_attack_feature(U64 occ, int sq);
U64 get_rook_attack(U64 occ, int sq);
void init_rook();
void init_rook_feature();
#endif //CHESS_C_LIB_ROOK_ATTACK_H_H
