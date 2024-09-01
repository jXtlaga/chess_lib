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
typedef enum COLOR{
    WHITE,
    BLACK
} COLOR;
U64 get_white_occ(Position position){
    return position.white_pawns | position.white_rooks | position.white_knights | position.white_bishops | position.white_queens | position.white_king;
}
U64 get_black_occ(Position position){
    return position.black_pawns | position.black_rooks | position.black_knights | position.black_bishops | position.black_queens | position.black_king;
}
typedef struct Move{
    U64 *move_start;
    U64 move_possibilities;
} Move;
Move get_moves(Position *position, int sq, COLOR color, U64 current_color_occ, U64 enemy_color_occ, U64 all_occ){
    U64 sq1 = 1ULL << sq;
    if(color == WHITE && (current_color_occ & sq1) != 0) {
        if (position->white_pawns & sq1) {
            if (sq >= 8 && sq < 16) {
                return (Move) {&position->white_pawns, white_pawn_moves_start(sq, all_occ, current_color_occ, enemy_color_occ)};
            };
            return (Move) {&position->white_pawns, white_pawn_moves(sq, all_occ, current_color_occ)};
        }
        if (position->white_rooks & sq1) {
            return (Move) {&position->white_rooks, rook_moves(sq, all_occ, current_color_occ)};
        }
        if (position->white_knights & sq1) {
            return (Move) {&position->white_knights, knight_moves(sq, current_color_occ)};
        }
        if (position->white_bishops & sq1) {
            return (Move) {&position->white_bishops, bishop_moves(sq, all_occ, current_color_occ)};
        }
        if (position->white_queens & sq1) {
            return (Move) {&position->white_queens, queen_moves(sq, all_occ, current_color_occ)};
        }
        if (position->white_king & sq1) {
            return (Move) {&position->white_king, king_moves(sq, current_color_occ)};
        }
    }
    if(color == BLACK && (current_color_occ & sq1) != 0) {
        if (position->black_pawns & sq1) {
            if (sq >= 48 && sq < 56) {
                return (Move) {&position->black_pawns, black_pawn_moves_start(sq, all_occ, current_color_occ, enemy_color_occ)};
            };
            return (Move) {&position->black_pawns, black_pawn_moves(sq, all_occ, current_color_occ)};
        }
        if (position->black_rooks & sq1) {
            return (Move) {&position->black_rooks, rook_moves(sq, all_occ, current_color_occ)};
        }
        if (position->black_knights & sq1) {
            return (Move) {&position->black_knights, knight_moves(sq, current_color_occ)};
        }
        if (position->black_bishops & sq1) {
            return (Move) {&position->black_bishops, bishop_moves(sq, all_occ, current_color_occ)};
        }
        if (position->black_queens & sq1) {
            return (Move) {&position->black_queens, queen_moves(sq, all_occ, current_color_occ)};
        }
        if (position->black_king & sq1) {
            return (Move) {&position->black_king, king_moves(sq, current_color_occ)};
        }
    }

    return (Move) {&position->white_pawns, ERROR_64};
}

void start_program(){
    init_moves();
    char input[255];
    Position main_position = create_start_position();
    print_position(main_position, 0);
    int selected_sq = -1;
    Move move_start_pos = {0, 0};
    while(1){
        U64 current_color_occ = main_position.moveNumber & 1U? get_black_occ(main_position) : get_white_occ(main_position);
        U64 enemy_color_occ = main_position.moveNumber & 1U? get_white_occ(main_position) : get_black_occ(main_position);
        U64 all_occ = current_color_occ | enemy_color_occ;
        
        printf("Enter move: ");
        scanf("%s", input);

        int sq = str_to_sq(input);

        if (sq != -1) {
            printf("Sq: %d\n", sq);
            if (selected_sq == -1){
                selected_sq = sq;
                Move move_options = get_moves(&main_position, sq, main_position.moveNumber & 1ULL? BLACK : WHITE, current_color_occ, enemy_color_occ, all_occ);
                if (move_options.move_possibilities == ERROR_64){
                    selected_sq = -1;
                    continue;
                }else {
                    move_start_pos = move_options;
                }
                print_position(main_position, move_start_pos.move_possibilities);
            }
            else if(move_start_pos.move_possibilities & (1ULL << sq)){
                *move_start_pos.move_start &= ~(1ULL << selected_sq);
                *move_start_pos.move_start |= (1ULL << sq);
                print_position(main_position, 0);
                selected_sq = -1;
                main_position.moveNumber ++;
            }
        }

        if(strcmp(input, "exit") == 0){
            break;
        }
    }
}