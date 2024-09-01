#include <stdio.h>
#include <assert.h>
//#include "../include/chess/bishop_attack.h"
#include "../include/chess/board_visualisation.h"
#include "../include/chess/define_types.h"
#include <math.h>
#include "../include/chess/bits_alghorithms.h"
#include "../include/chess/moves/moves_pieces.h"
#include "../include/chess/moves_position.h"
#include "../include/chess/console_visualization.h"
#include "../include/chess/console_chess.h"
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
    U64 test = white_pawn_moves_start(8, 0, 0, 0);

    print_bitboard(test);

    print_bitboard(get_white_pawn_move(8));
    start_program();
    return 0;
}