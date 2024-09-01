//
// Created by Jan Tlaga on 31/08/2024.
//

#include <stdio.h>
#include "../../include/chess/console_visualization.h"
#include "../../include/chess/bits_alghorithms.h"
void bitboard_to_array(U64 bitboard, char piece, char *array) {
    while (bitboard) {
        U64 LSB = get_LS1B(bitboard);
        int index = get_one_bit_index(LSB);
        array[index] = piece;
        bitboard = remove_bits(bitboard, LSB);
    }
}
void print_board(char *board) {
    for (int i = 63; i >= 0; i--) {
        printf("%c ", board[i]);
        if ((i) % 8 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}
void print_position(Position position, U64 attack){
    char board[64];
    for (int i = 0; i < 64; i++) {
        board[i] = '.';
    }
    bitboard_to_array(position.white_pawns, 'P', board);
    bitboard_to_array(position.white_rooks, 'R', board);
    bitboard_to_array(position.white_knights, 'N', board);
    bitboard_to_array(position.white_bishops, 'B', board);
    bitboard_to_array(position.white_queens, 'Q', board);
    bitboard_to_array(position.white_king, 'K', board);
    bitboard_to_array(position.black_pawns, 'p', board);
    bitboard_to_array(position.black_rooks, 'r', board);
    bitboard_to_array(position.black_knights, 'n', board);
    bitboard_to_array(position.black_bishops, 'b', board);
    bitboard_to_array(position.black_queens, 'q', board);
    bitboard_to_array(position.black_king, 'k', board);

    bitboard_to_array(attack, 'X', board);
    print_board(board);
}