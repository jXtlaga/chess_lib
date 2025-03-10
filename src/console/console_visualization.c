//
// Created by Jan Tlaga on 31/08/2024.
//

#include <stdio.h>
#include "../../include/chess/console_visualization.h"
#include "../../include/chess/algorithms/bits_algorithms.h"
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
void print_positions(Position *positions, U64 *attacks, int num_positions) {
    char boards[num_positions][64];
    char white_pieces_symbols[] = {'P', 'R', 'N', 'B', 'Q', 'K'};
    char black_pieces_symbols[] = {'p', 'r', 'n', 'b', 'q', 'k'};

    // Initialize boards
    for (int n = 0; n < num_positions; n++) {
        for (int i = 0; i < 64; i++) {
            boards[n][i] = '.';
        }
    }

    // Populate boards with pieces
    for (int n = 0; n < num_positions; n++) {
        U64 *white_pieces[] = {
                &positions[n].white_pieces.pawn,
                &positions[n].white_pieces.rook,
                &positions[n].white_pieces.knight,
                &positions[n].white_pieces.bishop,
                &positions[n].white_pieces.queen,
                &positions[n].white_pieces.king
        };
        U64 *black_pieces[] = {
                &positions[n].black_pieces.pawn,
                &positions[n].black_pieces.rook,
                &positions[n].black_pieces.knight,
                &positions[n].black_pieces.bishop,
                &positions[n].black_pieces.queen,
                &positions[n].black_pieces.king
        };

        for (int i = 0; i < 6; i++) {
            bitboard_to_array(*white_pieces[i], white_pieces_symbols[i], boards[n]);
            bitboard_to_array(*black_pieces[i], black_pieces_symbols[i], boards[n]);
        }

        bitboard_to_array(attacks[n], 'X', boards[n]);
    }

    // Print boards side by side
    for (int row = 7; row >= 0; row--) {
        for (int n = 0; n < num_positions; n++) {
            for (int col = 7; col >= 0; col--) {
                printf("%c ", boards[n][row * 8 + col]);
            }
            printf("   "); // Space between boards
        }
        printf("\n");
    }
    printf("\n");
}
void print_position_with_info(Position *position){
    position->move_number & 1 ? printf("Black to move\n") : printf("White to move\n");
    printf("castling white rights: %d\n", position->castling_white_rights);
    printf("castling black rights: %d\n", position->castling_black_rights);
    printf("en passant: %d\n", position->en_passant);
    printf("half move clock: %d\n", position->half_move_clock);
    printf("move number: %d\n", position->move_number);
    print_position(position, 0);
}