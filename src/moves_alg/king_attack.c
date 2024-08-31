//
// Created by Jan Tlaga on 31/08/2024.
//

#include "../../include/chess/moves/king_attack.h"

U64 king_lookup_[64];

U64 generate_king_attack(U64 sq1){
    U64 left = sq1 << 1 & 0xFEFEFEFEFEFEFEFE;
    U64 right = sq1 >> 1 & 0x7F7F7F7F7F7F7F7F;
    U64 left_right_center = left | right | sq1;
    return (left | right) | (left_right_center << 8) | (left_right_center >> 8);
}

void init_king(){
    for(int i = 0; i < 64; i++){
        U64 pos = 1ULL << i;
        U64 attack = generate_king_attack(pos);
        king_lookup_[i] = attack;
    }
}

U64 get_king_attack(int sq){
    return king_lookup_[sq];
}