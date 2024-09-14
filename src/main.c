#include "../include/chess/converter_position.h"
#include "../include/chess/moves_alg/moves_pieces.h"
#include "../include/chess/moves_alg/move_generation.h"
#include "../include/chess/console_visualization.h"
#include <stdio.h>
#include <assert.h>
#include "../include/chess/algorithms/bits_alghorithms.h"
#include "../include/chess/make_move.h"
#include "../include/chess/board_visualisation.h"
#include "../include/chess/mask_alg/attack_mask.h"
#include "../include/chess/algorithms/squares_alghorithms.h"
#include "../include/chess/mask_alg/check_mask.h"

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
void print_search_way(Position *position, U64 *attack, int depth_level) {
    printf("\n____________________________________\n");
    print_positions(position, attack, depth_level);
}
int compare_positions(Position *x, Position *y){
    if(x->white_pieces.pawn != y->white_pieces.pawn){
        return 0;
    }
    if(x->white_pieces.rook != y->white_pieces.rook){
        return 0;
    }
    if(x->white_pieces.knight != y->white_pieces.knight){
        return 0;
    }
    if(x->white_pieces.bishop != y->white_pieces.bishop){
        return 0;
    }
    if(x->white_pieces.queen != y->white_pieces.queen){
        return 0;
    }
    if(x->white_pieces.king != y->white_pieces.king){
        return 0;
    }
    if(x->black_pieces.pawn != y->black_pieces.pawn){
        return 0;
    }
    if(x->black_pieces.rook != y->black_pieces.rook){
        return 0;
    }
    if(x->black_pieces.knight != y->black_pieces.knight){
        return 0;
    }
    if(x->black_pieces.bishop != y->black_pieces.bishop){
        return 0;
    }
    if(x->black_pieces.queen != y->black_pieces.queen){
        return 0;
    }
    if(x->black_pieces.king != y->black_pieces.king){
        return 0;
    }
//    if(x->castling_white_rights != y->castling_white_rights){
//        return 0;
//    }
//    if(x->castling_black_rights != y->castling_black_rights){
//        return 0;
//    }
//    if(x->en_passant != y->en_passant){
//        return 0;
//    }

    return 1;

}
Position analysis_position[97862] = {0};
void search_depth_analysis(int depth_max, Position position) {
    int analysis_count[depth_max];
    int analysis_checkmate[depth_max];
    U64 analysis_last_to[depth_max];

    int analysis_being[97862] = {0};
    for(int i = 0; i < depth_max; i++){
        analysis_count[i] = 0;
        analysis_checkmate[i] = 0;
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
        analysis_last_to[depth_level] = last_move_to;
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
        analysis_count[depth_level]++;
        depth_level++;

        if (depth_level == depth_max) {

            int found = 0;
            for(int i = 0; i < 97862; i++){
                if(compare_positions(&new_position, &analysis_position[i])){
                    if (analysis_being[i] == 0) {
                        analysis_being[i] = 1;
                        found = 1;
                        break;
                    }
//                    else {
//                        printf("duplicate: %d\n", i);
//                        print_position_with_info(&new_position);
//                        print_position_with_info(&analysis_position[i]);
//                    }
                }

            }
            if (found == 0) {
                printf("not found\n");
                Position history_search[depth_max + 1];
                for (int i = 0; i < depth_level; i++) {
                    history_search[i] = last_positions[i];
                }
                history_search[depth_max] = new_position;
                print_search_way(history_search, analysis_last_to, depth_level + 1);
                print_position_with_info(&new_position);

            }
            depth_level--;
            continue;
        }
        last_positions[depth_level] = new_position;
        position_analysis(&new_position, vertices[depth_level], &count_vertices[depth_level]);
        if(count_vertices[depth_level] == 0){
            analysis_checkmate[depth_level] ++;
        }


    }

    //vertices[0]
    for(int i = 0; i < depth_max; i++){
        printf("______________________\n");
        printf("depth: %d, vertices count: %d \n", i + 1, analysis_count[i]);
        printf("checkmate: %d\n", analysis_checkmate[i]);
    }
    for(int i = 0; i < 97862; i++){
        if(analysis_being[i] == 0){
            printf("Not being: %d\t", i);
        }
    }

}

void search_depth(int depth_max, Position position) {
    int analysis_count[depth_max];
    int analysis_checkmate[depth_max];
    int analysis_check[depth_max];
    int analysis_promotion[depth_max];
    int analysis_en_passant[depth_max];
    int analysis_castling[depth_max];
    U64 analysis_last_to[depth_max];

    for(int i = 0; i < depth_max; i++){
        analysis_count[i] = 0;
        analysis_checkmate[i] = 0;
        analysis_check[i] = 0;
        analysis_promotion[i] = 0;
        analysis_en_passant[i] = 0;
        analysis_castling[i] = 0;
    }
    //706045930 - 897 = 706 045 033
    //

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
        analysis_last_to[depth_level] = last_move_to;
        if (last_move_to == 0) {
            count_vertices[depth_level]--;
            continue;
        }
        Position new_position;
        //2.4 if the last mask_move is a pawn move, check if it is a promotion
        if (last_mask_move.type == PAWN_MOVE && (last_move_to & 0xFF000000000000FF)) {
            //2.4.1 create 3 new mask_moves (promotion to rook, bishop, knight)

            vertices[depth_level][ite_ver] = (Move) {last_mask_move.from, last_mask_move.to, PROMOTION_BISHOP};
            vertices[depth_level][ite_ver + 1] = (Move) {last_mask_move.from, last_mask_move.to, PROMOTION_KNIGHT};
            vertices[depth_level][ite_ver + 2] = (Move) {last_mask_move.from, last_mask_move.to, PROMOTION_ROOK};
            vertices[depth_level][ite_ver + 3] = (Move) {last_mask_move.from, last_mask_move.to ^ last_move_to, PROMOTION_QUEEN};
            count_vertices[depth_level] += 3;
            //2.4.2 play the promotion to queen
            new_position = make_move(last_positions[depth_level], last_mask_move.from, last_move_to,
                                     PROMOTION_QUEEN);
            analysis_promotion[depth_level]++;
        }
        else{
            new_position = make_move(last_positions[depth_level], last_mask_move.from, last_move_to,
                                     last_mask_move.type);
            vertices[depth_level][ite_ver].to ^= last_move_to;
        }
        if(last_mask_move.type == PROMOTION_QUEEN || last_mask_move.type == PROMOTION_ROOK || last_mask_move.type == PROMOTION_BISHOP || last_mask_move.type == PROMOTION_KNIGHT){
            analysis_promotion[depth_level]++;
        }
        if(last_mask_move.type == EN_PASSANT){
            analysis_en_passant[depth_level]++;

        }
        if(last_mask_move.type == CASTLING){
            analysis_castling[depth_level]++;
        }
        //2.5 play
        //2.6 analyse the new position
        analysis_count[depth_level]++;
        depth_level++;
        if (depth_level == depth_max) {
            depth_level--;
            continue;
        }
        last_positions[depth_level] = new_position;
        position_analysis(&new_position, vertices[depth_level], &count_vertices[depth_level]);
        if(count_vertices[depth_level] == 0){
            analysis_checkmate[depth_level - 1] ++;
        }
    }

    for(int i = 0; i < depth_max; i++){
        printf("______________________\n");
        printf("depth: %d, vertices count: %d \n", i + 1, analysis_count[i]);
        printf("checkmate: %d\n", analysis_checkmate[i]);
        printf("check: %d\n", analysis_check[i]);
        printf("promotion: %d\n", analysis_promotion[i]);
        printf("en passant: %d\n", analysis_en_passant[i]);
        printf("castling: %d\n", analysis_castling[i]);
    }


}




#include <time.h>
#include <stdlib.h>

int main() {
    printf("%s", ascii_art);
    init_moves();
    Position position = fen_to_position("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    print_position_with_info(&position);
    print_position(&position, 0);

    search_depth(6, position);
    Move moves[17] = {0};
    return 0;
}