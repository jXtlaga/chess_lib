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
void print_position(Position *position, U64 attack){
    char board[64];
    for (int i = 0; i < 64; i++) {
        board[i] = '.';
    }
    char white_pieces_symbols[] = {'P', 'R', 'N', 'B', 'Q', 'K'};
    char black_pieces_symbols[] = {'p', 'r', 'n', 'b', 'q', 'k'};
    U64 *white_pieces[] = {
            &position->white_pieces.pawn,
            &position->white_pieces.rook,
            &position->white_pieces.knight,
            &position->white_pieces.bishop,
            &position->white_pieces.queen,
            &position->white_pieces.king
    };
    U64 *black_pieces[] = {
            &position->black_pieces.pawn,
            &position->black_pieces.rook,
            &position->black_pieces.knight,
            &position->black_pieces.bishop,
            &position->black_pieces.queen,
            &position->black_pieces.king
    };

    for (int i = 0; i < 6; i++) {
        bitboard_to_array(*white_pieces[i], white_pieces_symbols[i], board);
    }

    for (int i = 0; i < 6; i++) {
        bitboard_to_array(*black_pieces[i], black_pieces_symbols[i], board);
    }

    bitboard_to_array(attack, 'X', board);
    print_board(board);
}