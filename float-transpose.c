#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>

#include <xmmintrin.h>

#define TEST_W 4096
#define TEST_H 4096

void sse_transpose(float *src, float *dst, int w, int h)
{
    for (int x = 0; x < w; x += 4) {
        for (int y = 0; y < h; y += 4) {
            __m128 I0 = _mm_loadu_ps((src + (y + 0) * w + x));
            __m128 I1 = _mm_loadu_ps((src + (y + 1) * w + x));
            __m128 I2 = _mm_loadu_ps((src + (y + 2) * w + x));
            __m128 I3 = _mm_loadu_ps((src + (y + 3) * w + x));
            __m128 T0 = _mm_unpacklo_ps(I0, I1);
            __m128 T1 = _mm_unpacklo_ps(I2, I3);
            __m128 T2 = _mm_unpackhi_ps(I0, I1);
            __m128 T3 = _mm_unpackhi_ps(I2, I3);
            I0 = _mm_unpacklo_ps(T0, T1);
            I1 = _mm_unpackhi_ps(T0, T1);
            I2 = _mm_unpacklo_ps(T2, T3);
            I3 = _mm_unpackhi_ps(T2, T3);
            _mm_storeu_ps((dst + ((x + 0) * h) + y), I0);
            _mm_storeu_ps((dst + ((x + 1) * h) + y), I1);
            _mm_storeu_ps((dst + ((x + 2) * h) + y), I2);
            _mm_storeu_ps((dst + ((x + 3) * h) + y), I3);
        }
    }
}


static long diff_in_us(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec * 1000000.0 + diff.tv_nsec / 1000.0);
}

int main()
{
    /* verify the result of 4x4 matrix */
    {
        float testin[16] = { 0, 1,  2,  3,  4,  5,  6,  7,
                             8, 9, 10, 11, 12, 13, 14, 15
                           };
        float testout[16];
        float expected[16] = { 0, 4,  8, 12, 1, 5,  9, 13,
                               2, 6, 10, 14, 3, 7, 11, 15
                             };

        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++)
                printf(" %2f", testin[y * 4 + x]);
            printf("\n");
        }
        printf("\n");
        sse_transpose(testin, testout, 4, 4);
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++)
                printf(" %2f", testout[y * 4 + x]);
            printf("\n");
        }
        assert(0 == memcmp(testout, expected, 16 * sizeof(int)) &&
               "Verification fails");
    }

    {
        struct timespec start, end;
        float *src  = (float *) malloc(sizeof(float) * TEST_W * TEST_H);
        float *out1 = (float *) malloc(sizeof(float) * TEST_W * TEST_H);

        srand(time(NULL));
        for (int y = 0; y < TEST_H; y++)
            for (int x = 0; x < TEST_W; x++)
                *(src + y * TEST_W + x) = (float)rand();

        clock_gettime(CLOCK_REALTIME, &start);
        sse_transpose(src, out1, TEST_W, TEST_H);
        clock_gettime(CLOCK_REALTIME, &end);
        printf("sse: \t\t %ld us\n", diff_in_us(start, end));

        free(src);
        free(out1);
    }

    return 0;
}
