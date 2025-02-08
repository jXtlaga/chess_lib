//
// Created by Jan Tlaga on 9/23/24.
//

#include "../../include/chess/mask_feature_generation.h"
U64 create_pawn_attack_white(U64 occ){
    return (occ << 7) & (~ COLUMN_A) | (occ << 9) & (~ COLUMN_H);
}
U64 create_pawn_attack_black(U64 occ){
    return (occ >> 7) & (~ COLUMN_H) | (occ >> 9) & (~ COLUMN_A);
}
U64 get_pawn_attack(U64 occ, COLOR side){
    return side == WHITE ? create_pawn_attack_white(occ) : create_pawn_attack_black(occ);
}
