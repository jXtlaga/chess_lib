//
// Created by Jan Tlaga on 9/8/24.
//

#include "../../include/chess/make_move.h"
#include "../../include/chess/moves_alg/moves_pieces.h"
#include "../../include/chess/algorithms/squares_alghorithms.h"
#include "../../include/chess/castling_validation.h"
#include "../../include/chess/en_passant_validation.h"
#include <assert.h>
#include "../../include/chess/board_visualisation.h"
void promote_pawn(U64 *pawn, U64 *promoted_piece, U64 from_sq, U64 to_sq) {
    *pawn ^= from_sq;
    print_bitboard(from_sq);
    *promoted_piece |= to_sq;
}
void play_castling(U64 *king_sq1, U64 *rook_sq1, COLOR side, bool is_short_castling) {
    if (side == WHITE) {
        if (is_short_castling) {
            *king_sq1 = 0x2;
            *rook_sq1 |= 0x4;
            *rook_sq1 &= ~0x1;
        } else {
            *king_sq1 = 0x20;
            *rook_sq1 |= 0x10;
            *rook_sq1 &= ~0x80;
        }
    } else {
        if (is_short_castling) {
            *king_sq1 = 0x0200000000000000;
            *rook_sq1 |= 0x0400000000000000;
            *rook_sq1 &= ~0x0100000000000000;
        } else {
            *king_sq1 = 0x2000000000000000;
            *rook_sq1 |= 0x1000000000000000;
            *rook_sq1 &= ~0x8000000000000000;
        }
    }
}

void remove_enemy_piece(U64 remove_occ, Pieces_position *enemy_pieces) {
    U64 not_remove_occ = ~remove_occ;
    enemy_pieces->pawn &= not_remove_occ;
    enemy_pieces->rook &= not_remove_occ;
    enemy_pieces->bishop &= not_remove_occ;
    enemy_pieces->knight &= not_remove_occ;
    enemy_pieces->queen &= not_remove_occ;
}

void move_and_remove_piece(U64 *current_piece_occ, Pieces_position *enemy_pieces, U64 from_sq1, U64 to) {
    assert((from_sq1 & *current_piece_occ) != 0);
    *current_piece_occ ^= from_sq1;
    *current_piece_occ |= to;
    remove_enemy_piece(to, enemy_pieces);
}

Position make_move(Position position, U8 from, U64 to, TYPE_MOVE type_move) {
    Position new_position = position;
    const COLOR side = position.move_number & 1 ? BLACK : WHITE;
    new_position.move_number++;
    Pieces_position *current_pieces = (side == WHITE) ? &new_position.white_pieces : &new_position.black_pieces;
    Pieces_position *enemy_pieces = (side == WHITE) ? &new_position.black_pieces : &new_position.white_pieces;

    new_position.en_passant_white_sq = 0;
    new_position.en_passant_black_sq = 0;

    U64 current_occ = get_occ(current_pieces);

    U8 *current_castling_rights = (side == WHITE) ? &new_position.castling_white_rights
                                                  : &new_position.castling_black_rights;
    U64 from_sq1 = 1ULL << from;
    switch (type_move) {
        case CASTLING: {
            bool is_short_castling = (castling_short_right & to) != 0;
            bool is_long_castling = (castling_long_right & to) != 0;

            if (is_short_castling || is_long_castling) {
                play_castling(&current_pieces->king, &current_pieces->rook, side, is_short_castling);
                *current_castling_rights = 0;
            }
            break;
        }
        case EN_PASSANT: {
            current_pieces->pawn = from_sq1;
            current_pieces->pawn ^= to;
            enemy_pieces->pawn ^= (side == WHITE) ? (to >> 8) : (to << 8);
            break;
        }
        case PROMOTION_QUEEN: {
            promote_pawn(&current_pieces->pawn, &current_pieces->queen, from_sq1, to);
            break;
        }
        case PROMOTION_ROOK: {
            promote_pawn(&current_pieces->pawn, &current_pieces->rook, from_sq1, to);
            break;
        }
        case PROMOTION_BISHOP: {
            promote_pawn(&current_pieces->pawn, &current_pieces->bishop, from_sq1, to);
            break;
        }
        case PROMOTION_KNIGHT: {
            promote_pawn(&current_pieces->pawn, &current_pieces->knight, from_sq1, to);
            break;
        }
        case ROOK_MOVE: {
            if (side == WHITE) {
                if (is_castling_white_short_invalided(current_occ) == false) {
                    *current_castling_rights &= ~castling_short_right;
                }
                if (is_castling_white_long_invalided(current_occ) == false) {
                    *current_castling_rights &= ~castling_short_right;
                }
            } else {
                if (is_castling_black_long_invalided(current_occ) == true) {
                    *current_castling_rights &= ~castling_long_right;
                }
                if (is_castling_black_short_invalided(current_occ) == true) {
                    *current_castling_rights &= ~castling_short_right;
                }
            }
            move_and_remove_piece(&current_pieces->rook, enemy_pieces, from_sq1, to);
            break;
        }
        case BISHOP_MOVE: {
            move_and_remove_piece(&current_pieces->bishop, enemy_pieces, from_sq1, to);
            break;
        }
        case KNIGHT_MOVE: {
            move_and_remove_piece(&current_pieces->knight, enemy_pieces, from_sq1, to);
            break;
        }
        case QUEEN_MOVE: {
            move_and_remove_piece(&current_pieces->queen, enemy_pieces, from_sq1, to);
            break;
        }
        case KING_MOVE: {
            move_and_remove_piece(&current_pieces->king, enemy_pieces, from_sq1, to);
            *current_castling_rights = 0;
            break;
        }
        case PAWN_MOVE_START: {
            if (side == WHITE) {
                new_position.en_passant_white_sq = is_en_passant_white_valid(to);
            } else {
                new_position.en_passant_black_sq = is_en_passant_black_valid(to);
            }
            move_and_remove_piece(&current_pieces->pawn, enemy_pieces, from_sq1, to);
            break;
        }
        case PAWN_MOVE: {
            move_and_remove_piece(&current_pieces->pawn, enemy_pieces, from_sq1, to);
            break;
        }
    }

    return new_position;
}