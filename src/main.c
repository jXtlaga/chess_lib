#include <stdio.h>
#include <assert.h>
//#include "../include/chess/bishop_attack.h"
#include "../include/chess/board_visualisation.h"
#include "../include/chess/define_types.h"
#include <math.h>
#include "../include/chess/bits_alghorithms.h"
#include "../include/chess/combinatorics_mask.h"
#include "../include/chess/moves_calculation.h"
#include "../include/chess/squares_alghorithms.h"
#include "../include/chess/moves/bishop_attack.h"
#include "../include/chess/moves/rook_attack.h"
#include "../include/chess/moves/pawn_attack.h"
U64 generate_magic(U64 mask_full, int size){
    U64 mask = mask_full;
    U64 gen_magic = 0;
    for(int i =  size; i > 0; i--){
        U64 LS1B = get_LS1B(mask);
        int index = get_one_bit_index(LS1B);
        mask = mask & ~LS1B;
        gen_magic |= pow2_unsigned_64_exponent(63 - index - i);
    }
    assert(gen_magic >> (63 - size) == 0);
    return gen_magic;
}


int main() {
    init_pawn();
    int sq = 13;
    U64 black_pawn_att = get_white_pawn_attack(sq);
    print_bitboard(black_pawn_att);
    black_pawn_att |= get_white_pawn_move(sq);
    print_bitboard(black_pawn_att);
    return 0;
}