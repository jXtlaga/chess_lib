#include "../include/chess/converter_position.h"
#include "../include/chess/moves_alg/moves_pieces.h"
#include "../include/chess/moves_alg/move_generation.h"
#include "../include/chess/console_visualization.h"
#include <stdio.h>
#include <assert.h>
#include "../include/chess/algorithms/bits_algorithms.h"
#include "../include/chess/make_move.h"
#include "../include/chess/board_visualisation.h"
#include "../include/chess/mask_alg/attack_mask.h"
#include "../include/chess/algorithms/squares_algorithms.h"
#include "../include/chess/mask_alg/check_mask.h"
#include "../include/chess/search/search_tree.h"
#include <time.h>
#include <stdlib.h>


const char *ascii_art =
        "                                            \n"
        "         ,,                                 \n"
        "       `7MM                                 \n"
        "         MM                                 \n"
        " ,p6\"bo  MMpMMMb.  .gP\"Ya  ,pP\"Ybd ,pP\"Ybd \n"
        "6M'  OO  MM    MM ,M'   Yb 8I   `\" 8I   `\" \n"
        "8M       MM    MM 8M\"\"\"\"\"\" `YMMMa. `YMMMa. \n"
        "YM.    , MM    MM YM.    , L.   I8 L.   I8 \n"
        " YMbmd'.JMML  JMML.`Mbmmd' M9mmmP' M9mmmP' \n"
        "                                            \n"
        "                                            \n";


#include "../include/chess/moves/bishop_attack.h"
#include "../include/chess/moves/rook_attack.h"

int main() {
    printf("%s", ascii_art);
    init_moves();
    Position position = fen_to_position("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
//    print_position_with_info(&position);
    U64 full = -1;
    full ^= pow2_unsigned_64_exponent(11);
    search_depth(4, position);
    init_rook_feature();
    return 0;
}