//
// Created by Jan Tlaga on 27/08/2024.
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int get_distance_right(int sq){
    return sq % 8;
}
int get_distance_down(int sq){
    return (int) sq / 8;
}
int count_rook_line_mask(int distance){
    int count;
    if(distance == 0 || distance== 7){
        count = 6;
    }
    else{
        count = 5;
    }
    return count;
}
int count_rook_mask(int distance_right, int distance_down){
    return count_rook_line_mask(distance_right) + count_rook_line_mask(distance_down);;
}
int count_bishop_half_line_mask(int distance_x, int distance_y){
    int count = distance_x < distance_y ? distance_x : distance_y;
    if (count != 0){
        count -= 1;
    }
    return count;
}
int count_bishop_mask(int distance_right, int distance_down){
    int count;
    int distance_up = 7 - distance_down;
    int distance_left = 7 - distance_right;
    count = count_bishop_half_line_mask(distance_right,distance_up);
    count += count_bishop_half_line_mask(distance_up, distance_left);
    count += count_bishop_half_line_mask(distance_left, distance_down);
    count += count_bishop_half_line_mask(distance_down, distance_right);
    return count;
}
void print_mask_bit_count(int count_piece_mask(int, int)){
    printf("{ ");
    for(int sq = 0; sq < 64; sq++){
        int distance_right = get_distance_right(sq);
        int distance_down = get_distance_down(sq);
        if(sq % 8 == 0){
            printf("\n");
        }
        printf("%d, ", count_piece_mask(distance_right, distance_down));
    }
    printf("\n};");
}
const int BISHOP_MASK_BITS_COUNT[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
};
int bishop_mask[64];
int count_bishop(int sq) {
    return BISHOP_MASK_BITS_COUNT[sq];
}
int count_bishop2(int sq, int *arr_mask){
    return arr_mask[sq];
}




// Funkcja do mierzenia czasu działania obu funkcji
int compareFunctionTimes(int sq, int *arr_mask) {
    clock_t start, end;
    double cpu_time_used_count_bishop, cpu_time_used_count_bishop3;

    // Mierzenie czasu działania getRookLineMaskCount
    start = clock();
    count_bishop2(sq, arr_mask);
    end = clock();
    cpu_time_used_count_bishop = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Mierzenie czasu działania countRookLineMask
    start = clock();

    count_bishop(sq);
    end = clock();
    cpu_time_used_count_bishop3 = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Wyświetlanie wyników
    printf("Time taken by count_bishop: %f seconds\n", cpu_time_used_count_bishop);
    printf("Time taken by count_bishop3: %f seconds\n", cpu_time_used_count_bishop3);

    // Porównanie czasów
    if (cpu_time_used_count_bishop < cpu_time_used_count_bishop3) {
        printf("count_bishop is faster.\n");
        return 1;
    } else if (cpu_time_used_count_bishop > cpu_time_used_count_bishop3) {
        return 2;
    } else {
       return 0;
    }
}

int main() {
    int *arrRook = (int *) malloc(sizeof(int) * 64);
    int sum1 = 0, sum2 = 0;
    for(int sq = 0; sq < 64; sq++){
        int distance_right = get_distance_right(sq);
        int distance_down = get_distance_down(sq);

        bishop_mask[sq] = count_bishop_mask(distance_right, distance_down);

    }
    for(int i = 0; i<64; i++){
        int asn = compareFunctionTimes(i, arrRook);
        if (asn == 1){
            sum1++;
        }else if (asn == 2)
            sum2++;
    }
    printf("sum1: %d, sum2: %d", sum1, sum2);
    return 0;
}