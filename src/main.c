#include "../include/chess/converter_position.h"
#include "../include/chess/moves_alg/moves_pieces.h"
#include "../include/chess/moves_alg/move_generation.h"
#include "../include/chess/console_visualization.h"
#include <stdio.h>
#include <assert.h>
#include "../include/chess/algorithms/bits_alghorithms.h"
#include "../include/chess/make_move.h"
#include "../include/chess/board_visualisation.h"

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
    int analysis_count[depth_max];
    int analysis_checkmate = 0;
    for(int i = 0; i < depth_max; i++){
        analysis_count[i] = 0;
    }

    //1. set variables for the search
    //1.1 vertices - array of mask_moves for each depth
    Move vertices[depth_max][17];
    //1.2 count vertices - how many non analysed vertices are in the current depth
    int count_vertices[depth_max];
    for (int i = 0; i < depth_max; i++) {
        count_vertices[i] = 0;
    }
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
        else if (last_mask_move.type == EN_PASSANT) {
            new_position = make_move(last_positions[depth_level], last_mask_move.from, last_move_to,
                                     last_mask_move.type);
            vertices[depth_level][ite_ver].to ^= last_move_to;
        }
        else{
            new_position = make_move(last_positions[depth_level], last_mask_move.from, last_move_to,
                                     last_mask_move.type);
            vertices[depth_level][ite_ver].to ^= last_move_to;
        }
        //2.5 play
        //2.6 analyse the new position
        analysis_count[depth_level]++;
        depth_level++;

        if (depth_level == depth_max) {
//            Position history_search[depth_max+1];
//            for(int i = 0; i < depth_level; i++){
//                history_search[i] = last_positions[i];
//            }
//            history_search[depth_max] = new_position;
//            print_search_way(history_search, depth_level+1);
            depth_level--;
            continue;
        }
        last_positions[depth_level] = new_position;
        position_analysis(&new_position, vertices[depth_level], &count_vertices[depth_level]);
        if(count_vertices[depth_level] == 0){
            printf("checkmate\n");
            analysis_checkmate++;
        }

    }

    //vertices[0]
    for(int i = 0; i < depth_max; i++){
        printf("______________________\n");
        printf("depth: %d, vertices count: %d \n", i + 1, analysis_count[i]);
    }
    printf("checkmate count: %d\n", analysis_checkmate);

}
#include <time.h>
int main() {
    printf("%s", ascii_art);
    clock_t start_init, end_init;
    start_init = clock();
    init_moves();
    end_init = clock();

    char* check_moves = "2k5/2n5/2b5/8/8/8/4PN2/3R3K w - - 0 1";
    char *near_checkmate = "2k5/8/8/8/8/7r/6r1/3K4 w - - 0 1";
    char* check_mate = "2k5/8/8/8/8/8/6r1/4K2r w - - 0 1";
    char* castling_non_option = "1k3r2/8/8/8/8/1n6/8/R3K2R w KQk - 0 1";
    char* castling = "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1";
    char* castling_check = "2k1r3/8/8/8/8/8/8/R3K2R w KQk - 0 1";
    char* start = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    Position position = fen_to_position("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    print_position_with_info(&position);
    //position.en_passant_white_sq = 44;
    position.castling_white_rights = 0b11;
    position.castling_black_rights = 0b11;


//    clock_t start_search, end_search;
//    start_search = clock();
    search_depth(5, position);
    Move moves[17];
    int count = 0;
    position_analysis(&position, moves, &count);
//    printf("count: %d\n", count);
////    print_bitboard(moves[0].to);
//    end_search = clock();
    printf("\n");
    printf("Initialization time: %f\n", ((double) (end_init - start_init)) / CLOCKS_PER_SEC);
//    printf("Search time: %f\n", ((double) (end_search - start_search)) / CLOCKS_PER_SEC);
    return 0;
}