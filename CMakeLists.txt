cmake_minimum_required(VERSION 3.26)
project(chess_c_lib C)

set(CMAKE_C_STANDARD 11)

# Źródła główne
set(SRC_MAIN
    src/main.c
    src/converter_position.c
)

# Wizualizacja i konsola
set(SRC_CONSOLE
    src/console/console_chess.c
    src/console/console_visualization.c
    src/board_visualisation.c
)

# Algorytmy
set(SRC_ALGORITHMS
    src/algorithms/combinatorics_mask.c
    src/algorithms/bits_alghorithms.c
    src/algorithms/squares_alghorithms.c
)

# Generacja ataków
set(SRC_ATTACK_GENERATION
    src/attack_generation/bishop_attack.c
    src/attack_generation/rook_attack.c
    src/attack_generation/knight_attack.c
    src/attack_generation/king_attack.c
    src/attack_generation/pawn_attack.c
    src/attack_generation/magic_hash_bishop_rook.c
)

# Ruchy
set(SRC_MOVES
    src/moves_alg/moves_calculation.c
    src/moves_alg/moves_position.c
    src/moves_alg/moves_pieces.c
)

# Maski
set(SRC_MASKS
    src/mask_alg/check_mask.c
    src/mask_alg/pin_mask.c
    src/mask_alg/attack_mask.c
)

# Obsługa ruchów
set(SRC_MOVE_HANDLING
    src/move_handling/move_generation.c
    src/move_handling/make_move.c
    src/move_handling/mask_feature_generation.c
)

# Roszada i en passant
set(SRC_CASTLING
    src/castling_en_passant_handling/castling_validation.c
    src/castling_en_passant_handling/en_passant_validation.c
)

# Wyszukiwanie
set(SRC_SEARCH
    src/search_moves/search_tree.c
)

# Nagłówki
set(INCLUDES
    include/chess/search/search_tree.h
    include/chess/mask_feature_generation.h
    include/chess/magic_hash_bishop_rook.h
)

add_executable(chess_c_lib
    ${SRC_MAIN}
    ${SRC_CONSOLE}
    ${SRC_ALGORITHMS}
    ${SRC_ATTACK_GENERATION}
    ${SRC_MOVES}
    ${SRC_MASKS}
    ${SRC_MOVE_HANDLING}
    ${SRC_CASTLING}
    ${SRC_SEARCH}
    ${INCLUDES}
)
