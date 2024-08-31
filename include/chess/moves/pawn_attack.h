//
// Created by Jan Tlaga on 31/08/2024.
//

#ifndef CHESS_C_LIB_PAWN_ATTACK_H
#define CHESS_C_LIB_PAWN_ATTACK_H

#include "../define_types.h"

void init_pawn();

U64 get_white_pawn_attack(int sq);
U64 get_black_pawn_attack(int sq);

U64 get_white_pawn_move(int sq);
U64 get_black_pawn_move(int sq);

#endif //CHESS_C_LIB_PAWN_ATTACK_H
