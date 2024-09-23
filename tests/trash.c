//
// Created by Jan Tlaga on 03/09/2024.
//
#include <stdio.h>
#include <assert.h>
//#include "../include/chess/bishop_attack.h"
#include "../include/chess/board_visualisation.h"
#include "../include/chess/define_types.h"
#include <math.h>
#include <stdlib.h>
#include "../include/chess/bits_alghorithms.h"
#include "../include/chess/moves/moves_pieces.h"
#include "../include/chess/moves_position.h"
#include "../include/chess/console_visualization.h"
#include "../include/chess/console_chess.h"
#include "../include/chess/moves/pawn_attack.h"
#include "../include/chess/moves/rook_attack.h"
#include "../include/chess/combinatorics_mask.h"

U32 get_key_U32(U64 mask, U64 magic, U32 magic_shift) {
    return (mask * magic) >> magic_shift;
}
int get_compressed_size(int combination_len, bool include_zero) {
    int compressed_size;
    if (include_zero == true) {
        double log_comb = log2(combination_len + 1);
        compressed_size = (int) ceil(log_comb);
    } else {
        double log_comb = log2(combination_len);
        compressed_size = (int) ceil(log_comb);
    }
    return compressed_size;

}
/**
 * exist_index - array of indexes that are already used. If index is used, then exist_index[index] = 1
 *
 * @param combination_len
 * @param include_zero
 * @return
 */
int generate_magic_U32(int combination_len, bool include_zero) {
    assert(combination_len < 32);
    assert(combination_len > 0);
    int compressed_size = get_compressed_size(combination_len, include_zero);
    U32 comb_1 = 1ULL << combination_len - 1;
    U32 shift = combination_len - compressed_size;

    int exist_index[1 << compressed_size];
    for (int i = 0; i < (1 << compressed_size); i++) {
        exist_index[i] = 0;
    }
    if (include_zero == true) {
        exist_index[0] = 1;
    }

    int level = 0;

    int *products = (int *) malloc(sizeof(int) * 2);
    products[0] = -2;
    products[1] = 1;
    int index_products = 2;

    U32 keys[combination_len];
    for (int i = 0; i < combination_len; i++) {
        keys[i] = 0;
    }
    int index_keys = 0;


    while (level < combination_len) {
        int current_product = products[index_products - 1];
        U32 combination_level = comb_1 >> level;
        U32 key = get_key_U32(combination_level, current_product, shift);

        if (current_product == -1) {
            level--;
            index_keys--;
            exist_index[keys[index_keys]] = 0;
            keys[index_keys] = 0;
            index_products--;
        } else if (current_product == -2) {
            assert(0 && "error- magic not found");
        } else if (exist_index[key] == 0) {
            level++;
            keys[index_keys] = key;
            index_keys++;

            exist_index[key] = 1;
            products[index_products - 1] = -1;
            U32 add_product = (U32) (pow(2, level));
            products = (int *) realloc(products, sizeof(int) * (index_products + 2));
            products[index_products] = add_product + current_product;
            products[index_products + 1] = current_product;
            index_products += 2;

        } else {
            index_products--;
        }
    }
    int ans = products[index_products - 1];
    free(products);
    return ans;

}

U64 generate_line_magic(U64 mask_full, int size) {
    U64 mask = mask_full;
    U64 gen_magic = 0;
    for (int i = size; i > 0; i--) {
        U64 LS1B = get_LS1B(mask);
        int index = get_one_bit_index(LS1B);
        mask = mask & ~LS1B;
        gen_magic |= pow2_unsigned_64_exponent(63 - index - i);
        print_bitboard(gen_magic*LS1B);
    }
    return gen_magic;
}


int main() {
    int compressed_size = get_compressed_size(14, true);
    int index_test[compressed_size];
    for (int i = 0; i < compressed_size; i++) {
        index_test[i] = 0;
    }
    int shift_main = 63 - compressed_size;
    int count_distance = 14;
    for(int i = 0; i < 1; i++){
        int distance[4];
        get_distance_mask_rook(i, distance);
        U64 mask = generate_rook_full(i);
        U64 magic = generate_line_magic(mask, count_distance);
        U32 shift = 63 - count_distance;
        U32 magic_number = generate_magic_U32(count_distance, true);
        printf("___________\nindex: %d\n", i);
        print_bitboard(mask *magic);
        print_bitboard(magic);
        printf("magic_number: %llu\n", magic);
        while(mask != 0){
            U64 LS1B = get_LS1B(mask);
            int index = get_one_bit_index(LS1B);
            mask = mask & ~LS1B;
            U64 comb = (LS1B * magic) >> shift;
            printf("index_comb: %u\n", comb);
            print_bitboard(comb);
            print_bitboard(LS1B);
        }

    }
    return 0;
}