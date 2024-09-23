//
// Created by Jan Tlaga on 04/09/2024.
//
#include <ctype.h>
#include <utmpx.h>
#include <printf.h>
#include "../include/chess/converter_position.h"
#include "assert.h"
#include "../include/chess/algorithms/bits_algorithms.h"

bool is_white(char piece) {
    return isupper(piece);
}
U64 *char_to_piece_occ(Position *position, char piece) {
    Pieces_position *piece_position = is_white(piece) ? &position->white_pieces : &position->black_pieces;
    U64 *piece_occ;
    char piece_type = tolower(piece);
    switch (piece_type) {
        case 'p':
            return &piece_position->pawn;
        case 'r':
            return &piece_position->rook;
        case 'n':
            return &piece_position->knight;
        case 'b':
            return &piece_position->bishop;
        case 'q':
            return &piece_position->queen;
        case 'k':
            return &piece_position->king;
        default:
            printf("Unknown piece %c\n", piece_type);
            assert(false);
    }
}

Position fen_to_position(char *fen) {
    Position position = {0};
    Pieces_position white_pieces = {0};
    Pieces_position black_pieces = {0};
    position.white_pieces = white_pieces;
    position.black_pieces = black_pieces;

    int j = 0;
    int row = 0;
    int column = 0;
    //1. piece placement
    while (*fen != ' ') {
        //1.1 if it is a "/" go to next row
        if (*fen == '/') {
            fen++;
            j = 0;
            row++;
            column = 0;
        }
        //1.2 if it is a number, skip that number of squares
        else if (isdigit(*fen)) {
            for (int k = 0; k < *fen - '0'; k++) {
                j++;
                column ++;
            }
            fen++;
        }
        //1.3 if it is a piece, place it on the board
        else {
            U32 sq = 63 - (row * 8 + column);
            U64 *piece = char_to_piece_occ(&position, *fen);
            *piece |= 1ULL << (sq);
            fen++;
            j++;
            column++;
        }
    }
    //2. active color
    //2.1 skip space
    fen++;
    //2.2 get active color
    assert(*fen == 'w' || *fen == 'b');
    position.move_number += (*fen == 'w') ? 0 : 1;
    fen++;
    //3. castling rights
    //3.1 skip space
    fen++;
    //3.2 get castling rights
    while(*fen && *fen != ' ') {
        if (*fen == 'K') {
            position.castling_white_rights |= castling_short_right;
        }
        else if (*fen == 'Q') {
            position.castling_white_rights |= castling_long_right;
        }
        else if (*fen == 'k') {
            position.castling_black_rights |= castling_short_right;
        }
        else if (*fen == 'q') {
            position.castling_black_rights |= castling_long_right;
        }
        fen++;
    }
    //4. en passant square
    //4.1 skip space
    fen++;
    //4.2 get en passant square
    if (*fen != '-') {
        U8 file = 'h' - *fen;
        assert(file >= 0 && file < 8);
        fen++;
        U8 rank = *fen - '1';
        assert(rank >= 0 && rank < 8);
        position.en_passant = file + rank * 8;
    }
    //5. halfmove clock
    //5.1 skip space
    fen++;
    //5.2 get halfmove clock
    U16 half_move_clock = 0;
    while(*fen && *fen != ' ') {
        half_move_clock = half_move_clock * 10 + (*fen - '0');
        fen++;
    }
    position.half_move_clock = half_move_clock;
    //6. full move number
    //6.1 skip space
    fen++;
    //6.2 get full move number
    U16 full_move_number = 0;
    while(*fen && *fen != ' ') {
        full_move_number = full_move_number * 10 + (*fen - '0');
        fen++;
    }
    position.move_number += full_move_number * 2;



    return position;
}