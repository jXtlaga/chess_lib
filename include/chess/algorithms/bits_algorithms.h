//
// Created by Jan Tlaga on 29/08/2024.
//

#ifndef CHESS_C_LIB_BITS_ALGORITHMS_H
#define CHESS_C_LIB_BITS_ALGORITHMS_H
#include "../define_types.h"

U32 pow2_unsigned_32_exponent(int exponent);
U64 pow2_unsigned_64_exponent(int exponent);

int get_one_bit_index(U64 b);

U64 get_LS1B(U64 b);
U64 get_MS1B(U64 b);
U64 remove_bits(U64 b, U64 bits);


U64 fill_bits(U32 number_of_bits);
U64 fill_bits_down(U64 n);
U64 fill_bits_up(U64 n);
int getLSBOld(U64 b);

#endif //CHESS_C_LIB_BITS_ALGORITHMS_H
