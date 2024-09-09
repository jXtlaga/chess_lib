//
// Created by Jan Tlaga on 31/08/2024.
//
#include "../../include/chess/moves/pawn_attack.h"

U64 white_pawn_move_lookup_[64];
U64 black_pawn_move_lookup_[64];

U64 white_pawn_attack_lookup_[64];
U64 black_pawn_attack_lookup_[64];

U64 generate_white_pawn_attack(U64 sq1){
    U64 left_1 = sq1 << 7 & 0x7F7F7F7F7F7F7F7F;
    U64 right_1 = sq1 << 9 & 0xFEFEFEFEFEFEFEFE;
    return left_1 | right_1;
}

U64 generate_black_pawn_attack(U64 sq1){
    U64 left_1 = sq1 >> 7 & 0xFEFEFEFEFEFEFEFE;
    U64 right_1 = sq1 >> 9 & 0x7F7F7F7F7F7F7F7F;
    return left_1 | right_1;
}

U64 generate_white_pawn_move(U64 sq1){
    if((sq1 & ROW2) != 0){
        return (sq1 << 16) | (sq1 << 8);
    }
    return sq1 << 8;
}

U64 generate_black_pawn_move(U64 sq1){
    if((sq1 & ROW7) != 0){
        return (sq1 >> 16) | (sq1 >> 8);
    }
    return sq1 >> 8;
}

void init_pawn(){
    for(int i = 0; i < 64; i++){
        U64 pos = 1ULL << i;
        white_pawn_attack_lookup_[i] = generate_white_pawn_attack(pos);
        black_pawn_attack_lookup_[i] = generate_black_pawn_attack(pos);

        white_pawn_move_lookup_[i] = generate_white_pawn_move(pos);
        black_pawn_move_lookup_[i] = generate_black_pawn_move(pos);
    }
}
U64 get_white_pawn_attack(int sq){
    return white_pawn_attack_lookup_[sq];
}
U64 get_black_pawn_attack(int sq){
    return black_pawn_attack_lookup_[sq];
}
U64 get_white_pawn_move(int sq){
    return white_pawn_move_lookup_[sq];
}
U64 get_black_pawn_move(int sq){
    return black_pawn_move_lookup_[sq];
}