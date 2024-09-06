//
// Created by Jan Tlaga on 04/09/2024.
//
#include <ctype.h>
#include <utmpx.h>
#include "../include/chess/converter_position.h"
#include "assert.h"

bool is_white(char piece) {
    return isupper(piece);
}
U64 *char_to_piece_occ(Position *position, char piece) {
    Pieces_position *piece_position = is_white(piece) ? &position->white_pieces : &position->black_pieces;
    U64 *piece_occ;
    char piece_type = tolower(piece);
    switch (piece_type) {
        case 'p':
            piece_occ = &piece_position->pawn;
            break;
        case 'r':
            piece_occ = &piece_position->rook;
            break;
        case 'n':
            piece_occ = &piece_position->knight;
            break;
        case 'b':
            piece_occ = &piece_position->bishop;
            break;
        case 'q':
            piece_occ = &piece_position->queen;
            break;
        case 'k':
            piece_occ = &piece_position->king;
            break;
        default:
            piece_occ = NULL;
            assert(false);
    }
    return piece_occ;
}

Position fen_to_position(char *fen) {
    Position position = {0};
    Pieces_position white_pieces = {0};
    Pieces_position black_pieces = {0};
    position.white_pieces = white_pieces;
    position.black_pieces = black_pieces;

    int i = 0;
    int j = 0;
    int row = 0;
    int column = 0;
    while (fen[i] != ' ') {
        if (fen[i] == '/') {
            i++;
            j = 0;
            row++;
            column = 0;
        } else if (isdigit(fen[i])) {
            for (int k = 0; k < fen[i] - '0'; k++) {
                j++;
                column ++;
            }
            i++;
        } else {
            U32 sq = 63 - (row * 8 + column);
            U64 *piece = char_to_piece_occ(&position, fen[i]);
            *piece |= 1ULL << (sq);
            i++;
            j++;
            column++;
        }
    }
    return position;
}