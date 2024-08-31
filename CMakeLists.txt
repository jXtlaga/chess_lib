cmake_minimum_required(VERSION 3.26)
project(chess_c_lib C)

set(CMAKE_C_STANDARD 11)

add_executable(chess_c_lib
        src/main.c
        src/board_visualisation.c
        src/combinatorics_mask.c
        src/moves_alg/bishop_attack.c
        src/bits_alghorithms.c
        src/moves_calculation.c
        src/squares_alghorithms.c
        src/moves_alg/rook_attack.c
        src/moves_alg/knight_attack.c
        src/moves_alg/king_attack.c
        src/moves_alg/pawn_attack.c
)
