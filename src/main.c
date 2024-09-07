#include <stdio.h>
#include <assert.h>
//#include "../include/chess/bishop_attack.h"
#include "../include/chess/board_visualisation.h"
#include "../include/chess/define_types.h"
#include <math.h>
#include <stdlib.h>
#include "../include/chess/bits_alghorithms.h"
#include "../include/chess/moves/moves_pieces.h"
#include "../include/chess/moves_position.h"
#include "../include/chess/console_visualization.h"
#include "../include/chess/console_chess.h"
#include "../include/chess/moves/pawn_attack.h"
#include "../include/chess/moves/rook_attack.h"
#include "../include/chess/combinatorics_mask.h"
#include "../include/chess/check_mask.h"
#include "../include/chess/converter_position.h"
#include "../include/chess/pin_mask.h"
#include "../include/chess/attack_mask.h"
U64 get_occ(Pieces_position *position){
    return position->rook | position->bishop | position->queen | position->knight | position->pawn | position->king;
}
void add_move(Move *piece_move, int *ite_moves, int from, U64 to, TYPE_MOVE type){
    piece_move[*ite_moves].from = from;
    piece_move[*ite_moves].to = to;
    piece_move[*ite_moves].type = type;
    (*ite_moves)++;
}

void get_move_pin_check(U64 piece_occ, U64 all_occ, U64 current_occ, U64 pin_mask, U64 (*get_moves)(Board_move_info), Move *piece_move, int *ite_moves, TYPE_PIECE type){
    while(piece_occ != 0){
        U64 LS1B = get_LS1B(piece_occ);
        int sq = get_one_bit_index(LS1B);

        Board_move_info info_board = {sq, all_occ, current_occ};
        U64 possible_moves = get_moves(info_board);

        if (pin_mask != 0){
            possible_moves &= pin_mask;
        }
        if (possible_moves != 0){
            add_move(piece_move, ite_moves, sq, possible_moves, type);
        }
        piece_occ ^= LS1B;
    }
}
void get_one_move(U64 piece_occ, U64 all_occ, U64 current_occ, U64 (*get_moves)(Board_move_info), Move *piece_move, int *ite_moves, TYPE_PIECE type){
    while(piece_occ != 0){
        U64 LS1B = get_LS1B(piece_occ);
        int sq = get_one_bit_index(LS1B);

        Board_move_info info_board = {sq, all_occ, current_occ};
        U64 possible_moves = get_moves(info_board);

        if (possible_moves != 0){
            add_move(piece_move, ite_moves, sq, possible_moves, type);
        }
        piece_occ ^= LS1B;
    }
}
typedef struct Position_main_data{
    COLOR side;
    U64 current_occ;
    U64 all_occ;
    Pieces_position current_pieces_position;
    int king_sq;
    U64 check_rook;
    U64 check_bishop;
    U64 att_mask;
    U64 check_mask;
    U64 sum_pin_mask;
} Position_main_data;

Position_main_data init_main_data(Position *position, U64 *pin_mask){
    COLOR side = (position->move_number & 1) == 0 ? WHITE : BLACK;
    U64 current_occ = side == WHITE ? get_occ(&position->white_pieces) : get_occ(&position->black_pieces);
    U64 enemy_occ = side == WHITE ? get_occ(&position->black_pieces) : get_occ(&position->white_pieces);
    U64 all_occ = current_occ | enemy_occ;

    Pieces_position current_pieces_position = side == WHITE ? position->white_pieces : position->black_pieces;
    Pieces_position enemy_pieces_position = side == WHITE ? position->black_pieces : position->white_pieces;

    U64 enemy_rook_queen_occ = enemy_pieces_position.rook | enemy_pieces_position.queen;
    U64 enemy_bishop_queen_occ = enemy_pieces_position.bishop | enemy_pieces_position.queen;

    U64 king_sq1 = get_LS1B(current_pieces_position.king);
    int king_sq = get_one_bit_index(king_sq1);

    U64 check_rook = get_check_rook(king_sq, current_occ, enemy_occ);
    U64 check_bishop = get_check_bishop(king_sq, current_occ, enemy_occ);

    get_pin_rook(king_sq, current_occ, enemy_occ, enemy_rook_queen_occ, &pin_mask[0]);
    get_pin_bishop(king_sq, current_occ, enemy_occ, enemy_bishop_queen_occ, &pin_mask[2]);

    U64 sum_pin_mask = pin_mask[0] | pin_mask[1] | pin_mask[2] | pin_mask[3];

    U64 att_mask = get_full_enemy_attack_mask(enemy_pieces_position, all_occ & ~king_sq1, side);

    return (Position_main_data){side, current_occ, all_occ,
                                current_pieces_position, king_sq,
                                check_rook, check_bishop, att_mask, 0,
                                sum_pin_mask
        };
}

void double_check_analysis(Position_main_data *pos_info, Move *moves, int *ite_moves){
    U64 king_move = king_moves(pos_info->king_sq, pos_info->current_occ) & ~(pos_info->check_rook | pos_info->check_bishop) & ~pos_info->att_mask;
    add_move(moves, ite_moves, pos_info->king_sq,king_move, KING_MOVE);
}

void check_analysis(Position_main_data *pos_info, Move *moves, int *ite_moves, const U64 *position_occ_pieces, get_moves_func *moves_struct){
    pos_info->check_mask = pos_info->check_rook? pos_info->check_rook : pos_info->check_bishop;
    for(int i = 0; i < 7; i++){
        U64 pin_check_mask = pos_info->sum_pin_mask? position_occ_pieces[i] & ~pos_info->sum_pin_mask & pos_info->check_mask: position_occ_pieces[i] & pos_info->check_mask;
        U64 occ_without_pin = pos_info->sum_pin_mask?  position_occ_pieces[i] & ~pos_info->sum_pin_mask :  position_occ_pieces[i];
        get_move_pin_check(occ_without_pin, pos_info->all_occ, pos_info->current_occ, pin_check_mask,  (U64 (*)(Board_move_info))moves_struct[i], moves, ite_moves, i);
    }
}

void without_check_analysis(Position_main_data *pos_info, Move *moves, int *ite_moves, const U64 *position_occ_pieces, get_moves_func *moves_struct){
    for(int i = 0; i < 7; i++){
        U64 occ_without_pin = pos_info->sum_pin_mask? position_occ_pieces[i] & ~pos_info->sum_pin_mask : position_occ_pieces[i];
        get_one_move(occ_without_pin, pos_info->all_occ, pos_info->current_occ,  (U64 (*)(Board_move_info)) moves_struct[i], moves, ite_moves, i);
    }
}

void pin_analysis(Position_main_data *pos_info, Move *moves, int *ite_moves, const U64 *position_occ_pieces, get_moves_func *moves_struct, U64 pin_mask){
    for(int pos_i = 0; pos_i < 7; pos_i++){
        U64 position_occ_piece_pin = position_occ_pieces[pos_i] & pin_mask;
        if(position_occ_piece_pin != 0){
            get_move_pin_check(position_occ_piece_pin, pos_info->all_occ, pos_info->current_occ, pin_mask, (U64 (*)(Board_move_info)) moves_struct[pos_i], moves, ite_moves, pos_i);
        }
    }
}
void chess_analysis(Position *position, Move *moves, int *ite_moves){
    U64 pin_mask[4] = {0};
    Position_main_data pos_info = init_main_data(position, pin_mask);

    U64 position_pawn_start = pos_info.side == WHITE ? ROW2 & pos_info.current_pieces_position.pawn: ROW7 & pos_info.current_pieces_position.pawn;
    U64 position_pawn_after_start = pos_info.current_pieces_position.pawn ^ position_pawn_start;
    U64 position_occ_pieces[7] = {pos_info.current_pieces_position.rook, pos_info.current_pieces_position.knight, pos_info.current_pieces_position.bishop, pos_info.current_pieces_position.queen, pos_info.current_pieces_position.king, position_pawn_start, position_pawn_after_start};
    get_moves_func moves_struct[7] = {rook_moves_struct, knight_moves_struct, bishop_moves_struct, queen_moves_struct, king_moves_struct, pos_info.side == WHITE? white_pawn_moves_start_struct : black_pawn_moves_start_struct, pos_info.side == WHITE? white_pawn_moves_struct : black_pawn_moves_struct};


    //1. moves analysis depends on double check, check and without check
    //1.1 double check
    if(pos_info.check_rook != 0 && pos_info.check_bishop != 0){
        double_check_analysis(&pos_info, moves, ite_moves);
        return;
    }
    //1.2 check
    else if(pos_info.check_rook != 0 || pos_info.check_bishop != 0){
        //1.2.1 get moves with check_mask and without pin pieces
        check_analysis(&pos_info, moves, ite_moves, position_occ_pieces, moves_struct);
        //1.2.2 create pin_mask & check_mask - *if there is a check pin piece can move only on pin_mask and check_mask
        for(int i = 0; i < 4; i++){
            pin_mask[i] &= pos_info.check_mask;
        }
    }
    //1.3 without check
    else {
        //1.3.1 get moves without check_mask and without pin pieces
        without_check_analysis(&pos_info, moves, ite_moves, position_occ_pieces, moves_struct);
    }

    //2. moves analysis depends on pin pieces
    //2.1 is there any pin piece
   if(pos_info.sum_pin_mask != 0){
       for (int pin_i = 0; pin_i < 4; pin_i++){
           //2.1 not pin piece
           if (pin_mask[pin_i] == 0)
                continue;
            //2.2 get moves with pin piece
           pin_analysis(&pos_info, moves, ite_moves, position_occ_pieces, moves_struct, pin_mask[pin_i]);
       }
   }
}


int main() {
    init_moves();
    char* start = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    char* double_check = "8/3k4/8/5b2/8/8/2K2r2/8 w - - 0 1";
    char* pin_pos = "rnb1kbnr/pppp1ppp/8/8/3Pp2q/5P2/PPP1PBPP/RN1QKBNR w KQkq - 0 5";
    Position position = fen_to_position(pin_pos);
    position.move_number = 0;
    print_position(&position, 0);
    Move moves[16] = {0};
    int ite_moves = 0;
    chess_analysis(&position, moves, &ite_moves);
    for(int i = 0; i < ite_moves; i++){
        print_type_move(moves[i].type);
        print_bitboard(moves[i].to);
    }
    return 0;
}