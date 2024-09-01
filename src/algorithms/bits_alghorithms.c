//
// Created by Jan Tlaga on 29/08/2024.
//

#include "../../include/chess/bits_alghorithms.h"
int LSB_LOOKUP[] = {63,0,58,1,59,47,53,2,60,39,48,27,54,33,42,3,61,51,37,40,49,18,28,20,55,30,34,11,43,14,22,4,62,57,46,52,38,26,32,41,50,36,17,19,29,10,13,21,56,45,25,31,35,16,9,12,44,24,15,8,23,7,6,5};

U32 pow2_unsigned_32_exponent(int exponent){
    return 1U << exponent;
}
U64 pow2_unsigned_64_exponent(int exponent){
    return 1ULL << exponent;
}
int get_one_bit_index(U64 b){
    int index = (b * 0x07EDD5E59A4E28C2) >> 58;
    return LSB_LOOKUP[index];
}

U64 get_LS1B(U64 b){
    return b & -b;
}
U64 get_MS1B(U64 b){
    b |= b >> 32;
    b |= b >> 16;
    b |= b >> 8;
    b |= b >> 4;
    b |= b >> 2;
    b |= b >> 1;
    return b - (b >> 1);
}
U64 remove_bits(U64 b, U64 b_to_remove){
    return b & ~b_to_remove;
}

int getLSBOld(U64 b)
{
    int n = 1;
    if ((b & 0x00000000FFFFFFFF) == 0) {n = n +32; b = b >>32;}
    if ((b & 0x0000FFFF) == 0) {n = n +16; b = b >>16;}
    if ((b & 0x000000FF) == 0) {n = n + 8; b = b >> 8;}
    if ((b & 0x0000000F) == 0) {n = n + 4; b = b >> 4;}
    if ((b & 0x00000003) == 0) {n = n + 2; b = b >> 2;}
    return n - (b & 1);
}

U64 fill_bits(U32 number_of_bits) {
    if (number_of_bits >= 64) {
        return 0xFFFFFFFFFFFFFFFF;
    }
    return pow2_unsigned_64_exponent(number_of_bits) - 1;
}
U64 fill_bits_down(U64 n){
    U32 number_of_bits = get_one_bit_index(get_LS1B(n));
    return fill_bits(number_of_bits);
}
U64 fill_bits_up(U64 n){
    U64 LS1B = get_LS1B(n);
    U32 number_of_bits = get_one_bit_index(LS1B);
    return ~fill_bits(number_of_bits) & ~LS1B;
}


