//
// Created by Jan Tlaga on 9/8/24.
//
#include <stdio.h>
#include <time.h>
#define U64 u_int64_t
// Przykładowe funkcje do porównania
typedef struct Pieces{
    U64 pawns;
    U64 knights;
    U64 bishops;
    U64 rooks;
    U64 queens;
    U64 kings;
} Pieces;
void funkcja1(U64 *arr) {
    U64 x = 16;
    for(int ite = 0; ite < 1000000; ite++) {
        for (int i = 0; i < 6; i++) {
            arr[i] ^= x;
        }
    }
}

void funkcja2(U64 *arr) {
    for(int ite = 0; ite < 1000000; ite++) {
        U64 x = 16;
        for (int i = 0; i < 6; i++) {
            if (arr[i] & x) {
                arr[i] ^= x;
            }
        }
    }
}

int main() {
    for(int t = 0; t < 10; t++) {
        U64 arr[6] = {0, 0, 0, 0, 0, 0};
        clock_t start, end;
        double time_taken;
        start = clock();
        funkcja2(arr);
        end = clock();
        time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Czas funkcji 1: %f\n", time_taken);

        start = clock();
        funkcja1(arr);
        end = clock();
        time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Czas funkcji 2: %f\n", time_taken);
    }

    return 0;
}