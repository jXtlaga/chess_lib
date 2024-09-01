//
// Created by Jan Tlaga on 29/08/2024.
//

#include "../../include/chess/moves_calculation.h"
#include "../../include/chess/combinatorics_mask.h"
#include "../../include/chess/squares_alghorithms.h"
#include "../../include/chess/bits_alghorithms.h"
#include "../../include/chess/board_visualisation.h"
U64 generate_attack_left(U64 checking_side_mask, U64 occ, U64 fill_up_sq){
    U64 ans = 0;
    U64 attack_left = (checking_side_mask & occ & fill_up_sq);
    if(attack_left == 0){
        ans |= checking_side_mask & fill_up_sq;
    }
    else{
        U64 LS1B = get_LS1B(attack_left);
        U64 fill_down_LS1B = fill_bits_down(attack_left) | LS1B;
        attack_left =  fill_down_LS1B & checking_side_mask & fill_up_sq;
        ans |= attack_left;
    }
    return ans;
}
U64 generate_rook_attack(U64 occ, int sq) {
    U64 sq1 = 1ULL << sq;
    U64 row = DISTANCE_DOWN_TO_ROW[get_distance_down(sq)];
    U64 fill_up_sq = fill_bits_up(sq1);
    U64 ans = 0;

    for(int i = 0; i < 4; i++){
        ans |= generate_attack_left(row, occ, fill_up_sq);

        occ = rotate_board_90_clockwise(occ);
        sq1 = rotate_board_90_clockwise(sq1);
        sq = get_one_bit_index(sq1);
        row = DISTANCE_DOWN_TO_ROW[get_distance_down(sq)];
        fill_up_sq = fill_bits_up(sq1);
        ans = rotate_board_90_clockwise(ans);
    }
    return ans;
}

U64 generate_bishop_attack(U64 occ, int sq) {
    U64 sq1 = 1ULL << sq;
    U64 diag = DIAGONALS[get_distance_down(sq) - get_distance_right(sq) + 7];
    U64 fill_up_sq = fill_bits_up(sq1);
    U64 ans = 0;

    for(int i = 0; i < 4; i++){
        ans |= generate_attack_left(diag, occ, fill_up_sq);

        occ = rotate_board_90_clockwise(occ);
        sq1 = rotate_board_90_clockwise(sq1);
        sq = get_one_bit_index(sq1);
        diag = DIAGONALS[get_distance_down(sq) - get_distance_right(sq) + 7];
        fill_up_sq = fill_bits_up(sq1);
        ans = rotate_board_90_clockwise(ans);
    }
    return ans;
}