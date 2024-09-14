//
// Created by Jan Tlaga on 9/8/24.
//
#include "../../include/chess/castling_validation.h"
#include "../../include/chess/board_visualisation.h"
/**
 * Check if castling is invalided
  the king or rook moves from the starting position
  (after finding that is invalided we can remove the castling rights and function shouldn't be called again)
 */
bool is_castling_white_short_valid(U64 current_color_occ){
    return (current_color_occ & 0b1001) == 0b1001? true : false;
}
bool is_castling_white_long_valid(U64 current_color_occ){
    return (current_color_occ & 0b10001000) == 0b10001000? true : false;
}
bool is_castling_black_short_valid(U64 current_color_occ){
    return (current_color_occ & 0x0900000000000000) == 0x0900000000000000? true : false;
}
bool is_castling_black_long_valid(U64 current_color_occ){
    return (current_color_occ & 0x8800000000000000) == 0x8800000000000000? true : false;
}

bool is_castling_short_white_possible(U64 current_color_occ, U64 enemy_occ, U64 att_mask){
    if(att_mask & 0b1110)
        return false;
    if(enemy_occ & 0b0111)
        return false;
    if((current_color_occ & 0b1111) == 0b1001)
        return true;
    return false;
}
bool is_castling_long_white_possible(U64 current_color_occ, U64 enemy_occ,  U64 att_mask){
    if(att_mask & 0x38)
        return false;
    if(enemy_occ & 0xf8)
        return false;
    if((current_color_occ & 0xF8) == 0x88)
        return true;
    return false;
}
bool is_castling_short_black_possible(U64 current_color_occ, U64 enemy_occ, U64 att_mask){
    if(att_mask & 0x0E00000000000000)
        return false;
    if(enemy_occ & 0x0700000000000000)
        return false;
    if((current_color_occ & 0x0F00000000000000) == 0x0900000000000000)
        return true;
    return false;
}
bool is_castling_long_black_possible(U64 current_color_occ,U64 enemy_occ, U64 att_mask){
    if((att_mask  & 0x3800000000000000) != 0)
        return false;
    if(enemy_occ & 0xF800000000000000)
        return false;
    if((current_color_occ & 0xF800000000000000) == 0x8800000000000000)
        return true;
    return false;
}