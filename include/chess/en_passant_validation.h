//
// Created by Jan Tlaga on 9/8/24.
//

#ifndef CHESS_C_LIB_EN_PASSANT_VALIDATION_H
#define CHESS_C_LIB_EN_PASSANT_VALIDATION_H

#include "define_types.h"

U64 white_en_passant(int sq_en_passant, U64 current_pawns_occ);
U64 black_en_passant(int sq_en_passant, U64 current_pawns_occ);
U8 is_en_passant_white_valid(U64 to_move);
U8 is_en_passant_black_valid(U64 to_move);

bool en_passant_pin_taken(U64 hypothetical_occ, int king_sq, U64 enemy_rook_queen_occ);
#endif //CHESS_C_LIB_EN_PASSANT_VALIDATION_H
