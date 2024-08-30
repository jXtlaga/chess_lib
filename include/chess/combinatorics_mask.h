//
// Created by Jan Tlaga on 27/08/2024.
//

#ifndef CHESS_C_LIB_COMBINATORICS_MASK_H
#define CHESS_C_LIB_COMBINATORICS_MASK_H
#include "define_types.h"
int count_rook_full_mask(int distance_right, int distance_down);
int count_bishop_full_mask(int distance_right, int distance_down);

U64 generate_rook_mask_combination(int sq, U64 combination_sequence);
U64 generate_bishop_mask_combination(U64 combinations_sequence,int sq, int distance_right_up, int distance_up_left, int distance_left_down, int distance_down_right){

U64 generate_rook_full_mask(int sq);
U64 generate_bishop_full_mask(int sq);

void print_bishop_full_mask_bit_count();
void print_rook_full_mask_bit_count();

#endif //CHESS_C_LIB_COMBINATORICS_MASK_H
