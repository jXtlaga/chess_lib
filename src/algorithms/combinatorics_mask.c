//
// Created by Jan Tlaga on 27/08/2024.
//
#include "../../include/chess/combinatorics_mask.h"
#include "../../include/chess/algorithms/bits_algorithms.h"
#include "../../include/chess/algorithms/squares_algorithms.h"
#include "../../include/chess/board_visualisation.h"
#include <stdio.h>

int count_rook_line_mask(int distance) {
    int count;
    if (distance == 0 || distance == 7) {
        count = 6;
    } else {
        count = 5;
    }
    return count;
}

int count_rook_full_mask(int distance_right, int distance_down) {
    return count_rook_line_mask(distance_right) + count_rook_line_mask(distance_down);;
}

int count_bishop_half_line_mask(int distance_x, int distance_y) {
    int count = distance_x < distance_y ? distance_x : distance_y;
    if (count != 0) {
        count -= 1;
    }
    return count;
}

int count_bishop_full_mask(int distance_right, int distance_down) {
    int count;
    int distance_up = 7 - distance_down;
    int distance_left = 7 - distance_right;
    count = count_bishop_half_line_mask(distance_right, distance_up);
    count += count_bishop_half_line_mask(distance_up, distance_left);
    count += count_bishop_half_line_mask(distance_left, distance_down);
    count += count_bishop_half_line_mask(distance_down, distance_right);
    return count;
}

void print_mask_bit_count(int count_piece_mask(int, int)) {
    printf("{ ");
    for (int sq = 0; sq < 64; sq++) {
        int distance_right = get_distance_right(sq);
        int distance_down = get_distance_down(sq);
        if (sq % 8 == 0) {
            printf("\n");
        }
        printf("%d, ", count_piece_mask(distance_right, distance_down));
    }
    printf("\n};");
}

void print_bishop_full_mask_bit_count() {
    print_mask_bit_count(count_bishop_full_mask);
}

void print_rook_full_mask_bit_count() {
    print_mask_bit_count(count_rook_full_mask);
}

U64 create_mask(U64 combination, int shift) {
    return combination << shift;
}
/*
 * from combination [0,...,0,1,1,1] where combination_bit_count is number of bits from right that we want do rotate
 * for example:
 * [1 0 0
 *  0 1 0
 *  0 0 1]
 */
U64 create_45_or_225_degree_combination(U64 combination, U32 combination_bit_count) {
    U64 diagonal_combination = 0;
    U32 shift = combination_bit_count - 1;
    for (int i = 0; i < combination_bit_count; i++) {
        diagonal_combination |= (combination & (1ULL << i)) << shift * 8;
        shift -= 1;
    }
    return diagonal_combination;
}

U64 create_135_or_315_degree_combination(U64 combination, U32 combination_bit_count) {
    U64 diagonal_combination = 0;
    U32 shift = 0;
    for (int i = 0; i < combination_bit_count; i++) {
        diagonal_combination |= (combination & (1ULL << i)) << shift * 8;
        shift += 1;
    }
    return diagonal_combination;
}

U64 create_90_or_270_degree_combination(U64 combination, U32 combination_bit_count) {
    U64 diagonal_combination = 0;
    U32 shift = 0;
    for (int i = 0; i < combination_bit_count; i++) {
        diagonal_combination |= (combination & (1ULL)) << shift * 8;
        combination >>= 1;
        shift += 1;
    }
    return diagonal_combination;
}

U32 get_shift_0(U32 sq, U32 combination_count) {
    return sq - combination_count;
}

U32 get_shift_45(U32 sq, U32 combination_count) {
    return sq + 8 - combination_count;
}

U32 get_shift_90(U32 sq, U32 combination_count) {
    return sq + 8;
}

U32 get_shift_135(U32 sq, U32 combination_count) {
    return sq + 9;
}

U32 get_shift_180(U32 sq, U32 combination_count) {
    return sq + 1;
}

U32 get_shift_225(U32 sq, U32 combination_count) {
    return sq - 7 * combination_count - combination_count + 1;
}

U32 get_shift_270(U32 sq, U32 combination_count) {
    return sq - combination_count * 8;
}

U32 get_shift_315(U32 sq, U32 combination_count) {
    return sq - combination_count * 9;
}

U64 get_mask_from_degree(int degree, U64 combination, U32 combination_bit_count, int sq) {
    switch ( degree ) {
        case 0:
            return create_mask(combination, get_shift_0(sq, combination_bit_count));
        case 45: {
            U64 combination_rotated = create_45_or_225_degree_combination(combination, combination_bit_count);
            return create_mask(combination_rotated, get_shift_45(sq, combination_bit_count));
        }
        case 90: {
            U64 combination_rotated = create_90_or_270_degree_combination(combination, combination_bit_count);
            return create_mask(combination_rotated, get_shift_90(sq, combination_bit_count));
        }
        case 135: {
            U64 combination_rotated = create_135_or_315_degree_combination(combination, combination_bit_count);
            return create_mask(combination_rotated, get_shift_135(sq, combination_bit_count));
        }
        case 180:
            return create_mask(combination, get_shift_180(sq, combination_bit_count));
        case 225: {
            U64 combination_rotated = create_45_or_225_degree_combination(combination, combination_bit_count);
            return create_mask(combination_rotated, get_shift_225(sq, combination_bit_count));
        }
        case 270: {
            U64 combination_rotated = create_90_or_270_degree_combination(combination, combination_bit_count);
            return create_mask(combination_rotated, get_shift_270(sq, combination_bit_count));
        }
        case 315: {
            U64 combination_rotated = create_135_or_315_degree_combination(combination, combination_bit_count);
            return create_mask(combination_rotated, get_shift_315(sq, combination_bit_count));
        }
        default:
            return 0;

    }
}
/**
 * generate mask from combination sequence:
 * for example generate_rook_mask_combination(sq = 7, combination_sequence = 0b(1111 0111 1011)
 * [0 0 0 0 0 0 0 0]
 * [1 0 0 0 0 0 0 0]
 * [1 0 0 0 0 0 0 0]
 * [1 0 0 0 0 0 0 0]
 * [1 0 0 0 0 0 0 0]
 * [0 0 0 0 0 0 0 0]
 * [1 0 0 0 0 0 0 0]
 * [r 1 1 1 0 1 1 0]
 */
U64 generate_rook_mask_combination(U64 combination_sequence, int sq, int *distance_direction_rook) {
    U64 combination_iteration = combination_sequence;
    U64 combination_mask = 0;
    for (int i = 0; i < 4; i++) {
        U64 one_side_combination = combination_iteration & fill_bits(distance_direction_rook[i]);
        combination_iteration >>= distance_direction_rook[i];
        U64 mask_side = get_mask_from_degree(i * 90, one_side_combination, distance_direction_rook[i], sq);
        combination_mask |= mask_side;
    }
    return combination_mask;
}

U64 generate_bishop_mask_combination(U64 combinations_sequence, int sq, int *distance_direction_bishop) {
    U64 combinations_iteration = combinations_sequence;
    U64 combination_mask = 0;
    for (int i = 0; i < 4; i++) {
        U64 one_side_combination = combinations_iteration & fill_bits(distance_direction_bishop[i]);
        combinations_iteration >>= distance_direction_bishop[i];
        U64 mask_side = get_mask_from_degree(i * 90 + 45, one_side_combination, distance_direction_bishop[i], sq);
        combination_mask |= mask_side;
    }
    return combination_mask;
}

void get_distance_full_rook(int sq, int *distance_array) {
    distance_array[0] = get_distance_right(sq);
    distance_array[1] = 7 - get_distance_down(sq);
    distance_array[2] = 7 - get_distance_right(sq);
    distance_array[3] = get_distance_down(sq);
}

void get_distance_full_bishop(int sq, int *distance_array) {
    int distance_down = get_distance_down(sq);
    int distance_right = get_distance_right(sq);
    int distance_up = 7 - distance_down;
    int distance_left = 7 - distance_right;

    distance_array[0] = distance_right < distance_up ? distance_right : distance_up;
    distance_array[1] = distance_up < distance_left ? distance_up : distance_left;
    distance_array[2] = distance_left < distance_down ? distance_left : distance_down;
    distance_array[3] = distance_down < distance_right ? distance_down : distance_right;
}

void get_distance_mask_rook(int sq, int *distance_array) {
    int distance_down = get_distance_down(sq);
    int distance_right = get_distance_right(sq);
    int distance_up = 7 - distance_down;
    int distance_left = 7 - distance_right;

    distance_array[0] = distance_right == 0 ? distance_right : distance_right - 1;
    distance_array[1] = distance_up == 0 ? distance_up : distance_up - 1;
    distance_array[2] = distance_left == 0 ? distance_left : distance_left - 1;
    distance_array[3] = distance_down == 0 ? distance_down : distance_down - 1;
}

void get_distance_mask_bishop(int sq, int *distance_array) {
    int distance_down = get_distance_down(sq);
    int distance_right = get_distance_right(sq);
    int distance_up = 7 - distance_down;
    int distance_left = 7 - distance_right;

    distance_array[0] = count_bishop_half_line_mask(distance_right, distance_up);
    distance_array[1] = count_bishop_half_line_mask(distance_up, distance_left);
    distance_array[2] = count_bishop_half_line_mask(distance_left, distance_down);
    distance_array[3] = count_bishop_half_line_mask(distance_down, distance_right);
}

U64 generate_rook_full(int sq) {
    U64 combination_sequence = pow2_unsigned_64_exponent(14);
    combination_sequence--;
    int direction_rook[4];
    get_distance_full_rook(sq, direction_rook);
    return generate_rook_mask_combination(combination_sequence, sq, direction_rook);
}

U64 generate_bishop_full(int sq){
    int direction_bishop[4];
    get_distance_full_bishop(sq, direction_bishop);

    int count_bits = 0;
    for (int i = 0; i < 4; i++) {
        count_bits += direction_bishop[i];
    }
    U64 combination_sequence = pow2_unsigned_64_exponent(count_bits);
    combination_sequence--;
    return generate_bishop_mask_combination(combination_sequence, sq, direction_bishop);
}

U64 generate_rook_full_mask(int sq) {
    U64 combination_sequence = pow2_unsigned_32_exponent(
            count_rook_full_mask(get_distance_right(sq), get_distance_down(sq)));
    combination_sequence--;
    int direction_rook[4];
    get_distance_mask_rook(sq, direction_rook);
    return generate_rook_mask_combination(combination_sequence, sq, direction_rook);
}

U64 generate_bishop_full_mask(int sq) {
    U64 combination_sequence = pow2_unsigned_32_exponent(
            count_bishop_full_mask(get_distance_right(sq), get_distance_down(sq)));
    combination_sequence--;
    int direction_bishop[4];
    get_distance_mask_bishop(sq, direction_bishop);
    return generate_bishop_mask_combination(combination_sequence, sq, direction_bishop);

}
