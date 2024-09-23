#include <printf.h>
#include <assert.h>
#include "../../include/chess/moves_alg/move_generation.h"
#include "../../include/chess/moves_alg/moves_pieces.h"
#include "../../include/chess/mask_alg/attack_mask.h"
#include "../../include/chess/algorithms/bits_algorithms.h"
#include "../../include/chess/mask_alg/check_mask.h"
#include "../../include/chess/mask_alg/pin_mask.h"
#include "../../include/chess/algorithms/squares_algorithms.h"
#include "../../include/chess/en_passant_validation.h"
#include "../../include/chess/castling_validation.h"

void add_move(Move *piece_move, int *ite_moves, int from, U64 to, TYPE_MOVE type) {
    if(to == 0) return;
    piece_move[*ite_moves].from = from;
    piece_move[*ite_moves].to = to;
    piece_move[*ite_moves].type = type;
    (*ite_moves)++;
}

void get_move_pin_check(U64 piece_occ, U64 all_occ, U64 current_occ, U64 enemy_occ, U64 pin_mask, U64 (*get_moves)(Board_move_info),
                        Move *piece_move, int *ite_moves, TYPE_MOVE type) {
    while (piece_occ != 0) {
        U64 LS1B = get_LS1B(piece_occ);
        int sq = get_one_bit_index(LS1B);

        Board_move_info info_board = {sq, all_occ, current_occ, enemy_occ};
        U64 possible_moves = get_moves(info_board);
        if (pin_mask != 0) {
            possible_moves &= pin_mask;
        }
        if (possible_moves != 0) {
            add_move(piece_move, ite_moves, sq, possible_moves, type);
        }
        piece_occ ^= LS1B;
    }
}

void get_one_move(U64 piece_occ, U64 all_occ, U64 current_occ, U64 enemy_occ, U64 (*get_moves)(Board_move_info), Move *piece_move,
                  int *ite_moves, TYPE_MOVE type) {
    while (piece_occ != 0) {
        U64 LS1B = get_LS1B(piece_occ);
        int sq = get_one_bit_index(LS1B);

        Board_move_info info_board = {sq, all_occ, current_occ, enemy_occ};
        U64 possible_moves = get_moves(info_board);

        if (possible_moves != 0) {
            add_move(piece_move, ite_moves, sq, possible_moves, type);
        }
        piece_occ ^= LS1B;
    }
}

typedef struct Position_main_data {
    COLOR side;
    U64 current_occ;
    U64 enemy_occ;
    U64 all_occ;
    Pieces_position current_pieces_position;
    int king_sq;
    U64 check_rook;
    U64 check_bishop;
    U64 check_knight;
    U64 check_pawn;
    U64 check_mask;
    U64 att_mask;
    U64 sum_pin_mask;
    U8 current_castling_rights;
} Position_main_data;

Position_main_data init_main_data(Position *position, U64 *pin_mask) {
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

    U64 check_rook = get_check_rook(king_sq, all_occ, enemy_rook_queen_occ);
    U64 check_bishop = get_check_bishop(king_sq, all_occ, enemy_bishop_queen_occ);
    U64 check_knight = get_check_knight(king_sq, enemy_pieces_position.knight);
    U64 check_pawn = get_check_pawn(king_sq, enemy_pieces_position.pawn, side);

    U64 check_mask = check_rook | check_bishop | check_knight | check_pawn;

    get_pin_rook(king_sq, current_occ, enemy_occ, enemy_rook_queen_occ, &pin_mask[0]);
    get_pin_bishop(king_sq, current_occ, enemy_occ, enemy_bishop_queen_occ, &pin_mask[2]);

    U64 sum_pin_mask = pin_mask[0] | pin_mask[1] | pin_mask[2] | pin_mask[3];

    U64 att_mask = get_full_enemy_attack_mask(enemy_pieces_position, all_occ ^ king_sq1, side);

    U8 castling_rights = side == WHITE ? position->castling_white_rights : position->castling_black_rights;

    return (Position_main_data) {side, current_occ, enemy_occ, all_occ,
                                 current_pieces_position, king_sq,
                                 check_rook, check_bishop,check_knight, check_pawn,
                                 check_mask, att_mask, sum_pin_mask,
                                 castling_rights
    };
}

void king_escape_analysis(Position_main_data *pos_info, Move *moves, int *ite_moves) {
    U64 king_move =
            king_moves(pos_info->king_sq, pos_info->current_occ) & ~pos_info->att_mask;
    add_move(moves, ite_moves, pos_info->king_sq, king_move, KING_MOVE);
}

void check_analysis(Position_main_data *pos_info, Move *moves, int *ite_moves, const U64 *position_occ_pieces,
                    get_moves_func *moves_struct) {
    for (int i = 0; i < 6; i++) {
        U64 occ_without_pin = ~pos_info->sum_pin_mask & position_occ_pieces[i];
        get_move_pin_check(occ_without_pin, pos_info->all_occ, pos_info->current_occ, pos_info->enemy_occ, pos_info->check_mask,
                           (U64 (*)(Board_move_info)) moves_struct[i], moves, ite_moves, i);

    }
}

void without_check_analysis(Position_main_data *pos_info, Move *moves, int *ite_moves, const U64 *position_occ_pieces,
                            get_moves_func *moves_struct) {
    for (int i = 0; i < 6; i++) {
        U64 occ_without_pin = pos_info->sum_pin_mask ? position_occ_pieces[i] & ~pos_info->sum_pin_mask
                                                     : position_occ_pieces[i];
        get_one_move(occ_without_pin, pos_info->all_occ, pos_info->current_occ,pos_info->enemy_occ,
                     (U64 (*)(Board_move_info)) moves_struct[i], moves, ite_moves, i);
    }
}

void pin_analysis(Position_main_data *pos_info, Move *moves, int *ite_moves, const U64 *position_occ_pieces,
                  get_moves_func *moves_struct, U64 pin_mask) {
    for (int pos_i = 0; pos_i < 6; pos_i++) {
        U64 position_occ_piece_pin = position_occ_pieces[pos_i] & pin_mask;
        if (position_occ_piece_pin != 0) {
            get_move_pin_check(position_occ_piece_pin, pos_info->all_occ, pos_info->current_occ, pos_info->enemy_occ, pin_mask,
                               (U64 (*)(Board_move_info)) moves_struct[pos_i], moves, ite_moves, pos_i);
        }
    }
}

void
en_passant_pin_analysis(U64 en_passant_mask_with_pin, const U64 *pin_mask, U64 en_passant_sq1, U64 *ans_en_passant_mask) {
    while (en_passant_mask_with_pin != 0) {
        U64 LS1B = get_LS1B(en_passant_mask_with_pin);
        //3.3.3 possible move with en passant have to be diagonal (start pin_array from 2)
        for (int pin_i = 2; pin_i < 4; pin_i++) {
            if ((LS1B & pin_mask[pin_i]) && (en_passant_sq1 & pin_mask[pin_i])) {
                *ans_en_passant_mask |= LS1B;
            }
        }
        en_passant_mask_with_pin ^= LS1B;
    }
}
void castling_analysis_half(U8 *current_castling_rights, U64 current_rook_occ, U64 current_king_occ, COLOR side) {
    U64 king_rook_occ = current_rook_occ | current_king_occ;
    if (side == WHITE){
        if (is_castling_white_short_valid(king_rook_occ) == false)
            *current_castling_rights &= ~castling_short_right;
        if (is_castling_white_long_valid(king_rook_occ) == false)
            *current_castling_rights &= ~castling_long_right;
    }
    else {
        if (is_castling_black_short_valid(king_rook_occ) == false)
            *current_castling_rights &= ~castling_short_right;
        if (is_castling_black_long_valid(king_rook_occ) == false)
            *current_castling_rights &= ~castling_long_right;
    }
}
#include "../../include/chess/console_visualization.h"
#include "../../include/chess/board_visualisation.h"

void position_analysis(Position *position, Move *moves, int *ite_moves) {
    const COLOR side = position->move_number & 1 ? BLACK : WHITE;
    Pieces_position *current_pieces = (side == WHITE) ? &position->white_pieces : &position->black_pieces;
    Pieces_position *enemy_pieces = (side == WHITE) ? &position->black_pieces : &position->white_pieces;

    U8 *current_castling_rights = (side == WHITE) ? &position->castling_white_rights
                                                  : &position->castling_black_rights;
    U8 *enemy_castling_rights = (side == WHITE) ? &position->castling_black_rights
                                                : &position->castling_white_rights;
    U64 current_king_rook_occ = current_pieces->rook | current_pieces->king;
    U64 enemy_king_rook_occ = enemy_pieces->rook | enemy_pieces->king;
    if (side == WHITE) {
        if (is_castling_white_short_valid(current_king_rook_occ) == false) {
            *current_castling_rights &= ~castling_short_right;
        }
        if (is_castling_white_long_valid(current_king_rook_occ) == false) {
            *current_castling_rights &= ~castling_long_right;
        }
        if (is_castling_black_short_valid(enemy_king_rook_occ) == false) {
            *enemy_castling_rights &= ~castling_short_right;
        }
        if (is_castling_black_long_valid(enemy_king_rook_occ) == false) {
            *enemy_castling_rights &= ~castling_long_right;
        }
    } else {
        if (is_castling_white_short_valid(enemy_king_rook_occ) == false) {
            *enemy_castling_rights &= ~castling_short_right;
        }
        if (is_castling_white_long_valid(enemy_king_rook_occ) == false) {
            *enemy_castling_rights &= ~castling_long_right;
        }
        if (is_castling_black_short_valid(current_king_rook_occ) == false) {
            *current_castling_rights &= ~castling_short_right;
        }
        if (is_castling_black_long_valid(current_king_rook_occ) == false) {
            *current_castling_rights &= ~castling_long_right;
        }
    }

    U64 pin_mask[4] = {0};
    Position_main_data pos_info = init_main_data(position, pin_mask);

    U64 position_pawn_start = pos_info.side == WHITE ? ROW2 & pos_info.current_pieces_position.pawn : ROW7 &
                                                                                                      pos_info.current_pieces_position.pawn;
    U64 position_pawn_after_start = pos_info.current_pieces_position.pawn ^ position_pawn_start;
    U64 position_occ_pieces[6] = {position_pawn_after_start, position_pawn_start, pos_info.current_pieces_position.rook, pos_info.current_pieces_position.knight,
                                  pos_info.current_pieces_position.bishop, pos_info.current_pieces_position.queen};
    get_moves_func moves_struct[6] = {pos_info.side == WHITE ? white_pawn_moves_struct
                                                             : black_pawn_moves_struct,
                                      pos_info.side == WHITE ? white_pawn_moves_start_struct : black_pawn_moves_start_struct, rook_moves_struct, knight_moves_struct, bishop_moves_struct, queen_moves_struct};



    //0.1 king escape analysis
    king_escape_analysis(&pos_info, moves, ite_moves);
    //1. moves analysis depends on double check, check and without check
    //1.1 double check
    if (pos_info.check_rook != 0 && pos_info.check_bishop != 0) {
        return;
    }
    if (pos_info.check_knight != 0 && (pos_info.check_bishop != 0 || pos_info.check_rook != 0)) {
        return;
    }
    if (pos_info.check_pawn != 0 && (pos_info.check_bishop != 0 || pos_info.check_rook != 0)) {
        return;
    }
    //1.2 check
    if (pos_info.check_mask){
        //1.2.1 get moves with check_mask and without pin pieces
        check_analysis(&pos_info, moves, ite_moves, position_occ_pieces, moves_struct);
        //1.2.2 create pin_mask & check_mask - *if there is a check pin piece can move only on pin_mask and check_mask
        for (int i = 0; i < 4; i++) {
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
    if (pos_info.sum_pin_mask != 0) {
        for (int pin_i = 0; pin_i < 4; pin_i++) {
            //2.1 not pin piece
            if (pin_mask[pin_i] == 0)
                continue;
            //2.2 get moves with pin piece
            pin_analysis(&pos_info, moves, ite_moves, position_occ_pieces, moves_struct, pin_mask[pin_i]);
        }
    }

    //3. en passant
    //3.0 en_passant_sq - sq where piece can do en passant
    U8 en_passant_sq = position->en_passant;
    //3.1 is enemy move  - there is no en passant
    if (en_passant_sq != 0) {
        //3.2 get en passant mask (pawns_occ that can do en_passant)
        U64 en_passant_mask =
                pos_info.side == WHITE ? white_en_passant(en_passant_sq, pos_info.current_pieces_position.pawn)
                                       : black_en_passant(en_passant_sq, pos_info.current_pieces_position.pawn);
        //3.2 is there en passant mask
        if (en_passant_mask != 0) {
            //3.3 check if taken pawn is pinned
            //3.3.1 check if there is only one pawn that can do en passant (if there is more than one pawn that can do en passant, there is no rook taken pin)
            bool is_pin_taken = false;
            U64 attacker_1 = get_LS1B(en_passant_mask);
            U64 en_passant_sq1 = 1ULL << en_passant_sq;
            if ((en_passant_mask ^ attacker_1) == 0) {
                //3.3.2 get taken pawn (sq where piece can do en passant plus one square up or down)
                U64 pawn_taken = 1ULL << (en_passant_sq + (pos_info.side == WHITE ? -8 : 8));
                U64 enemy_rook_queen_occ = pos_info.side == WHITE ? position->black_pieces.rook | position->black_pieces.queen
                                                                  : position->white_pieces.rook | position->white_pieces.queen;
                U64 hypothetical_occ = pos_info.all_occ ^ attacker_1;
                hypothetical_occ ^= pawn_taken;
                hypothetical_occ |= en_passant_sq1;
                is_pin_taken = en_passant_pin_taken(hypothetical_occ, pos_info.king_sq, enemy_rook_queen_occ);
            }
            if (!is_pin_taken){
                U64 ans_en_passant_mask = 0;
                U64 en_passant_mask_without_pin = en_passant_mask & ~pos_info.sum_pin_mask;


                U64 en_passant_mask_with_pin = en_passant_mask & pos_info.sum_pin_mask;
                //3.4.1 check en passant without pin
                if (en_passant_mask_without_pin != 0)
                    ans_en_passant_mask = en_passant_mask_without_pin;

                    //3.4.2 check en passant with pin
                else if (en_passant_mask_with_pin) {
                    en_passant_pin_analysis(en_passant_mask_with_pin, pin_mask, en_passant_sq1, &ans_en_passant_mask);
                }
                //3.4.3 add en passant move
                if (ans_en_passant_mask != 0) {
                    add_move(moves, ite_moves, en_passant_sq, ans_en_passant_mask, EN_PASSANT);
                }
            }

        }
    }
    //remove maybe
    if(pos_info.check_mask) return;
    //4. castling
    U64 ans_castling = 0;
    //4.1 check if position have castling rights
    if (pos_info.current_castling_rights & castling_short_right) {
        //4.2 check if castling is possible
        U8 is_castling_possible =
                pos_info.side == WHITE ? is_castling_short_white_possible(pos_info.current_occ, pos_info.enemy_occ, pos_info.att_mask)
                                       : is_castling_short_black_possible(pos_info.current_occ, pos_info.enemy_occ, pos_info.att_mask);
        if (is_castling_possible){
            ans_castling = castling_short_right;
        }

    }
    if (pos_info.current_castling_rights & castling_long_right) {
        U8 is_castling_possible =
                pos_info.side == WHITE ? is_castling_long_white_possible(pos_info.current_occ, pos_info.enemy_occ, pos_info.att_mask)
                                       : is_castling_long_black_possible(pos_info.current_occ, pos_info.enemy_occ, pos_info.att_mask);
        if (is_castling_possible){
            ans_castling |= castling_long_right;
        }
    }
    if (ans_castling != 0) {
        add_move(moves, ite_moves, pos_info.king_sq, ans_castling, CASTLING);
    }
}

