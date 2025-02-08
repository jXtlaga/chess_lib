//
// Created by Jan Tlaga on 27/08/2024.
//
#include "../../include/chess/moves/bishop_attack.h"
#include "../../include/chess/algorithms/bits_algorithms.h"
#include <assert.h>
#include <printf.h>
#include "../../include/chess/moves_alg/moves_calculation.h"
#include "../../include/chess/algorithms/squares_algorithms.h"
#include "../../include/chess/board_visualisation.h"
#include "../../include/chess/magic_hash_bishop_rook.h"

U64 bishop_hash_[64][512] = {0};
U16 bishop_hash_feature_[64][512] = {0};

U16 get_bishop_attack_feature(U64 occ, int sq){
    U64 mask = BISHOP_MASK[sq];
    U64 magic = BISHOP_MAGIC[sq];
    int index = (occ & mask) * magic >> (64 - BISHOP_MASK_BITS_COUNT[sq]);
    U16 feature = bishop_hash_feature_[sq][index];
    return feature;
}

U64 get_bishop_attack(U64 occ, int sq){
    U64 mask = BISHOP_MASK[sq];
    U64 magic = BISHOP_MAGIC[sq];
    int index = (occ & mask) * magic >> (64 - BISHOP_MASK_BITS_COUNT[sq]);
    U64 attacks = bishop_hash_[sq][index];
    return attacks;
}

void init_sq_bishop(int sq){
    U64 *magic_ = bishop_hash_[sq];

    int distance_bishop[4];
    get_distance_mask_bishop(sq, distance_bishop);

    U32 possible_combination = pow2_unsigned_32_exponent(BISHOP_MASK_BITS_COUNT[sq]);

    U64 bishop_mask = BISHOP_MASK[sq];
    U64 magic_hash = BISHOP_MAGIC[sq];
    U64 bishop_bits_count = BISHOP_MASK_BITS_COUNT[sq];
    for(int combination_sequence = 0; combination_sequence < possible_combination; combination_sequence++){
        U64 sequence_mask = generate_bishop_mask_combination(combination_sequence, sq, distance_bishop);
        U64 *magic_numbers = bishop_hash_[sq];
        U32 index = (sequence_mask & bishop_mask) * magic_hash >> (64 - bishop_bits_count);
        assert(index < 4096 && "error- bigger index");
        assert(magic_numbers[index] == 0 && "error- repeated index");
        *(magic_ + index) = generate_bishop_attack(sequence_mask, sq);
    }
}
void init_bishop(){
    for(int sq = 0; sq < 64; sq++){
        init_sq_bishop(sq);
    }
}
U16 generate_bishop_feature(int sq, U64 attack){
    U64 down_fill = fill_bits(sq);
    U64 up_fill = (~down_fill) ^ (1ULL << sq);
    int distance_right = get_distance_right(sq);
    int distance_down = get_distance_down(sq);
    U64 diagonal_down_right = DIAGONAL_DOWN_RIGHT_TO_UP_LEFT[distance_right + distance_down];
    U64 diagonal_down_left = DIAGONAL_DOWN_LEFT_TO_UP_RIGHT[distance_down - distance_right + 7];

    int distance_right_up = __builtin_popcountl(diagonal_down_right & up_fill & attack);
    int distance_left_up = __builtin_popcountl(diagonal_down_left & up_fill & attack);
    int distance_left_down = __builtin_popcountl(diagonal_down_right & down_fill & attack);
    int distance_right_down = __builtin_popcountl(diagonal_down_left & down_fill & attack);

    U16 distance = distance_right_up | (distance_left_up << 3) | (distance_right_down << 6) | (distance_left_down << 9);
    return distance;
}

/*
 * creating hash table for bishop attacks distance - it is used as a feature in the neural network.
 * U16 distance = numb;
 * distance_right_up = numb & 0x7;
 * distance_left_up = (numb >> 3) & 0x7;
 * ...
 */
void init_bishop_feature(){
    for(int sq = 0; sq < 64; sq++){
        for(int i = 0; i < 512; i++){
            U64 attack = bishop_hash_[sq][i];
            bishop_hash_feature_[sq][i] = generate_bishop_feature(sq, attack);
        }
    }
}