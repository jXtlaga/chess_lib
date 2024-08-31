//
// Created by Jan Tlaga on 31/08/2024.
//

#include "../../include/chess/moves/knight_attack.h"
U64 knight_lookup_[64];
U64 generate_knight_attack(U64 sq1){
    U64 left_1 = sq1 << 1 & 0xFEFEFEFEFEFEFEFE;
    U64 left_2 = sq1 << 2 & 0xFCFCFCFCFCFCFCFC;
    U64 right_1 = sq1 >> 1 & 0x7F7F7F7F7F7F7F7F;
    U64 right_2 = sq1 >> 2 & 0x3F3F3F3F3F3F3F3F;

    U64 att_1 = left_1 | right_1;
    U64 att_2 = left_2 | right_2;

    return (att_1 << 16) | (att_1 >> 16) | (att_2 << 8) | (att_2 >> 8);
}
void init_knight(){
    for(int i = 0; i < 64; i++){
        U64 pos = 1ULL << i;
        U64 attack = generate_knight_attack(pos);
        knight_lookup_[i] = attack;
    }
}
U64 get_knight_attack(int sq){
    return knight_lookup_[sq];
}