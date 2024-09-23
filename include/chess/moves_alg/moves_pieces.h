//
// Created by Jan Tlaga on 31/08/2024.
//

#ifndef CHESS_C_LIB_MOVES_PIECES_H
#define CHESS_C_LIB_MOVES_PIECES_H

#include "../define_types.h"

U64 rook_moves(int sq, U64 all_occ, U64 current_color_occ);
U64 bishop_moves(int sq, U64 all_occ, U64 current_color_occ);
U64 queen_moves(int sq, U64 all_occ, U64 current_color_occ);
U64 knight_moves(int sq, U64 current_color_occ);
U64 king_moves(int sq, U64 current_color_occ);

U64 white_pawn_moves_start(int sq, U64 all_occ, U64 enemy_color_occ);
U64 black_pawn_moves_start(int sq, U64 all_occ, U64 enemy_color_occ);
U64 white_pawn_moves(int sq, U64 all_occ, U64 enemy_color_occ);
U64 black_pawn_moves(int sq, U64 all_occ, U64 enemy_color_occ);

U64 rook_moves_struct(Board_move_info *info);
U64 bishop_moves_struct(Board_move_info *info);
U64 queen_moves_struct(Board_move_info *info);
U64 knight_moves_struct(Board_move_info *info);
U64 king_moves_struct(Board_move_info *info);

U64 white_pawn_moves_start_struct(Board_move_info *info);
U64 black_pawn_moves_start_struct(Board_move_info *info);
U64 white_pawn_moves_struct(Board_move_info *info);
U64 black_pawn_moves_struct(Board_move_info *info);

void init_moves();
#endif //CHESS_C_LIB_MOVES_PIECES_H
