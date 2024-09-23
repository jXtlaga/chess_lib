//
// Created by Jan Tlaga on 9/8/24.
//

#include "../../include/chess/en_passant_validation.h"
#include "../../include/chess/moves/pawn_attack.h"
#include "../../include/chess/algorithms/bits_algorithms.h"
#include "../../include/chess/mask_alg/check_mask.h"
#include "../../include/chess/moves/bishop_attack.h"
#include "../../include/chess/moves/rook_attack.h"
#include "../../include/chess/board_visualisation.h"

/* en_passant
 * functions en_passant return pawns occ that can do en passant
 */
U64 white_en_passant(int sq_en_passant, U64 current_pawns_occ){
    return get_black_pawn_attack(sq_en_passant) & current_pawns_occ;
}
U64 black_en_passant(int sq_en_passant, U64 current_pawns_occ){
    return get_white_pawn_attack(sq_en_passant) & current_pawns_occ;
}

bool en_passant_pin_taken(U64 hypothetical_occ, int king_sq, U64 enemy_rook_queen_occ){
    U64 mask = get_rook_attack(hypothetical_occ, king_sq);
    if ((mask & enemy_rook_queen_occ) != 0){
        return true;
    }
    return false;
}
/* is_en_passant_valid
 * functions is_en_passant_valid return sq where pawn can do en passant
 */
U8 is_en_passant_white_valid(U64 to_move){
    U64 move_two_up = to_move & 0xFF000000;
    if (move_two_up){
        return get_one_bit_index(move_two_up) - 8;
    }
    else {
        return 0;
    }
}
U8 is_en_passant_black_valid(U64 to_move){
    U64 move_two_up = to_move & 0xFF00000000;
    if (move_two_up){
        return get_one_bit_index(move_two_up) + 8;
    }
    else {
        return 0;
    }
}