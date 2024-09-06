//
// Created by Jan Tlaga on 27/08/2024.
//

#include "../include/chess/board_visualisation.h"
void print_bitboard(U64 n) {
    U64 i;
    unsigned int j = 0;
    for (i = (U64) 1 << (sizeof(U64) * 8 - 1); i > 0; i = i >> 1) {
        j++;
        if (n & i)
            printf("1");
        else
            printf("0");
        if (j % 8 == 0)
            printf("\n");
    }
    printf("\n");
}

void print_type(TYPE_PIECE type){
    switch(type){
        case ROOK:
            printf("ROOK\n");
            break;
        case KNIGHT:
            printf("KNIGHT\n");
            break;
        case BISHOP:
            printf("BISHOP\n");
            break;
        case QUEEN:
            printf("QUEEN\n");
            break;
        case KING:
            printf("KING\n");
            break;
        case PAWN:
            printf("PAWN\n");
            break;
    }
}

void print_type_move(TYPE_MOVE type){
    switch(type){
        case ROOK_MOVE:
            printf("ROOK_MOVE\n");
            break;
        case BISHOP_MOVE:
            printf("BISHOP_MOVE\n");
            break;
        case QUEEN_MOVE:
            printf("QUEEN_MOVE\n");
            break;
        case KNIGHT_MOVE:
            printf("KNIGHT_MOVE\n");
            break;
        case KING_MOVE:
            printf("KING_MOVE\n");
            break;
        case PAWN_MOVE:
            printf("PAWN_MOVE\n");
            break;
        case CASTLING_SHORT:
            printf("CASTLING_SHORT\n");
            break;
        case CASTLING_LONG:
            printf("CASTLING_LONG\n");
            break;
        case EN_PASSANT:
            printf("EN_PASSANT\n");
            break;
        case PROMOTION:
            printf("PROMOTION\n");
            break;
    }
}

