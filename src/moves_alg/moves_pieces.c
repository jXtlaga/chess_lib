//
// Created by Jan Tlaga on 31/08/2024.
//

#include "../../include/chess/moves/moves_pieces.h"
#include "../../include/chess/moves/rook_attack.h"
#include "../../include/chess/moves/bishop_attack.h"
#include "../../include/chess/moves/knight_attack.h"
#include "../../include/chess/moves/pawn_attack.h"
#include "../../include/chess/moves/king_attack.h"

/**
 * occ - occupancy of the board
 * all_occ - occupancy of all pieces
 */
U64 rook_moves(int sq, U64 all_occ, U64 current_color_occ){
    return get_rook_attack(all_occ, sq) & ~current_color_occ;
}

U64 bishop_moves(int sq, U64 all_occ, U64 current_color_occ){
    return get_bishop_attack(all_occ, sq) & ~current_color_occ;
}

U64 queen_moves(int sq, U64 all_occ, U64 current_color_occ){
    return (get_bishop_attack(all_occ, sq) | get_rook_attack(all_occ, sq)) & ~current_color_occ;
}

U64 knight_moves(int sq, U64 current_color_occ){
    return get_knight_attack(sq) & ~current_color_occ;
}

U64 king_moves(int sq, U64 current_color_occ){
    return get_king_attack(sq) & ~current_color_occ;
}
U64 black_pawn_moves(int sq, U64 current_color_occ){
    return get_black_pawn_attack(sq) & ~current_color_occ;
}
U64 white_pawn_moves(int sq, U64 current_color_occ){
    return get_white_pawn_attack(sq) & ~current_color_occ;
}


void init_moves(){
    init_rook();
    init_bishop();
    init_knight();
    init_pawn();
    init_king();
}