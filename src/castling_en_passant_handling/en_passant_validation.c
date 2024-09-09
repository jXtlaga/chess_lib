//
// Created by Jan Tlaga on 9/8/24.
//

#include "../../include/chess/en_passant_validation.h"
#include "../../include/chess/moves/pawn_attack.h"

/* en_passant
 * functions en_passant return pawns occ that can do en passant
 */
U64 white_en_passant(int sq_en_passant, U64 current_pawns_occ){
    return get_black_pawn_attack(sq_en_passant) & current_pawns_occ;
}
U64 black_en_passant(int sq_en_passant, U64 current_pawns_occ){
    return get_white_pawn_attack(sq_en_passant) & current_pawns_occ;
}

U8 is_en_passant_white_valid(U64 to_move){
    return (to_move & 0xFF00) >> 16;
}
U8 is_en_passant_black_valid(U64 to_move){
    return (to_move & 0xFF0000000000) >> 40;
}