#include <stdio.h>
#include <assert.h>
//#include "../include/chess/bishop_attack.h"
#include "../include/chess/board_visualisation.h"
#include "../include/chess/define_types.h"
#include <math.h>
#include "../include/chess/bits_alghorithms.h"
#include "../include/chess/combinatorics_mask.h"
#include "../include/chess/moves_calculation.h"
#include "../include/chess/squares_alghorithms.h"
int main() {
    int sq = 32;
    U64 mask_rook = 0;
    U64 ans = generate_rook_attack(mask_rook, sq);
    printBinaryAsBoard(ans);
    U64 ans_bishop = generate_bishop_attack(mask_rook, sq);
    printBinaryAsBoard(ans_bishop);
    return 0;
}