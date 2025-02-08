cmake_minimum_required(VERSION 3.26)
project(chess_c_lib C)

set(CMAKE_C_STANDARD 11)

add_executable(chess_c_lib
        src/main.c
        src/board_visualisation.c
        src/algorithms/combinatorics_mask.c
        src/attack_generation/bishop_attack.c
        src/algorithms/bits_alghorithms.c
        src/moves_alg/moves_calculation.c
        src/algorithms/squares_alghorithms.c
        src/attack_generation/rook_attack.c
        src/attack_generation/knight_attack.c
        src/attack_generation/king_attack.c
        src/attack_generation/pawn_attack.c
        src/moves_alg/moves_position.c
        src/console/console_chess.c
        src/console/console_visualization.c
        src/moves_alg/moves_pieces.c
        src/mask_alg/check_mask.c
        src/converter_position.c
        src/mask_alg/pin_mask.c
        src/mask_alg/attack_mask.c
        src/move_handling/move_generation.c
        src/move_handling/make_move.c
        src/castling_en_passant_handling/castling_validation.c
        src/castling_en_passant_handling/en_passant_validation.c
        include/chess/search/search_tree.h
        src/search_moves/search_tree.c
        src/move_handling/mask_feature_generation.c
        include/chess/mask_feature_generation.h
        src/attack_generation/magic_hash_bishop_rook.c
        include/chess/magic_hash_bishop_rook.h
)
