#include "../include/chess/converter_position.h"
#include "../include/chess/moves_alg/moves_position.h"
#include "../include/chess/moves_alg/moves_pieces.h"
#include "../include/chess/moves_alg/move_generation.h"
#include "../include/chess/moves_alg/moves_pieces.h"
#include "../include/chess/moves_alg/moves_position.h"
#include "../include/chess/moves_alg/move_generation.h"
#include "../include/chess/mask_alg/attack_mask.h"
#include "../include/chess/mask_alg/check_mask.h"
#include "../include/chess/board_visualisation.h"
#include "../include/chess/console_visualization.h"
#include <stdio.h>
#include "../include/chess/make_move.h"
const char* ascii_art =
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



int main() {
    printf("%s", ascii_art);
    init_moves();

    char* start = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    char* double_check = "8/3k4/8/5b2/8/8/2K2r2/8 w - - 0 1";
    char* en_passant_pos = "1k6/8/8/3pP3/8/8/1K6/8 w - d6 0 2";
    char *en_passant_and_pin = "1k6/2b5/8/3pP3/8/8/7K/8 w - - 0 1";
    char* pin_pos = "rnb1kbnr/pppp1ppp/8/8/3Pp2q/5P2/PPP1PBPP/RN1QKBNR w KQkq - 0 5";
    char* castling_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1";
    char* castling_2 = "rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1";
    Position position = fen_to_position(castling_2);

    position.move_number = 0;
    //position.en_passant_white_sq = 44;
    position.castling_white_rights = 0b11;
    position.castling_black_rights = 0b11;
    print_position(&position, 0);
    Move moves[16] = {0};
    int ite_moves = 0;
    position_analysis(&position, moves, &ite_moves);
    for(int i = 0; i < ite_moves; i++){
        print_type_move(moves[i].type);
        print_bitboard(moves[i].to);
    }
    Position Position_new = make_move(position, 8, 0x10000, PAWN_MOVE_START);
    print_position(&Position_new, 0);
    position_analysis(&Position_new, moves, &ite_moves);
    for(int i = 0; i < ite_moves; i++){
        print_type_move(moves[i].type);
        print_bitboard(moves[i].to);
    }
    return 0;
}