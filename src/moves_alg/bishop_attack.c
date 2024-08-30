//
// Created by Jan Tlaga on 27/08/2024.
//
#include "../../include/chess/bishop_attack.h"
#include "../../include/chess/bits_alghorithms.h"
#include <assert.h>
#include "math.h"
#include "../../include/chess/squares_alghorithms.h"

U64 bishop_hash_[64][512] = {0};

const U64 BISHOP_MASK[] = {0x0040201008040200, 0x0000402010080400, 0x0000004020100A00, 0x0000000040221400, 0x0000000002442800, 0x0000000204085000, 0x0000020408102000, 0x0002040810204000,
                          0x0020100804020000, 0x0040201008040000, 0x00004020100A0000, 0x0000004022140000, 0x0000000244280000, 0x0000020408500000, 0x0002040810200000, 0x0004081020400000,
                          0x0010080402000200, 0x0020100804000400, 0x004020100A000A00, 0x0000402214001400, 0x0000024428002800, 0x0002040850005000, 0x0004081020002000, 0x0008102040004000,
                          0x0008040200020400, 0x0010080400040800, 0x0020100A000A1000, 0x0040221400142200, 0x0002442800284400, 0x0004085000500800, 0x0008102000201000, 0x0010204000402000,
                          0x0004020002040800, 0x0008040004081000, 0x00100A000A102000, 0x0022140014224000, 0x0044280028440200, 0x0008500050080400, 0x0010200020100800, 0x0020400040201000,
                          0x0002000204081000, 0x0004000408102000, 0x000A000A10204000, 0x0014001422400000, 0x0028002844020000, 0x0050005008040200, 0x0020002010080400, 0x0040004020100800,
                          0x0000020408102000, 0x0000040810204000, 0x00000A1020400000, 0x0000142240000000, 0x0000284402000000, 0x0000500804020000, 0x0000201008040200, 0x0000402010080400,
                          0x0002040810204000, 0x0004081020400000, 0x000A102040000000, 0x0014224000000000, 0x0028440200000000, 0x0050080402000000, 0x0020100804020000, 0x0040201008040200,
};

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

const U64 BISHOP_MAGIC[64] = {
        0x420c80100408202ULL,   0x1204311202260108ULL,   0x2008208102030000ULL,   0x24081001000caULL,   0x488484041002110ULL,   0x1a080c2c010018ULL,   0x20a02a2400084ULL,   0x440404400a01000ULL,
        0x8931041080080ULL,   0x200484108221ULL,   0x80460802188000ULL,   0x4000090401080092ULL,   0x4000011040a00004ULL,   0x20011048040504ULL,   0x2008008401084000ULL,   0x102422a101a02ULL,
        0x2040801082420404ULL,   0x8104900210440100ULL,   0x202101012820109ULL,   0x248090401409004ULL,   0x44820404a00020ULL,   0x40808110100100ULL,   0x480a80100882000ULL,   0x184820208a011010ULL,
        0x110400206085200ULL,   0x1050010104201ULL,   0x4008480070008010ULL,   0x8440040018410120ULL,   0x41010000104000ULL,   0x4010004080241000ULL,   0x1244082061040ULL,   0x51060000288441ULL,
        0x2215410a05820ULL,   0x6000941020a0c220ULL,   0xf2080100020201ULL,   0x8010020081180080ULL,   0x940012060060080ULL,   0x620008284290800ULL,   0x8468100140900ULL,   0x418400aa01802100ULL,
        0x4000882440015002ULL,   0x420220a11081ULL,   0x401a26030000804ULL,   0x2184208000084ULL,   0xa430820a0410c201ULL,   0x640053805080180ULL,   0x4a04010a44100601ULL,   0x10014901001021ULL,
        0x422411031300100ULL,   0x824222110280000ULL,   0x8800020a0b340300ULL,   0xa8000441109088ULL,   0x404000861010208ULL,   0x40112002042200ULL,   0x2141006480b00a0ULL,   0x2210108081004411ULL,
        0x2010804070100803ULL,   0x7a0011010090ac31ULL,   0x18005100880400ULL,   0x8010001081084805ULL,   0x400200021202020aULL,   0x4100342100a0221ULL,   0x404408801010204ULL,   0x6360041408104012ULL,
};
U64 get_bishop_attack(U64 occ, int sq){
    U64 mask = BISHOP_MASK[sq];
    U64 magic = BISHOP_MAGIC[sq];
    int index = (occ & mask) * magic >> (64 - BISHOP_MASK_BITS_COUNT[sq]);
    U64 attacks = bishop_hash_[sq][index];
    return attacks;
}
//int bishopMagicSqInit(int sq, dirCombination dirComb[4]){
//    int distance_right = get_distance_right(sq);
//    int distance_down = get_distance_down(sq);
//    U32 possible_combination = pow2_unsigned_32_exponent(BISHOP_MASK_BITS_COUNT[sq]);
//    for(int i = 0; i < possible_combination; i++){
//        generate_bishop_mask_combination(sq, i, dirComb);
//    }
//    U64 *magicNumbers =  bishopMagicNumbers[sq];
//    uint8_t iteRight = 0;
//    uint64_t downPosition = nextCombination(dirComb[3], iteDown);
//    iteDown++;
//    //creating key from (mask & sum position) * magic_number >> 64 - count of bits in mask
//    U64 position = rightPosition|leftPosition|upPosition|downPosition;
//    unsigned int index = (bishopMask[sq] & position) * BMagic[sq] >> (64 - BishopBits[sq]);
//    assert(index < 4096 && "error- bigger index");
//    assert(magicNumbers[index] == 0 && "error- repeated index");
//    U64 board = 1;
//    board <<= sq;
//    *(magicNumbers + index) = bishopShiftMoves(board, 0, position);
//    return 1;
//}
//void bishopInit(){
//    for(int i = 0; i < 64; i++){
//        int dirBitsUp = 6- i / 8;
//        int dirBitsDown = i / 8 - 1;
//        int dirBitsLeft = 6- i % 8;
//        int dirBitsRight = (i % 8) - 1;
//
//        int dirBits45 = dirBitsRight < dirBitsUp ? dirBitsRight : dirBitsUp;
//        int dirBits225= dirBitsLeft < dirBitsDown ? dirBitsLeft  : dirBitsDown;
//        int dirBits135 = dirBitsUp < dirBitsLeft ? dirBitsUp : dirBitsLeft;
//        int dirBits315 = dirBitsDown < dirBitsRight ? dirBitsDown : dirBitsRight;
//
//        dirCombination dirComb[] = {{dirBits45, pow(2, dirBits45), i - dirBits45 + 8, 45},
//                                    {dirBits225, pow(2, dirBits225), i - (dirBits225)*7 + 1 - dirBits225, 45},
//                                    {dirBits135, pow(2, dirBits135),  i + 1 + dirBits135, 135},
//                                    {dirBits315, pow(2, dirBits315),  i - 9*dirBits315 - 8 + dirBits315, 135}
//        };
//        int initSq = bishopMagicSqInit(i, dirComb);
//        assert(initSq == 1 && "error- bishopInit");
//    }
//}
