//
// Created by Jan Tlaga on 05/09/2024.
//

#include "../../include/chess/mask_alg/attack_mask.h"
#include "../../include/chess/algorithms/bits_algorithms.h"
#include "../../include/chess/moves/rook_attack.h"
#include "../../include/chess/moves/bishop_attack.h"
#include "../../include/chess/moves/knight_attack.h"
#include "../../include/chess/moves/king_attack.h"
#include "../../include/chess/moves/pawn_attack.h"

U64 get_enemy_attack_mask_rook_bishop(U64 enemy_piece_occ, U64 all_occ_without_king , U64 (*get_moves_func)(U64, int)){
    U64 attack_mask = 0;
    while(enemy_piece_occ){
        U64 LS1B = get_LS1B(enemy_piece_occ);
        int sq = get_one_bit_index(LS1B);
        attack_mask |= get_moves_func(all_occ_without_king, sq);
        enemy_piece_occ ^= LS1B;
    }
    return attack_mask;
}
U64 get_one_enemy_attack_mask_knight_king_pawn(U64 enemy_piece_occ, U64 (*get_moves_func)(int)){
    U64 attack_mask = 0;
    while(enemy_piece_occ){
        U64 LS1B = get_LS1B(enemy_piece_occ);
        int sq = get_one_bit_index(LS1B);
        attack_mask |= get_moves_func(sq);
        enemy_piece_occ ^= LS1B;
    }
    return attack_mask;
}
U64 get_full_enemy_attack_mask(Pieces_position enemy_pieces, U64 all_occ_without_king, COLOR side){
    U64 attack_mask = 0;
    attack_mask |= get_enemy_attack_mask_rook_bishop(enemy_pieces.rook | enemy_pieces.queen,all_occ_without_king, get_rook_attack);
    attack_mask |= get_enemy_attack_mask_rook_bishop(enemy_pieces.bishop | enemy_pieces.queen, all_occ_without_king, get_bishop_attack);
    attack_mask |= get_one_enemy_attack_mask_knight_king_pawn(enemy_pieces.knight, get_knight_attack);
    attack_mask |= get_one_enemy_attack_mask_knight_king_pawn(enemy_pieces.king, get_king_attack);
    attack_mask |= get_one_enemy_attack_mask_knight_king_pawn(enemy_pieces.pawn, side == WHITE ? get_black_pawn_attack : get_white_pawn_attack);
    return attack_mask;
}
