//
// Created by Jan Tlaga on 27/08/2024.
//

#ifndef CHESS_C_LIB_DEFINITION_TYPES_H
#define CHESS_C_LIB_DEFINITION_TYPES_H
#include "stdint.h"

typedef uint64_t U64;
typedef uint32_t U32;
typedef uint16_t U16;
typedef uint8_t U8;

#define bool char
#define true 1
#define false 0

#define ROW1 0xFF
#define ROW2 0xFF00
#define ROW3 0xFF0000
#define ROW4 0xFF000000
#define ROW5 0xFF00000000
#define ROW6 0xFF0000000000
#define ROW7 0xFF000000000000
#define ROW8 0xFF00000000000000

#define COLUMN_A 0x8080808080808080
#define COLUMN_B 0x4040404040404040
#define COLUMN_C 0x2020202020202020
#define COLUMN_D 0x1010101010101010
#define COLUMN_E 0x0808080808080808
#define COLUMN_F 0x0404040404040404
#define COLUMN_G 0x0202020202020202
#define COLUMN_H 0x0101010101010101

static U64 DISTANCE_DOWN_TO_ROW[8] = {ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7, ROW8};
static U64 DISTANCE_RIGHT_TO_COLUMN[8] = {COLUMN_H, COLUMN_G, COLUMN_F, COLUMN_E, COLUMN_D, COLUMN_C, COLUMN_B, COLUMN_A};
static U64 DIAGONAL_DOWN_LEFT_TO_UP_RIGHT[15] = {0x80, 0x8040, 0x804020, 0x80402010, 0x8040201008, 0x804020100804, 0x80402010080402, 0x8040201008040201, 0x4020100804020100, 0x2010080402010000, 0x1008040201000000, 0x0804020100000000, 0x0402010000000000, 0x0201000000000000, 0x0100000000000000};
static U64 DIAGONAL_DOWN_RIGHT_TO_UP_LEFT[15] = {0x1, 0x102, 0x10204, 0x1020408, 0x102040810, 0x10204081020, 0x1020408102040, 0x102040810204080, 0x204081020408000, 0x408102040800000, 0x810204080000000, 0x1020408000000000, 0x2040800000000000, 0x4080000000000000, 0x8000000000000000};

#define CASTLE_SHORT_CODE 1;
#define CASTLE_LONG_CODE 2;

typedef enum COLOR{
    WHITE = 0,
    BLACK = 1
} COLOR;

typedef struct Board_move_info{
    int sq;
    U64 all_occ;
    U64 current_color_occ;
    U64 enemy_color_occ;
} Board_move_info;

typedef struct Pieces_position{
    U64 pawn;
    U64 rook;
    U64 knight;
    U64 bishop;
    U64 queen;
    U64 king;
} Pieces_position;

typedef struct Position{
    Pieces_position white_pieces;
    Pieces_position black_pieces;
    U8 en_passant;
    U8 castling_white_rights;
    U8 castling_black_rights;
    U16 move_number;
    U16 half_move_clock;
} Position;


#define castling_short_right 0b1
#define castling_long_right 0b10

typedef enum TYPE_MOVE {
    PAWN_MOVE = 0,
    PAWN_MOVE_START = 1,
    ROOK_MOVE = 2,
    KNIGHT_MOVE = 3,
    BISHOP_MOVE = 4,
    QUEEN_MOVE = 5,
    KING_MOVE = 6,
    CASTLING = 7,
    EN_PASSANT = 8,
    PROMOTION_QUEEN = 9,
    PROMOTION_ROOK = 10,
    PROMOTION_BISHOP = 11,
    PROMOTION_KNIGHT = 12
} TYPE_MOVE;

typedef struct Move{
    U8 from;
    U64 to;
    TYPE_MOVE type;
} Move;


typedef enum TYPE_PIECE{
    ROOK = 0,
    KNIGHT = 1,
    BISHOP = 2,
    QUEEN = 3,
    KING = 4,
    PAWN = 5
} TYPE_PIECE;

typedef U64 (*get_moves_func)(Board_move_info*);


#endif //CHESS_C_LIB_DEFINITION_TYPES_H
