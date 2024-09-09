//
// Created by Jan Tlaga on 29/08/2024.
//
#include "../../include/chess/algorithms/squares_alghorithms.h"
int get_distance_right(int sq){
    return sq % 8;
}
int get_distance_down(int sq){
    return (int) sq / 8;
}
U64 flip_board_diag_A8_H1(U64 x) {
    const U64 k1 = 0xaa00aa00aa00aa00ULL;
    const U64 k2 = 0xcccc0000cccc0000ULL;
    const U64 k4 = 0xf0f0f0f00f0f0f0fULL;

    U64 ans;
    ans  = x ^ (x << 36);
    x ^= k4 & (ans ^ (x >> 36));
    ans  = k2 & (x ^ (x << 18));
    x ^= ans ^ (ans >> 18);

    ans  = k1 & (x ^ (x << 9));
    x ^= ans ^ (ans >> 9);

    return x;
}
U64 flip_board_vertical(U64 x){
    return ((x << 56) |
            ((x << 40) & 0x00FF000000000000ULL) |
            ((x << 24) & 0x0000FF0000000000ULL) |
            ((x << 8) & 0x000000FF00000000ULL) |
            ((x >> 8) & 0x00000000FF000000ULL) |
            ((x >> 24) & 0x0000000000FF0000ULL) |
            ((x >> 40) & 0x000000000000FF00ULL) |
            (x >> 56));
}

U64 rotate_board_90_clockwise(U64 x){
    return flip_board_vertical(flip_board_diag_A8_H1(x));
}

U64 get_occ(Pieces_position *position) {
    return position->rook | position->bishop | position->queen | position->knight | position->pawn | position->king;
}