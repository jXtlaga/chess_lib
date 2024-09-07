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

/**
 * Check if castling is invalided
  the king or rook moves from the starting position
  (after finding that is invalided we can remove the castling rights and function shouldn't be called again)
 */
bool is_castling_white_short_invalided(U64 current_color_occ){
    return (current_color_occ & 0b1001) == 0b1001? false : true;
}
bool is_castling_white_long_invalided(U64 current_color_occ){
    return (current_color_occ & 0b10001000) == 0b10001000? false : true;
}
bool is_castling_black_short_invalided(U64 current_color_occ){
    return (current_color_occ & 0x0900000000000000) == 0x0900000000000000? false : true;
}
bool is_castling_black_long_invalided(U64 current_color_occ){
    return (current_color_occ & 0x8800000000000000) == 0x8800000000000000? false : true;
}

bool is_castling_short_white_possible(U64 current_color_occ, U64 att_mask){
    if((att_mask & 0b1110) != 0)
        return false;
    if((current_color_occ & 0b1111) == 0b1001)
        return true;
    return false;
}
bool is_castling_long_white_possible(U64 current_color_occ, U64 att_mask){
    if((att_mask & 0x78) != 0)
        return false;
    if((current_color_occ & 0xF8) == 0x88)
        return true;
    return false;
}
bool is_castling_short_black_possible(U64 current_color_occ, U64 att_mask){
    if((att_mask & 0x0E00000000000000) != 0)
        return false;
    if((current_color_occ & 0x0F00000000000000) == 0x0900000000000000)
        return true;
    return false;
}
bool is_castling_long_black_possible(U64 current_color_occ, U64 att_mask){
    if((att_mask & 0x7800000000000000) != 0)
        return false;
    if((current_color_occ & 0xF800000000000000) == 0x8800000000000000)
        return true;
    return false;
}
U64 white_pawn_moves_start(int sq, U64 all_occ, U64 current_color_occ, U64 enemy_color_occ){
    U64 move = get_rook_attack(all_occ, sq)  & ~current_color_occ & get_white_pawn_move(sq);
    U64 attack = get_white_pawn_attack(sq) & enemy_color_occ;
    return move | attack;
}
U64 white_pawn_moves_start_struct(Board_move_info *info){
    U64 move = get_rook_attack(info->all_occ, info->sq) & get_white_pawn_move(info->sq);
    U64 attack = get_white_pawn_attack(info->sq) & info->enemy_color_occ;
    return move | attack;
}

U64 black_pawn_moves_start(int sq, U64 all_occ, U64 current_color_occ, U64 enemy_color_occ){
    U64 move = get_rook_attack( all_occ, sq) & ~current_color_occ & get_black_pawn_move(sq);
    U64 attack = get_black_pawn_attack(sq) & enemy_color_occ;
    return move | attack;
}
U64 black_pawn_moves_start_struct(Board_move_info *info){
    U64 move = get_rook_attack(info->all_occ, info->sq) & ~(info->current_color_occ) & get_black_pawn_move(info->sq);
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

U64 white_pawn_en_passant(int sq, U64 en_passant){
    U64 attack = get_white_pawn_attack(sq) & en_passant;
    return attack;
}
U64 black_pawn_en_passant_attack(int sq, U64 en_passant){
    U64 attack = get_black_pawn_attack(sq) & en_passant;
    return attack;
}


void init_moves(){
    init_rook();
    init_bishop();
    init_knight();
    init_pawn();
    init_king();
}