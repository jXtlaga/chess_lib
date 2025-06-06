//
// Created by Jan Tlaga on 01/09/2024.
//
#include "../../include/chess/mask_alg/check_mask.h"
#include "../../include/chess/moves/rook_attack.h"
#include "../../include/chess/moves/bishop_attack.h"
#include "../../include/chess/moves/knight_attack.h"
#include "../../include/chess/moves/king_attack.h"
#include "../../include/chess/moves/pawn_attack.h"
#define true 1
#define false 0
#include <assert.h>

#include "../../include/chess/moves/rook_attack.h"
#include "../../include/chess/algorithms/bits_algorithms.h"
#include "../../include/chess/moves/bishop_attack.h"
#include "../../include/chess/algorithms/squares_algorithms.h"
#include "../../include/chess/board_visualisation.h"
U64 get_check_rook(int king_sq, U64 all_occ, U64 enemy_rook_queen_occ){
    U64 mask = get_rook_attack(all_occ, king_sq);
    U64 attackers = mask & enemy_rook_queen_occ;
    if ((mask & enemy_rook_queen_occ) != 0){
        U64 attackers_mask = get_rook_attack(all_occ, get_one_bit_index(get_LS1B(attackers)));
        mask &= attackers_mask;
        mask |= attackers;
        return mask;
    }
    return 0;
}

U64 get_check_bishop(int king_sq, U64 all_occ, U64 enemy_bishop_queen_occ){
    U64 mask = get_bishop_attack(all_occ, king_sq);
    U64 attackers = mask & enemy_bishop_queen_occ;
    if ((mask & enemy_bishop_queen_occ) != 0){
        U64 attackers_mask = get_bishop_attack(all_occ, get_one_bit_index(get_LS1B(attackers)));
        mask &= attackers_mask;
        mask |= attackers;
        return mask;
    }
    return 0;
}
U64 get_check_knight(int king_sq, U64 enemy_knight_occ){
    U64 mask = get_knight_attack(king_sq);
    return mask & enemy_knight_occ;
}

U64 get_check_pawn(int king_sq, U64 enemy_pawn_occ, COLOR king_side) {
    U64 mask = king_side == WHITE ? get_white_pawn_attack(king_sq) : get_black_pawn_attack(king_sq);
    return mask & enemy_pawn_occ;
}

