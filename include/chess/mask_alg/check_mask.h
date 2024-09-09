//
// Created by Jan Tlaga on 01/09/2024.
//

#ifndef CHESS_C_LIB_CHECK_MASK_H
#define CHESS_C_LIB_CHECK_MASK_H

#include "define_types.h"

U64 get_check_rook(int king_sq, U64 all_occ, U64 rook_queen_occ);
U64 get_check_bishop(int king_sq, U64 all_occ, U64 bishop_queen_occ);

#endif //CHESS_C_LIB_CHECK_MASK_H
