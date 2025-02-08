//
// Created by Jan Tlaga on 30/08/2024.
//

#include "../../include/chess/moves/rook_attack.h"
#include "../../include/chess/combinatorics_mask.h"
#include "../../include/chess/moves_alg/moves_calculation.h"
#include "../../include/chess/algorithms/bits_algorithms.h"
#include "../../include/chess/algorithms/squares_algorithms.h"
#include "../../include/chess/console_visualization.h"
#include "../../include/chess/board_visualisation.h"
#include "../../include/chess/magic_hash_bishop_rook.h"
#include <assert.h>
#include <printf.h>

U64 rook_hash_[64][4096] = {0};
U16 rook_hash_feature_[64][4096] = {0};

U16 get_rook_attack_feature(U64 occ, int sq){
    U64 mask = ROOK_MASK[sq];
    U64 magic = ROOK_MAGIC[sq];
    int index = (occ & mask) * magic >> (64 - ROOK_MASK_BITS_COUNT[sq]);
    U16 feature = rook_hash_feature_[sq][index];
    return feature;
}
U64 get_rook_attack(U64 occ, int sq){
    U64 mask = ROOK_MASK[sq];
    U64 magic = ROOK_MAGIC[sq];
    int index = (occ & mask) * magic >> (64 - ROOK_MASK_BITS_COUNT[sq]);
    U64 attacks = rook_hash_[sq][index];
    return attacks;
}
void init_sq_rook(int sq){
    U64 *magic_ = rook_hash_[sq];

    int distance_rook[4];
    get_distance_mask_rook(sq, distance_rook);

    U32 possible_combination = pow2_unsigned_32_exponent(ROOK_MASK_BITS_COUNT[sq]);

    U64 rook_mask = ROOK_MASK[sq];
    U64 magic_hash = ROOK_MAGIC[sq];
    U64 rook_bits_count = ROOK_MASK_BITS_COUNT[sq];
    for(int combination_sequence = 0; combination_sequence < possible_combination; combination_sequence++){
        U64 sequence_mask = generate_rook_mask_combination(combination_sequence, sq, distance_rook);
        U64 *magic_numbers = rook_hash_[sq];
        U32 index = (sequence_mask & rook_mask) * magic_hash >> (64 - rook_bits_count);
        assert(index < 4096 && "error- bigger index");
        assert(magic_numbers[index] == 0 && "error- repeated index");
        *(magic_ + index) = generate_rook_attack(sequence_mask, sq);
    }
}
void init_rook(){
    for(int sq = 0; sq < 64; sq++){
        init_sq_rook(sq);
    }
}
U16 generate_rook_feature(int sq, U64 attack){
    U64 down_fill = fill_bits(sq);
    U64 up_fill = (~down_fill) ^ (1ULL << sq);
    int distance_right = get_distance_right(sq);
    int distance_down = get_distance_down(sq);
    U64 row = DISTANCE_DOWN_TO_ROW[distance_down];
    U64 column = DISTANCE_RIGHT_TO_COLUMN[distance_right];
    int distance_right_attack = __builtin_popcountl(row & down_fill & attack);
    int distance_left_attack = __builtin_popcountl(row & up_fill & attack);
    int distance_up_attack = __builtin_popcountl(column & up_fill & attack);
    int distance_down_attack = __builtin_popcountl(column & down_fill & attack);
    U16 distance = distance_right_attack | (distance_left_attack << 3) | (distance_up_attack << 6) | (distance_down_attack << 9);
    return distance;
}
void init_rook_feature(){
    for(int sq = 0; sq < 64; sq++){
        for(int i = 0; i < 4096; i++){
            rook_hash_feature_[sq][i] = generate_rook_feature(sq, rook_hash_[sq][i]);
        }
    }
}