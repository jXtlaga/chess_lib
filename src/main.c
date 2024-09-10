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
#include "../include/chess/algorithms/bits_alghorithms.h"
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


void search_depth(int depth_max, Position position){
    U64 count_vertices[depth_max];
    Move vertices[depth_max][16];
    int depth_level = 0;
    count_vertices[depth_level] = 0;
    position_analysis(&position, vertices[depth_level], &count_vertices[depth_level]);

    while(depth_level >= 0){
        if(count_vertices[depth_level] == 0){
            depth_level--;
            continue;
        }
        if(depth_level == depth_max){
            depth_level--;
            continue;
        }
        count_vertices[depth_level] --;
        int ite_ver = count_vertices[depth_level];
        //Position position_new = make_move(vertices[depth_level][ite_ver], vertices[depth_level][count_vertices[depth_level] - 1].from, vertices[depth_level][count_vertices[depth_level] - 1].to, vertices[depth_level][count_vertices[depth_level] - 1].type);
        count_vertices[depth_level] = 0;
        //osition_analysis(&position_new, vertices[depth_level], &count_vertices[depth_level]);
        depth_level++;
    }

    //vertices[0]

}
int main() {
    printf("%s", ascii_art);
    init_moves();

    char* start = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    char* double_check = "8/3k4/8/5b2/8/8/2K2r2/8 w - - 0 1";
    char* en_passant_pos = "1k6/8/8/3pP3/8/8/1K6/8 w - d6 0 2";
    char *en_passant_and_pin = "1k6/2b5/8/3pP3/8/8/7K/8 w - - 0 1";
    char* pin_pos = "rnb1kbnr/pppp1ppp/8/8/3Pp2q/5P2/PPP1PBPP/RN1QKBNR w KQkq - 0 5";
    char* castling_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1";
    char* castling_2 = "r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1";
    char* pawn_end_game = "8/3P1k2/8/2K5/8/8/7r/4R3 b - - 0 1";
    Position position = fen_to_position(start);

    position.move_number = 0;
    //position.en_passant_white_sq = 44;
    position.castling_white_rights = 0b11;
    position.castling_black_rights = 0b11;

    print_position(&position, 0);
    Move moves[16] = {0};
    int ite_moves = 0;
    position_analysis(&position, moves, &ite_moves);

//    for(int i = 0; i < ite_moves; i++){
//        print_type_move(moves[i].type);
//        print_bitboard(moves[i].to);
//    }
    for(int i = 0; i < ite_moves; i++) {
        U8 from_new = moves[i].from;
        U64 to_new = moves[i].to;
        TYPE_MOVE type_new = moves[i].type;
        if (type_new == CASTLING) {
            if (to_new & castling_short_right){
                Position new_position = make_move(position, from_new, castling_short_right, moves[i].type);
                print_position(&new_position, 0);
            }
            if (to_new & castling_long_right){
                Position new_position = make_move(position, from_new, castling_long_right, moves[i].type);
                print_position(&new_position, 0);
            }
        }
        else if(type_new == PAWN_MOVE){
            COLOR color = position.move_number & 1 ? BLACK : WHITE;

        }
        if (moves[i].type != CASTLING) {
            while(to_new != 0){
                U64 to_LSB = get_LS1B(to_new);
                to_new ^= to_LSB;
                Position new_position = make_move(position, from_new, to_LSB, moves[i].type);
                print_position(&new_position, 0);
            }
        }
    }
    //Position Position_new = make_move(position, from, to, type_movement);

    return 0;
}