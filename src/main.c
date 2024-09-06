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

void get_one_move(U64 piece_occ, U64 all_occ, U64 current_occ, U64 pin_mask, U64 check_mask, U64 (*get_moves)(Board_move_info), Move *piece_move, int *ite_moves, TYPE_PIECE type){
    while(piece_occ != 0){
        U64 LS1B = get_LS1B(piece_occ);
        int sq = get_one_bit_index(LS1B);

        Board_move_info info_board = {sq, all_occ, current_occ};
        U64 possible_moves = get_moves(info_board);

        if (pin_mask != 0){
            possible_moves &= pin_mask;
        }
        if (check_mask != 0){
            possible_moves &= check_mask;
        }
        if (possible_moves != 0){
            add_move(piece_move, ite_moves, sq, possible_moves, type);
        }
        piece_occ ^= LS1B;
    }
}

void get_full_moves(U64 piece_occ, U64 all_occ, U64 current_occ, U64 *pin_mask_array, U64 check_mask , U64 (*get_moves_func)(Board_move_info), Move *piece_move, int *ite_moves, TYPE_MOVE type){
    for(int i = 0; i < 4; i++){
        U64 piece_pin_occ = piece_occ & pin_mask_array[i];
        if(piece_pin_occ != 0){
            piece_occ ^= piece_pin_occ;
            get_one_move(piece_pin_occ, all_occ, current_occ, pin_mask_array[i], check_mask, get_moves_func, piece_move, ite_moves, type);
        }
    }
    get_one_move(piece_occ, all_occ, current_occ, 0, check_mask, get_moves_func, piece_move, ite_moves, type);
}



void chess_analysis(Position *position, Move *moves, int *ite_moves){

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

    U64 pin[4] = {0};
    get_pin_rook(king_sq, current_occ, enemy_occ, enemy_rook_queen_occ, pin);

    get_pin_bishop(king_sq, current_occ, enemy_occ, enemy_bishop_queen_occ, &pin[2]);

    U64 full_pin_mask = pin[0] | pin[1] | pin[2] | pin[3];

    U64 att_mask = get_full_enemy_attack_mask(position->black_pieces, all_occ & ~king_sq1, side);

    U64 check_mask = 0;
    if(check_rook != 0 && check_bishop != 0){
        U64 king_move = king_moves(king_sq, current_occ) & ~(check_rook | check_bishop) & ~att_mask;
        add_move(moves, ite_moves, king_sq,king_move, KING_MOVE);
        return;
    }
    else if(check_rook != 0 || check_bishop != 0){
        check_mask = check_rook | check_bishop;
    }
    get_full_moves(current_pieces_position.rook, all_occ, current_occ, pin, check_mask, (U64 (*)(Board_move_info)) rook_moves_struct, moves, ite_moves, ROOK_MOVE);
    get_full_moves(current_pieces_position.knight, all_occ, current_occ, pin, check_mask, (U64 (*)(Board_move_info)) knight_moves_struct, moves, ite_moves, KNIGHT_MOVE);
    get_full_moves(current_pieces_position.bishop, all_occ, current_occ, pin, check_mask, (U64 (*)(Board_move_info)) bishop_moves_struct, moves, ite_moves, BISHOP_MOVE);
    get_full_moves(current_pieces_position.queen, all_occ, current_occ, pin, check_mask, (U64 (*)(Board_move_info)) queen_moves_struct, moves, ite_moves, QUEEN_MOVE);
    get_full_moves(current_pieces_position.king, all_occ, current_occ, pin, check_mask, (U64 (*)(Board_move_info)) king_moves_struct, moves, ite_moves, KING_MOVE);
    U64 start_pawn_line = side == WHITE ? ROW2 : ROW7;
    //get_full_moves(current_pieces_position.pawn, all_occ, current_occ, pin, check_mask, (U64 (*)(Board_move_info)) pawn_moves_struct, moves, ite_moves, PAWN_MOVE);
    get_full_moves(current_pieces_position.pawn & ~start_pawn_line , all_occ, enemy_occ, pin, check_mask,  side == WHITE? (U64 (*)(Board_move_info)) white_pawn_moves_struct: black_pawn_moves_struct, moves, ite_moves, PAWN_MOVE);
    get_full_moves(current_pieces_position.pawn & start_pawn_line, all_occ, enemy_occ, pin, check_mask,  side == WHITE? (U64 (*)(Board_move_info)) white_pawn_moves_start_struct: black_pawn_moves_start_struct, moves, ite_moves, PAWN_MOVE);

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