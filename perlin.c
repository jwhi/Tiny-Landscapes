#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// const int Y_MAX = 1000;
// const int X_MAX = 1000;
#define Y_MAX 1000
#define X_MAX 1000



int seed = -1;

float Gradient[Y_MAX][X_MAX][2];

/* Function to linearly interpolate between a0 and a1
 * Weight w should be in the range [0.0, 1.0]
 *
 * as an alternative, this slightly faster equivalent function (macro) can be used:
 * #define lerp(a0, a1, w) ((a0) + (w)*((a1) - (a0))) 
 */
float lerp(float a0, float a1, float w) {
    return (1.0f - w)*a0 + w*a1;
}

float RandomPercent() {
    return (double)rand() / (double)RAND_MAX;
}

void GenerateGradient() {
    if (seed < 0) {
        srand(time(0));
    } else {
        srand(seed);
    }
    for (int y = 0; y < Y_MAX; y++) {
        for (int x = 0; x < X_MAX; x++) {
            Gradient[y][x][0] = RandomPercent();
            Gradient[y][x][1] = RandomPercent();
        }
    }
}


// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y) {
    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    return (dx*Gradient[iy][ix][0] + dy*Gradient[iy][ix][1]);
}

// Compute Perlin noise at coordinates x, y
float perlin(float x, float y) {

    // Determine grid cell coordinates
    int x0 = (int)x;
    int x1 = x0 + 1;
    int y0 = (int)y;
    int y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    // Interpolate between grid point gradients
    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = lerp(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = lerp(n0, n1, sx);

    value = lerp(ix0, ix1, sy);
    return value;
}

/*
u255A : ╚
u255D : ╝
u2554 : ╔
u2557 : ╗
u2551 : ║
u2550 : ═

*/

// ARGS {weight} {seed} {draw_mode}
// draw modes:
// 0: decimal value
// 1: detailed zones
// 2: simple zones
int main(int argc, char *argv[]) {
    printf("Hello World!\n");
    int mapWidth = 100;
    int mapHeight = 30;

    float scale = 10.8001f;
    if (argc >= 2) {
        scale = atof(argv[1]);
    }
    if (argc >= 3) {
        seed =  atoi(argv[2]);
    }

    int drawSymbols = 1;
    if (argc >= 4) {
        drawSymbols = atoi(argv[3]);
    }


    printf("Generating gradient...\n");
    GenerateGradient();


    printf("%s", "\u2554");
    for (int x = 0; x < mapWidth; x++) {
        printf("%s", "\u2550");
        if (drawSymbols == 0) {
            printf("\u2550\u2550\u2550\u2550");
        }
    }
    printf("%s", "\u2557\n");

    for (int y = 0; y < mapHeight; y++) {
        printf("%s", "\u2551");
        for (int x = 0; x < mapWidth; x++) {
            float sampleX = x / scale;
            float sampleY = y / scale;
            float noiseValue = fabs(perlin(sampleX, sampleY));
            float gradientValue = 1 - lerp(1, 0, noiseValue);
            char drawChar = '_';

            if (drawSymbols == 1) {
                if (gradientValue <= 0.01) {
                    drawChar = ' ';
                } else if (gradientValue <= 0.05) {
                    drawChar = ' ';
                } else if (gradientValue <= 0.06) {
                    drawChar = '.';
                } else if (gradientValue <= 0.08) {
                    drawChar = ',';
                } else if (gradientValue <= 0.1) {
                    drawChar = ':';
                } else if (gradientValue <= 0.15) {
                    drawChar = '*';
                } else if (gradientValue <= 0.2) {
                    drawChar = '%';
                } else if (gradientValue <= 0.9) {
                    drawChar = '&';
                } else if (gradientValue <= 1) {
                    drawChar = '#';
                } else {
                    drawChar = 'E';
                }
            } else {
                
                if (gradientValue <= 0.05) {
                    drawChar = ' ';
                } else if (gradientValue <= 0.1) {
                    drawChar = '.';
                } else {
                    drawChar = '#';
                }
            }
            if (drawSymbols > 0) {
                printf("%c", drawChar);
            } else {
                printf("%0.2f ", gradientValue);
            }
        }
        printf("%s\n", "\u2551");
    }

    printf("%s", "\u255A");
    for (int x = 0; x < mapWidth; x++) {
        printf("%s", "\u2550");
        if (drawSymbols == 0) {
            printf("\u2550\u2550\u2550\u2550");
        }
    }
    printf("%s", "\u255D\n");

    /*
    printf("Perlin for 10,10: %f\n\n\n", perlin(0.5f, 0.4f));
    for (int y = 0; y < Y_MAX; y++) {
        for (int x = 0; x < X_MAX; x++) {
            printf("%0.2f %0.2f, ", Gradient[y][x][0], Gradient[y][x][1]);
        }
        printf("\n");
    }
    */
    return 0;
}
