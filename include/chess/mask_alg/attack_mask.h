//
// Created by Jan Tlaga on 05/09/2024.
//

#ifndef CHESS_C_LIB_ATTACK_MASK_H
#define CHESS_C_LIB_ATTACK_MASK_H
#include "../define_types.h"
U64 get_full_enemy_attack_mask(Pieces_position enemy_pieces, U64 all_occ_without_king, COLOR side);
#endif //CHESS_C_LIB_ATTACK_MASK_H
