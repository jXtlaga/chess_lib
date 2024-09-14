//
// Created by Jan Tlaga on 31/08/2024.
//

#include "../../include/chess/moves_alg/moves_pieces.h"
#include "../../include/chess/moves/rook_attack.h"
#include "../../include/chess/moves/bishop_attack.h"
#include "../../include/chess/moves/knight_attack.h"
#include "../../include/chess/moves/pawn_attack.h"
#include "../../include/chess/moves/king_attack.h"
#include "../../include/chess/board_visualisation.h"

/**
 * occ - occupancy of the board
 * all_occ - occupancy of all pieces
 */
U64 rook_moves(int sq, U64 all_occ, U64 current_color_occ){
    return get_rook_attack(all_occ, sq) & ~current_color_occ;
}
U64 rook_moves_struct(Board_move_info *info){
    return get_rook_attack(info->all_occ, info->sq) & ~info->current_color_occ;
}

U64 bishop_moves(int sq, U64 all_occ, U64 current_color_occ){
    return get_bishop_attack(all_occ, sq) & ~current_color_occ;
}
U64 bishop_moves_struct(Board_move_info *info){
    return get_bishop_attack(info->all_occ, info->sq) & ~info->current_color_occ;
}

U64 queen_moves(int sq, U64 all_occ, U64 current_color_occ){
    return (get_bishop_attack(all_occ, sq) | get_rook_attack(all_occ, sq)) & ~current_color_occ;
}

U64 queen_moves_struct(Board_move_info *info){
    return (get_bishop_attack(info->all_occ, info->sq) | get_rook_attack(info->all_occ, info->sq)) & ~info->current_color_occ;
}

U64 knight_moves(int sq, U64 current_color_occ){
    return get_knight_attack(sq) & ~current_color_occ;
}
U64 knight_moves_struct(Board_move_info *info){
    return get_knight_attack(info->sq) & ~info->current_color_occ;
}

U64 king_moves(int sq, U64 current_color_occ){
    return get_king_attack(sq) & ~current_color_occ;
}
U64 king_moves_struct(Board_move_info *info){
    return get_king_attack(info->sq) & ~info->current_color_occ;
}

U64 white_pawn_moves_start(int sq, U64 all_occ, U64 enemy_color_occ){
    U64 move = get_rook_attack(all_occ, sq)  & ~all_occ & get_white_pawn_move(sq);
    U64 attack = get_white_pawn_attack(sq) & enemy_color_occ;
    return move | attack;
}
U64 white_pawn_moves_start_struct(Board_move_info *info){
    U64 move = get_rook_attack(info->all_occ, info->sq) & ~(info->all_occ) & get_white_pawn_move(info->sq);
    U64 attack = get_white_pawn_attack(info->sq) & info->enemy_color_occ;
    return move | attack;
}

U64 black_pawn_moves_start(int sq, U64 all_occ, U64 enemy_color_occ){
    U64 move = get_rook_attack( all_occ, sq) & ~all_occ & get_black_pawn_move(sq);
    U64 attack = get_black_pawn_attack(sq) & enemy_color_occ;
    return move | attack;
}
U64 black_pawn_moves_start_struct(Board_move_info *info){
    U64 move = get_rook_attack(info->all_occ, info->sq) & ~(info->all_occ) & get_black_pawn_move(info->sq);
    U64 attack = get_black_pawn_attack(info->sq) & info->enemy_color_occ;
    return move | attack;
}

U64 white_pawn_moves(int sq, U64 all_occ, U64 enemy_color_occ){
    U64 move = get_white_pawn_move(sq) & ~all_occ;
    U64 attack = get_white_pawn_attack(sq) & enemy_color_occ;
    return move | attack;
}
U64 white_pawn_moves_struct(Board_move_info *info){
    U64 move = get_white_pawn_move(info->sq) & ~info->all_occ;
    U64 attack = get_white_pawn_attack(info->sq) & info->enemy_color_occ;
    return move | attack;
}

U64 black_pawn_moves(int sq, U64 all_occ, U64 enemy_color_occ){
    U64 move = get_black_pawn_move(sq) & ~all_occ;
    U64 attack = get_black_pawn_attack(sq) & enemy_color_occ;
    return move | attack;
}
U64 black_pawn_moves_struct(Board_move_info *info){
    U64 move = get_black_pawn_move(info->sq) & ~info->all_occ;
    U64 attack = get_black_pawn_attack(info->sq) & info->enemy_color_occ;
    return move | attack;
}

void init_moves(){
    init_rook();
    init_bishop();
    init_knight();
    init_pawn();
    init_king();
}