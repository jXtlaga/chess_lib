//
// Created by Jan Tlaga on 31/08/2024.
//
#include <stdio.h>
#include "../../include/chess/console_chess.h"
#include "../../include/chess/define_types.h"
#include "../../include/chess/console_visualization.h"
#include "../../include/chess/moves_position.h"
#include "../../include/chess/moves/moves_pieces.h"
#include <string.h>
#include <assert.h>
#define ERROR_64 0xFFFFFFFFFFFFFFFF
int str_to_sq(char* str){
    if(strlen(str) > 2){
        return -1;
    }
    if(str[0] < 'a' || str[0] > 'h'){
        return -1;
    }
    if(str[1] < '1' || str[1] > '8'){
        return -1;
    }
    int file = 7 - (str[0] - 'a');
    int rank = str[1] - '1';
    return rank * 8 + file;
}

U64 get_white_occ(Position position){
    return position.white_pieces.pawn | position.white_pieces.rook | position.white_pieces.knight | position.white_pieces.bishop | position.white_pieces.queen | position.white_pieces.king;
}
U64 get_black_occ(Position position){
    return position.black_pieces.pawn | position.black_pieces.rook | position.black_pieces.knight | position.black_pieces.bishop | position.black_pieces.queen | position.black_pieces.king;
}
typedef struct Move_console{
    U64 *move_start;
    U64 move_possibilities;
} Move_console;
Move_console get_moves_console(Position *position, int sq, COLOR color, U64 current_color_occ, U64 enemy_color_occ, U64 all_occ) {
    U64 sq1 = 1ULL << sq;

    Pieces_position *pieces = (color == WHITE) ? &position->white_pieces : &position->black_pieces;

    if((current_color_occ & sq1) != 0) {
        if (pieces->pawn & sq1) {
            if ((color == WHITE && sq >= 8 && sq < 16) || (color == BLACK && sq >= 48 && sq < 56)) {
                return (Move_console) {&pieces->pawn, color == WHITE ? white_pawn_moves_start(sq, all_occ, current_color_occ, enemy_color_occ) : black_pawn_moves_start(sq, all_occ, current_color_occ, enemy_color_occ)};
            }
            return (Move_console) {&pieces->pawn, color == WHITE ? white_pawn_moves(sq, all_occ, current_color_occ) : black_pawn_moves(sq, all_occ, current_color_occ)};
        }
        if (pieces->rook & sq1) {
            return (Move_console) {&pieces->rook, rook_moves(sq, all_occ, current_color_occ)};
        }
        if (pieces->knight & sq1) {
            return (Move_console) {&pieces->knight, knight_moves(sq, current_color_occ)};
        }
        if (pieces->bishop & sq1) {
            return (Move_console) {&pieces->bishop, bishop_moves(sq, all_occ, current_color_occ)};
        }
        if (pieces->queen & sq1) {
            return (Move_console) {&pieces->queen, queen_moves(sq, all_occ, current_color_occ)};
        }
        if (pieces->king & sq1) {
            return (Move_console) {&pieces->king, king_moves(sq, current_color_occ)};
        }
    }

    return (Move_console) {&position->white_pieces.pawn, ERROR_64};
}


void start_program(){
    init_moves();
    char input[255];
    Position position = create_start_position();
    print_position(&position, 0);
    int selected_sq = -1;
    Move_console move_start_pos = {0, 0};
    while(1){
        U64 current_color_occ = position.move_number & 1U? get_black_occ(position) : get_white_occ(position);
        U64 enemy_color_occ = position.move_number & 1U? get_white_occ(position) : get_black_occ(position);
        U64 all_occ = current_color_occ | enemy_color_occ;
        
        printf("Enter move: ");
        scanf("%s", input);

        int sq = str_to_sq(input);

        if (sq != -1) {
            printf("Sq: %d\n", sq);
            if (selected_sq == -1){
                selected_sq = sq;
                Move_console move_options = get_moves_console(&position, sq,
                                                              position.move_number & 1ULL ? BLACK : WHITE,
                                                              current_color_occ, enemy_color_occ, all_occ);
                if (move_options.move_possibilities == ERROR_64){
                    selected_sq = -1;
                    continue;
                }else {
                    move_start_pos = move_options;
                }
                print_position(&position, move_start_pos.move_possibilities);
            }
            else if(move_start_pos.move_possibilities & (1ULL << sq)){
                *move_start_pos.move_start &= ~(1ULL << selected_sq);
                *move_start_pos.move_start |= (1ULL << sq);
                print_position(&position, 0);
                selected_sq = -1;
                position.move_number ++;
            }
        }

        if(strcmp(input, "exit") == 0){
            break;
        }
    }
}