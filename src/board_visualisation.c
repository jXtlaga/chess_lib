//
// Created by Jan Tlaga on 27/08/2024.
//

#include "../include/chess/board_visualisation.h"
void printBinaryAsBoard(U64 n) {
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