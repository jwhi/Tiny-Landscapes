#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define Y_MAX 1000
#define X_MAX 1000

// foreground ESC[38;5;#m   background: ESC[48;5;#m
#define COLOR_RED "\033[38;5;196m"
#define COLOR_WHITE "\033[38;5;255m"
#define COLOR_GREEN "\033[38;5;40m"
#define COLOR_BLUE "\033[38;5;33m"

#define BORDER_TOP_LEFT "\u2554"        // ╔
#define BORDER_TOP_RIGHT "\u2557"       // ╗
#define BORDER_TOP_BOTTOM "\u2550"      // ═
#define BORDER_BOTTOM_LEFT "\u255A"     // ╚
#define BORDER_BOTTOM_RIGHT "\u255D"    // ╝
#define BORDER_LEFT_RIGHT "\u2551"      // ║

struct region {
    char background[50];
    char foreground[50];
    char symbol;
    float height;
};

/*
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
                    */

struct region regions[10] = {
        {.background = "", .foreground = "", .symbol = ' ', .height = 0.01 }, // Deep Water
        {.background = "", .foreground = "", .symbol = ' ', .height = 0.05 }, // Shallow Water
        {.background = "", .foreground = "", .symbol = '.', .height = 0.06 }, // Sand
        {.background = "", .foreground = "", .symbol = ',', .height = 0.08 }, // Grass
        {.background = "", .foreground = "", .symbol = ':', .height = 0.1 }, // Rocky
        {.background = "", .foreground = "", .symbol = '"', .height = 0.15 }, // Mountain Meadow
        {.background = "", .foreground = "", .symbol = '*', .height = 0.2 }, // Mountain
        {.background = "", .foreground = "", .symbol = '%', .height = 0.5 }, // Mountain tops
        {.background = "", .foreground = "", .symbol = '&', .height = 0.9 }, // Snow
        {.background = "", .foreground = "", .symbol = '#', .height = 1 }, // Ice
    };


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


    printf("%sGenerating gradient...\n", COLOR_RED);
    GenerateGradient();

    printf("%s", BORDER_TOP_LEFT);
    for (int x = 0; x < mapWidth; x++) {
        printf("%s", BORDER_TOP_BOTTOM);
        if (drawSymbols == 0) {
            printf("%s%s%s%s", BORDER_TOP_BOTTOM, BORDER_TOP_BOTTOM, BORDER_TOP_BOTTOM, BORDER_TOP_BOTTOM);
        }
    }
    printf("%s\n", BORDER_TOP_RIGHT);

    for (int y = 0; y < mapHeight; y++) {
        printf("%s", BORDER_LEFT_RIGHT);
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
        printf("%s\n", BORDER_LEFT_RIGHT);
    }

    printf("%s", BORDER_BOTTOM_LEFT);
    for (int x = 0; x < mapWidth; x++) {
        printf("%s", BORDER_TOP_BOTTOM);
        if (drawSymbols == 0) {
            printf("%s%s%s%s", BORDER_TOP_BOTTOM, BORDER_TOP_BOTTOM, BORDER_TOP_BOTTOM, BORDER_TOP_BOTTOM);
        }
    }
    printf("%s\n", BORDER_BOTTOM_RIGHT);
    
    printf("%sBLUE\n%sGREEN\n%sRED\n%sWHITE\n", COLOR_BLUE, COLOR_GREEN, COLOR_RED, COLOR_WHITE);

    return 0;
}
