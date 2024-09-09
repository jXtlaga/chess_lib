//
// Created by Jan Tlaga on 05/09/2024.
//

#include "../../include/chess/mask_alg/pin_mask.h"
#include "../../include/chess/algorithms/bits_alghorithms.h"
#include "../../include/chess/moves/rook_attack.h"
#include "../../include/chess/moves/bishop_attack.h"
#include "../../include/chess/algorithms/squares_alghorithms.h"

U64 get_pin_side(U64 side_attackers, U64 king_sq1, U64 king_super_attack, U64 current_color_occ, U64 enemy_color_occ, U64(get_attack)(U64, int)){
    U64 ans = 0;
    while(side_attackers != 0){
        U64 attacker = get_LS1B(side_attackers);
        side_attackers ^= attacker;

        U64 between_king_attacker = get_attack(king_sq1, get_one_bit_index(attacker)) & king_super_attack;
        U64 mask_copy = between_king_attacker;
        if((mask_copy & enemy_color_occ) != 0){
            continue;
        }
        mask_copy &= current_color_occ;
        if (mask_copy == 0){
            continue;
        }
        mask_copy ^= get_LS1B(mask_copy);
        if(mask_copy == 0){
            ans |= between_king_attacker | attacker;
        }
    }
    return ans;
}

void get_pin_rook(int king_sq, U64 current_color_occ, U64 enemy_color_occ, U64 enemy_rook_queen_occ, U64 *pin_rook){
    U64 king_sq1 = 1ULL << king_sq;
    U64 king_super_rook = get_rook_attack(enemy_rook_queen_occ, king_sq);

    U64 column_attackers = king_super_rook & DISTANCE_RIGHT_TO_COLUMN[get_distance_right(king_sq)] & enemy_rook_queen_occ;
    U64 row_attackers = king_super_rook & DISTANCE_DOWN_TO_ROW[get_distance_down(king_sq)] & enemy_rook_queen_occ;

    *(pin_rook) = get_pin_side(column_attackers, king_sq1, king_super_rook, current_color_occ, enemy_color_occ, get_rook_attack);
    *(pin_rook + 1) = get_pin_side(row_attackers, king_sq1, king_super_rook, current_color_occ, enemy_color_occ, get_rook_attack);
}
void get_pin_bishop(int king_sq, U64 current_color_occ, U64 enemy_color_occ, U64 enemy_bishop_queen_occ, U64 *pin_bishop){
    U64 king_sq1 = 1ULL << king_sq;
    U64 king_super_bishop = get_bishop_attack(enemy_bishop_queen_occ, king_sq);

    int distance_right = get_distance_right(king_sq);
    int distance_down = get_distance_down(king_sq);

    U64 diagonal_attackers = king_super_bishop & DIAGONAL_DOWN_RIGHT_TO_UP_LEFT[distance_right + distance_down ] & enemy_bishop_queen_occ;
    U64 anti_diagonal_attackers = king_super_bishop & DIAGONAL_DOWN_LEFT_TO_UP_RIGHT[distance_down - distance_right + 7] & enemy_bishop_queen_occ;
    *(pin_bishop) = get_pin_side(diagonal_attackers, king_sq1, king_super_bishop, current_color_occ, enemy_color_occ, get_bishop_attack);
    *(pin_bishop + 1) = get_pin_side(anti_diagonal_attackers, king_sq1, king_super_bishop, current_color_occ, enemy_color_occ, get_bishop_attack);
}