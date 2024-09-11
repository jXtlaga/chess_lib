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
#include <assert.h>
#include "../include/chess/algorithms/bits_alghorithms.h"
#include "../include/chess/make_move.h"

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

void print_search_vertices(Position position, int depth_level) {
    printf("________________\n");
    printf("Depth level: %d\n", depth_level);
    print_position(&position, 0);
}
void print_search_way(Position *position, int depth_level) {
    printf("\n____________________________________\n");
    U64 attack[depth_level];
    for(int i = 0; i < depth_level; i++){
        attack[i] = 0;
    }
    print_positions(position, attack, depth_level);
}

void search_depth(int depth_max, Position position) {
    int index_depth3 = 0;
    //1. set variables for the search
    //1.1 vertices - array of mask_moves for each depth
    Move vertices[depth_max][17];
    //1.2 count vertices - how many non analysed vertices are in the current depth
    int count_vertices[depth_max];
    count_vertices[0] = 0;
    //1.3 depth_level - current depth level
    int depth_level = 0;
    //1.4 set the thirst mask_moves after input position
    position_analysis(&position, vertices[depth_level], &count_vertices[depth_level]);
    //1.5
    Position last_positions[depth_max];
    last_positions[0] = position;
    //2. creating the depth search
    while (depth_level >= 0) {
        //2.1 if there are no vertices in the current depth level, go to the previous level
        if (count_vertices[depth_level] == 0) {
            depth_level--;
            continue;
        }
        assert(count_vertices[depth_level] > 0);
        //2.3 take the last mask_move
        if(index_depth3 == 37){
            printf("stop");
        }
        int ite_ver = count_vertices[depth_level] - 1;
        Move last_mask_move = vertices[depth_level][ite_ver];
        U64 last_move_to = get_LS1B(last_mask_move.to);
        if (last_move_to == 0) {
            count_vertices[depth_level]--;
            continue;
        }
        Position new_position;
        //2.4 if the last mask_move is a pawn move, check if it is a promotion
        if (last_mask_move.type == PAWN_MOVE && (last_move_to & 0xFF000000000000FF)) {
            //2.4.1 create 3 new mask_moves (promotion to rook, bishop, knight)
            vertices[depth_level][ite_ver] = (Move) {last_mask_move.from, last_move_to, PROMOTION_BISHOP};
            vertices[depth_level][ite_ver + 1] = (Move) {last_mask_move.from, last_move_to, PROMOTION_KNIGHT};
            vertices[depth_level][ite_ver + 2] = (Move) {last_mask_move.from, last_move_to, PROMOTION_ROOK};
            count_vertices[depth_level] += 3;
            //2.4.2 play the promotion to queen
            new_position = make_move(last_positions[depth_level], last_mask_move.from, last_move_to,
                                              PROMOTION_QUEEN);

        }
        else{
            new_position = make_move(last_positions[depth_level], last_mask_move.from, last_move_to,
                                     last_mask_move.type);
            vertices[depth_level][ite_ver].to ^= last_move_to;
        }
        //2.5 play
        //2.6 analyse the new position
        depth_level++;
        print_search_vertices(new_position, depth_level);
        print_search_way(last_positions, depth_level);
        printf("Count vertices: %d\n", count_vertices[depth_level]);
        Position history_search[depth_max+1];
        for(int i = 0; i < depth_level; i++){
            history_search[i] = last_positions[i];
        }
        history_search[depth_max] = new_position;
        if (depth_level == depth_max) {
            index_depth3++;
            printf("\n\n_______________________________\nDepth 3: %d\n", index_depth3);
            print_search_way(history_search, depth_level+1);
            depth_level--;
            continue;
        }
        last_positions[depth_level] = new_position;
        position_analysis(&new_position, vertices[depth_level], &count_vertices[depth_level]);
    }

    //vertices[0]

}

int main() {
    printf("%s", ascii_art);
    init_moves();

    char *start = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    char *double_check = "8/3k4/8/5b2/8/8/2K2r2/8 w - - 0 1";
    char *en_passant_pos = "1k6/8/8/3pP3/8/8/1K6/8 w - d6 0 2";
    char *en_passant_and_pin = "1k6/2b5/8/3pP3/8/8/7K/8 w - - 0 1";
    char *pin_pos = "rnb1kbnr/pppp1ppp/8/8/3Pp2q/5P2/PPP1PBPP/RN1QKBNR w KQkq - 0 5";
    char *castling_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1";
    char *castling_2 = "r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1";
    char *pawn_end_game = "8/3P1k2/8/2K5/8/8/7r/4R3 b - - 0 1";
    char *end_game = "5k2/1P3p2/8/K7/8/8/8/8 b - - 0 1";
    char *end_game2 = "1R3k2/5p2/8/K7/8/8/8/8 b - - 0 1";
    Position position = fen_to_position(end_game2);

    position.move_number = 1;
    //position.en_passant_white_sq = 44;
    position.castling_white_rights = 0b11;
    position.castling_black_rights = 0b11;

    Move moves[16] = {0};
    int ite_moves = 0;


    search_depth(4, position);

    return 0;
}