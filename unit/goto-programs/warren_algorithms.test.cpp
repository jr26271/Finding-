
#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
#include <goto-programs/abstract-interpretation/warren_approximation.h>

/**
 * Compute extrema of x | y by visiting all possible values of x and y
 * @author Edoardo Manino.
 * @brief Extrema of x | y given x in [a,b] and y in [c,d], potentially negative
 * @param min_or Pointer to the minimum value result
 * @param max_or Pointer to the maximum value result
 * @param a Min value of variable x
 * @param b Max value of variable x
 * @param c Min value of variable y
 * @param d Max value of variable y
 */
void brute_force_or(INT_X* min_or, INT_X* max_or, INT_X a, INT_X b, INT_X c, INT_X d)
{
    INT_X i, j;

    *max_or = b | d;
    *min_or = a | c;

    for(i = a; i <= b; i++)
        for(j = c; j <= d; j++) {
            INT_X res = i | j;
            if(res > *max_or)
                *max_or = res;
            if(res < *min_or)
                *min_or = res;
        }
}

/**
 * Compute extrema of x & y by visiting all possible values of x and y
 * @author Edoardo Manino.
 * @brief Extrema of x & y given x in [a,b] and y in [c,d], potentially negative
 * @param min_and Pointer to the minimum value result
 * @param max_and Pointer to the maximum value result
 * @param a Min value of variable x
 * @param b Max value of variable x
 * @param c Min value of variable y
 * @param d Max value of variable y
 */
void brute_force_and(INT_X* min_and, INT_X* max_and, INT_X a, INT_X b, INT_X c, INT_X d)
{
    INT_X i, j;

    *max_and = b & d;
    *min_and = a & c;

    for(i = a; i <= b; i++)
        for(j = c; j <= d; j++) {
            INT_X res = i & j;
            if(res > *max_and)
                *max_and = res;
            if(res < *min_and)
                *min_and = res;
        }
}

/**
 * Compute extrema of x ^ y by visiting all possible values of x and y
 * @author Edoardo Manino.
 * @brief Extrema of x ^ y given x in [a,b] and y in [c,d], potentially negative
 * @param min_xor Pointer to the minimum value result
 * @param max_xor Pointer to the maximum value result
 * @param a Min value of variable x
 * @param b Max value of variable x
 * @param c Min value of variable y
 * @param d Max value of variable y
 */
void brute_force_xor(INT_X* min_xor, INT_X* max_xor, INT_X a, INT_X b, INT_X c, INT_X d)
{
    INT_X i, j;

    *max_xor = b ^ d;
    *min_xor = a ^ c;

    for(i = a; i <= b; i++)
        for(j = c; j <= d; j++) {
            INT_X res = i ^ j;
            if(res > *max_xor)
                *max_xor = res;
            if(res < *min_xor)
                *min_xor = res;
        }
}

/**
 * Compute extrema of ~x by visiting all possible values of x
 * @author Edoardo Manino.
 * @brief Extrema of ~x given x in [a,b], potentially negative
 * @param min_not Pointer to the minimum value result
 * @param max_not Pointer to the maximum value result
 * @param a Min value of variable x
 * @param b Max value of variable x
 */
void brute_force_not(INT_X* min_not, INT_X* max_not, INT_X a, INT_X b)
{
    INT_X i;

    *max_not = ~a;
    *min_not = ~b;

    for(i = a; i <= b; i++) {
        INT_X res = ~i;
        if(res > *max_not)
            *max_not = res;
        if(res < *min_not)
            *min_not = res;
    }
}

/**
 * Compute extrema of x << y by visiting all possible values of x and y
 * @author Edoardo Manino.
 * @brief Extrema of x << y given x in [a,b] and y in [c,d], potentially negative
 * @param min_lsh Pointer to the minimum value result
 * @param max_lsh Pointer to the maximum value result
 * @param a Min value of variable x
 * @param b Max value of variable x
 * @param c Min value of variable y
 * @param d Max value of variable y
 */
void brute_force_left_shift(INT_X* min_lsh, INT_X* max_lsh, INT_X a, INT_X b, INT_X c, INT_X d)
{
    INT_X i, j;

    *max_lsh = b << d;
    *min_lsh = a << c;

    for(i = a; i <= b; i++)
        for(j = c; j <= d; j++) {
            INT_X res = i << j;
            if(res > *max_lsh)
                *max_lsh = res;
            if(res < *min_lsh)
                *min_lsh = res;
        }
}

/**
 * Compute extrema of x >> y by visiting all possible values of x and y
 * @author Edoardo Manino.
 * @brief Extrema of x >> y given x in [a,b] and y in [c,d], potentially negative
 * @param min_rsh Pointer to the minimum value result
 * @param max_rsh Pointer to the maximum value result
 * @param a Min value of variable x
 * @param b Max value of variable x
 * @param c Min value of variable y
 * @param d Max value of variable y
 */
void brute_force_right_shift(INT_X* min_rsh, INT_X* max_rsh, INT_X a, INT_X b, INT_X c, INT_X d)
{
    INT_X i, j;

    *max_rsh = b >> c;
    *min_rsh = a >> d;

    for(i = a; i <= b; i++)
        for(j = c; j <= d; j++) {
            INT_X res = i >> j;
            if(res > *max_rsh)
                *max_rsh = res;
            if(res < *min_rsh)
                *min_rsh = res;
        }
}

/**
 * Compute the extrema of unsigned trunc(x, n) by visiting all possible values of x
 * @author Edoardo Manino.
 * @brief Extrema of unsigned trunc(x, n) given n bits and x in [a,b] potentially negative
 * @param min_trunc Pointer to the minimum value result
 * @param max_trunc Pointer to the maximum value result
 * @param a Min value of variable x
 * @param b Max value of variable x
 * @param n Number of bits to keep
 */
void brute_force_unsigned_truncate(UINT_X* min_trunc, UINT_X* max_trunc, INT_X a, INT_X b, UINT_X n)
{
    INT_X i;

    UINT_X mask = (1 << n) - 1;
    *max_trunc = b & mask;
    *min_trunc = a & mask;

    for(i = a; i <= b; i++) {
        UINT_X res = i & mask;
        if(res > *max_trunc)
            *max_trunc = res;
        if(res < *min_trunc)
            *min_trunc = res;
    }
}

/**
 * Compute the extrema of signed trunc(x, n) by visiting all possible values of x
 * @author Edoardo Manino.
 * @brief Extrema of signed trunc(x, n) given n bits and x in [a,b] potentially negative
 * @param min_trunc Pointer to the minimum value result
 * @param max_trunc Pointer to the maximum value result
 * @param a Min value of variable x
 * @param b Max value of variable x
 * @param n Number of bits to keep
 */
void brute_force_signed_truncate(INT_X* min_trunc, INT_X* max_trunc, INT_X a, INT_X b, UINT_X n)
{
    INT_X i;

    *max_trunc = unsigned_2_signed(b, n);
    *min_trunc = unsigned_2_signed(a, n);

    for(i = a; i <= b; i++) {
        INT_X res = unsigned_2_signed(i, n);
        if(res > *max_trunc)
            *max_trunc = res;
        if(res < *min_trunc)
            *min_trunc = res;
    }
}

/**
 * Compute the extrema of unsigned extend(x, n) by visiting all possible values of x
 * @author Edoardo Manino.
 * @brief Extrema of unsigned extend(x, n) given n bits and x in [a,b] potentially negative
 * @param min_trunc Pointer to the minimum value result
 * @param max_trunc Pointer to the maximum value result
 * @param a Min value of variable x
 * @param b Max value of variable x
 * @param n Number of bits in the input
 */
void brute_force_unsigned_extend(UINT_X* min_ext, UINT_X* max_ext, INT_X a, INT_X b, UINT_X n)
{
    INT_X i;

    UINT_X mask = (1 << n) - 1;
    *max_ext = b & mask;
    *min_ext = a & mask;

    for(i = a; i <= b; i++) {
        UINT_X res = i & mask;
        if(res > *max_ext)
            *max_ext = res;
        if(res < *min_ext)
            *min_ext = res;
    }
}

/**
 * Compute the extrema of signed extend(x, n) by visiting all possible values of x
 * @author Edoardo Manino.
 * @brief Extrema of signed extend(x, n) given n bits and unsigned x in [a,b]
 * @param min_trunc Pointer to the minimum value result
 * @param max_trunc Pointer to the maximum value result
 * @param a Min value of variable x
 * @param b Max value of variable x
 * @param n Number of bits in the input
 */
void brute_force_signed_extend(INT_X* min_ext, INT_X* max_ext, UINT_X a, UINT_X b, UINT_X n)
{
    INT_X i;

    UINT_X sign_bit = 1 << (n - 1);
    *max_ext = unsigned_2_signed(b, n);
    *min_ext = unsigned_2_signed(a, n);

    for(i = a; i <= b; i++) {
        INT_X res = ((i & sign_bit) != 0)? i | (-sign_bit): i;
        if(res > *max_ext)
            *max_ext = res;
        if(res < *min_ext)
            *min_ext = res;
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for unsigned_max_or() and unsigned_min_or()
 */
void test_unsigned_or()
{
    int a[] = {0, 13, 256, 4638, 77583};
    int b[] = {4, 13, 286, 6548, 78583};
    int c[] = {2, 11, 567,  333, 76784};
    int d[] = {3, 67, 599, 5789, 78787};
    int i;

    for(i = 0; i < 5; i++) {

        INT_X ref_min, ref_max;
        brute_force_or(&ref_min, &ref_max, a[i], b[i], c[i], d[i]);

        INT_X val_min = unsigned_min_or(a[i], b[i], c[i], d[i]);
        INT_X val_max = unsigned_max_or(a[i], b[i], c[i], d[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for unsigned_max_and() and unsigned_min_and()
 */
void test_unsigned_and()
{
    int a[] = {0, 13, 256, 4638, 77583};
    int b[] = {4, 13, 286, 6548, 78583};
    int c[] = {2, 11, 567,  333, 76784};
    int d[] = {3, 67, 599, 5789, 78787};
    int i;

    for(i = 0; i < 5; i++) {

        INT_X ref_min, ref_max;
        brute_force_and(&ref_min, &ref_max, a[i], b[i], c[i], d[i]);

        INT_X val_min = unsigned_min_and(a[i], b[i], c[i], d[i]);
        INT_X val_max = unsigned_max_and(a[i], b[i], c[i], d[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for unsigned_max_xor() and unsigned_min_xor()
 */

void test_unsigned_xor()
{
    int a[] = {0, 13, 256, 4638, 77583};
    int b[] = {4, 13, 286, 6548, 78583};
    int c[] = {2, 11, 567,  333, 76784};
    int d[] = {3, 67, 599, 5789, 78787};
    int i;

    for(i = 0; i < 5; i++) {

        INT_X ref_min, ref_max;
        brute_force_xor(&ref_min, &ref_max, a[i], b[i], c[i], d[i]);

        INT_X val_min = unsigned_min_xor(a[i], b[i], c[i], d[i]);
        INT_X val_max = unsigned_max_xor(a[i], b[i], c[i], d[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for unsigned_max_not() and unsigned_min_not()
 */
void test_unsigned_not()
{
    int a[] = {0, 13, 256, 4638, 77583};
    int b[] = {4, 13, 286, 6548, 78583};
    int i;

    for(i = 0; i < 5; i++) {

        INT_X ref_min, ref_max;
        brute_force_not(&ref_min, &ref_max, a[i], b[i]);

        INT_X val_min = unsigned_min_not(a[i], b[i]);
        INT_X val_max = unsigned_max_not(a[i], b[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for unsigned_max_left_shift() and unsigned_min_left_shift()
 */
void test_unsigned_left_shift()
{
    int a[] = {0, 13, 256, 4638, 77583};
    int b[] = {4, 13, 286, 6548, 78583};
    int c[] = {0,  1,  12,    3,     4};
    int d[] = {3,  1,  17,    5,     8};
    int i;

    for(i = 0; i < 5; i++) {

        INT_X ref_min, ref_max;
        brute_force_left_shift(&ref_min, &ref_max, a[i], b[i], c[i], d[i]);

        INT_X val_min = unsigned_min_left_shift(a[i], b[i], c[i], d[i]);
        INT_X val_max = unsigned_max_left_shift(a[i], b[i], c[i], d[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for unsigned_max_right_shift() and unsigned_min_right_shift()
 */
void test_unsigned_right_shift()
{
    int a[] = {0, 13, 256, 4638, 77583};
    int b[] = {4, 13, 286, 6548, 78583};
    int c[] = {0,  1,  12,    3,     4};
    int d[] = {3,  1,  17,    5,     8};
    int i;

    for(i = 0; i < 5; i++) {

        INT_X ref_min, ref_max;
        brute_force_right_shift(&ref_min, &ref_max, a[i], b[i], c[i], d[i]);

        INT_X val_min = unsigned_min_right_shift(a[i], b[i], c[i], d[i]);
        INT_X val_max = unsigned_max_right_shift(a[i], b[i], c[i], d[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for unsigned_2_unsigned_max_truncate() and unsigned_2_unsigned_min_truncate()
 */
void test_unsigned_2_unsigned_truncate()
{
    int a[] = {0, 13, 256, 4638, 77583};
    int b[] = {4, 13, 286, 6548, 78583};
    int n[] = {3,  1,  17,    5,     8};
    int i;

    for(i = 0; i < 5; i++) {

        UINT_X ref_min, ref_max;
        brute_force_unsigned_truncate(&ref_min, &ref_max, a[i], b[i], n[i]);

        UINT_X val_min = unsigned_2_unsigned_min_truncate(a[i], b[i], n[i]);
        UINT_X val_max = unsigned_2_unsigned_max_truncate(a[i], b[i], n[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for signed_2_unsigned_max_truncate() and signed_2_unsigned_min_truncate()
 */
void test_signed_2_unsigned_truncate()
{
    int a[] = {0, -13, -256, -6548, -78583};
    int b[] = {4,  13,   15, -4638, -77583};
    int n[] = {3,  1,    17,     5,     12};
    int i;

    for(i = 0; i < 5; i++) {

        UINT_X ref_min, ref_max;
        brute_force_unsigned_truncate(&ref_min, &ref_max, a[i], b[i], n[i]);

        UINT_X val_min = signed_2_unsigned_min_truncate(a[i], b[i], n[i]);
        UINT_X val_max = signed_2_unsigned_max_truncate(a[i], b[i], n[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for signed_2_unsigned_max_extend() and signed_2_unsigned_min_extend()
 */
void test_signed_2_unsigned_extend()
{
    int a[] = {0, -13, -256, -6548, -78583};
    int b[] = {4,  13,   15, -4638, -77583};
    int n[] = {3,  5,    11,    14,     17};
    int i;

    for(i = 0; i < 5; i++) {

        UINT_X ref_min, ref_max;
        brute_force_unsigned_extend(&ref_min, &ref_max, a[i], b[i], n[i]);

        UINT_X val_min = signed_2_unsigned_min_extend(a[i], b[i], n[i]);
        UINT_X val_max = signed_2_unsigned_max_extend(a[i], b[i], n[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for signed_max_or() and signed_min_or()
 */
void test_signed_or()
{
    int a[] = {-435, -345, -231, -13, -24, -11,  345, 763, 8903};
    int b[] = {-432, -321, -221,  24,  55,  34,  446, 984, 9045};
    int c[] = {-234,  -67,   13, -89, -32, 456, -234, -89,  732};
    int d[] = {-166,   45,   64, -77,  26, 572, -211,  44,  871};
    int i;

    for(i = 0; i < 9; i++) {

        INT_X ref_min, ref_max;
        brute_force_or(&ref_min, &ref_max, a[i], b[i], c[i], d[i]);

        INT_X val_min = signed_min_or(a[i], b[i], c[i], d[i]);
        INT_X val_max = signed_max_or(a[i], b[i], c[i], d[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for signed_max_and() and signed_min_and()
 */
void test_signed_and()
{
    int a[] = {-435, -345, -231, -13, -24, -11,  345, 763, 8903};
    int b[] = {-432, -321, -221,  24,  55,  34,  446, 984, 9045};
    int c[] = {-234,  -67,   13, -89, -32, 456, -234, -89,  732};
    int d[] = {-166,   45,   64, -77,  26, 572, -211,  44,  871};
    int i;

    for(i = 0; i < 9; i++) {

        INT_X ref_min, ref_max;
        brute_force_and(&ref_min, &ref_max, a[i], b[i], c[i], d[i]);

        INT_X val_min = signed_min_and(a[i], b[i], c[i], d[i]);
        INT_X val_max = signed_max_and(a[i], b[i], c[i], d[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for signed_max_xor() and signed_min_xor()
 */
void test_signed_xor()
{
    int a[] = {-435, -345, -231, -13, -24, -11,  345, 763, 8903};
    int b[] = {-432, -321, -221,  24,  55,  34,  446, 984, 9045};
    int c[] = {-234,  -67,   13, -89, -32, 456, -234, -89,  732};
    int d[] = {-166,   45,   64, -77,  26, 572, -211,  44,  871};
    int i;

    for(i = 0; i < 9; i++) {

        INT_X ref_min, ref_max;
        brute_force_xor(&ref_min, &ref_max, a[i], b[i], c[i], d[i]);

        INT_X val_min = signed_min_xor(a[i], b[i], c[i], d[i]);
        INT_X val_max = signed_max_xor(a[i], b[i], c[i], d[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for signed_max_not() and signed_min_not()
 */
void test_signed_not()
{
    int a[] = {-435, -13, 0, 1, 345};
    int b[] = {-432,  24, 0, 1, 446};
    int i;

    for(i = 0; i < 5; i++) {

        INT_X ref_min, ref_max;
        brute_force_not(&ref_min, &ref_max, a[i], b[i]);

        INT_X val_min = signed_min_not(a[i], b[i]);
        INT_X val_max = signed_max_not(a[i], b[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for signed_max_left_shift() and signed_min_left_shift()
 */
void test_signed_left_shift()
{
    int a[] = {-435, -345, -231, -13, -24, -11,  345, 763, 8903};
    int b[] = {-432, -321, -221,  24,  55,  34,  446, 984, 9045};
    int c[] = {   0,    1,   12,   3,   4,   0,    1,  12,    3};
    int d[] = {   3,    1,   17,   5,   8,   3,    1,  17,    5};
    int i;

    for(i = 0; i < 9; i++) {

        INT_X ref_min, ref_max;
        brute_force_left_shift(&ref_min, &ref_max, a[i], b[i], c[i], d[i]);

        INT_X val_min = signed_min_left_shift(a[i], b[i], c[i], d[i]);
        INT_X val_max = signed_max_left_shift(a[i], b[i], c[i], d[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for signed_max_right_shift() and signed_min_right_shift()
 */
void test_signed_right_shift()
{
    int a[] = {-435, -345, -231, -13, -24, -11,  345, 763, 8903};
    int b[] = {-432, -321, -221,  24,  55,  34,  446, 984, 9045};
    int c[] = {   0,    1,   12,   3,   4,   0,    1,  12,    3};
    int d[] = {   3,    1,   17,   5,   8,   3,    1,  17,    5};
    int i;

    for(i = 0; i < 9; i++) {

        INT_X ref_min, ref_max;
        brute_force_right_shift(&ref_min, &ref_max, a[i], b[i], c[i], d[i]);

        INT_X val_min = signed_min_right_shift(a[i], b[i], c[i], d[i]);
        INT_X val_max = signed_max_right_shift(a[i], b[i], c[i], d[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for unsigned_2_signed()
 */
void test_unsigned_2_signed()
{
    CHECK(unsigned_2_signed(10, 5) == 10);
    CHECK(unsigned_2_signed(10, 4) == -6);
    CHECK(unsigned_2_signed(10, 3) == 2);
    CHECK(unsigned_2_signed(10, 2) == -2);
    CHECK(unsigned_2_signed(10, 1) == 0);
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for unsigned_2_signed_max_truncate() and unsigned_2_signed_min_truncate()
 */
void test_unsigned_2_signed_truncate()
{
    int a[] = {0, 13, 256, 4638, 77583};
    int b[] = {4, 13, 286, 6548, 78583};
    int n[] = {3,  1,  17,    5,     8};
    int i;

    for(i = 0; i < 5; i++) {

        INT_X ref_min, ref_max;
        brute_force_signed_truncate(&ref_min, &ref_max, a[i], b[i], n[i]);

        INT_X val_min = unsigned_2_signed_min_truncate(a[i], b[i], n[i]);
        INT_X val_max = unsigned_2_signed_max_truncate(a[i], b[i], n[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for signed_2_signed_max_truncate() and signed_2_signed_min_truncate()
 */
void test_signed_2_signed_truncate()
{
    int a[] = {0, -13, -256, -6548, -78583};
    int b[] = {4,  13,   15, -4638, -77583};
    int n[] = {3,  1,    17,     5,     12};
    int i;

    for(i = 0; i < 5; i++) {

        INT_X ref_min, ref_max;
        brute_force_signed_truncate(&ref_min, &ref_max, a[i], b[i], n[i]);

        INT_X val_min = signed_2_signed_min_truncate(a[i], b[i], n[i]);
        INT_X val_max = signed_2_signed_max_truncate(a[i], b[i], n[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

/**
 * @author Edoardo Manino.
 * @brief Unit test for unsigned_2_signed_max_extend() and unsigned_2_signed_min_extend()
 */
void test_unsigned_2_signed_extend()
{
    int a[] = {0, 13,  15, 4638, 77583};
    int b[] = {4, 13, 256, 6548, 78583};
    int n[] = {3,  4,  11,   13,    17};
    int i;

    for(i = 0; i < 5; i++) {

        INT_X ref_min, ref_max;
        brute_force_signed_extend(&ref_min, &ref_max, a[i], b[i], n[i]);

        INT_X val_min = unsigned_2_signed_min_extend(a[i], b[i], n[i]);
        INT_X val_max = unsigned_2_signed_max_extend(a[i], b[i], n[i]);

        CHECK(val_min == ref_min);
        CHECK(val_max == ref_max);
    }
}

TEST_CASE(
  "Warren algorithms tests",
  "[ai][interval-analysis]")
{
    test_unsigned_or();
    test_unsigned_and();
    test_unsigned_xor();
    test_unsigned_not();
    test_unsigned_left_shift();
    test_unsigned_right_shift();
    test_unsigned_2_unsigned_truncate();
    test_signed_2_unsigned_truncate();
    test_signed_2_unsigned_extend();
    test_signed_or();
    test_signed_and();
    test_signed_xor();
    test_signed_not();
    test_signed_left_shift();
    test_signed_right_shift();
    test_unsigned_2_signed();
    test_unsigned_2_signed_truncate();
    test_signed_2_signed_truncate();
    test_unsigned_2_signed_extend();
}