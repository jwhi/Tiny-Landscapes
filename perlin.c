#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define Y_MAX 1000
#define X_MAX 1000

// foreground ESC[38;5;#m   background: ESC[48;5;#m
#define COLOR_RED "\033[38;5;196m"
#define COLOR_WHITE "\033[38;5;255m"
#define COLOR_GREEN "\033[38;5;40m"
#define COLOR_BLUE "\033[38;5;33m"

#define BACKGROUND_DARK_BLUE "\033[48;5;19m"
#define BACKGROUND_BLUE "\033[48;5;21m"
#define BACKGROUND_LIGHT_BLUE "\033[48;5;26m"

#define BACKGROUND_SAND "\033[48;5;187m"

#define BACKGROUND_CLIFF "\033[48;5;243m"
#define BACKGROUND_MOUNTAIN "\033[48;5;240m"
#define BACKGROUND_MOUNTAIN_SNOW "\033[48;5;15m"

#define BACKGROUND_GRASS "\033[48;5;22m"
#define BACKGROUND_MOUNTAIN_GRASS "\033[48;5;34m"

#define FOREGROUND_DARK_BLUE "\033[38;5;19m"
#define FOREGROUND_BLUE "\033[38;5;21m"
#define FOREGROUND_LIGHT_BLUE "\033[38;5;26m"

#define FOREGROUND_SAND "\033[38;5;187m"

#define FOREGROUND_CLIFF "\033[38;5;243m"
#define FOREGROUND_MOUNTAIN "\033[38;5;240m"
#define FOREGROUND_MOUNTAIN_SNOW "\033[38;5;15m"

#define FOREGROUND_GRASS_1 "\033[38;5;22m"
#define FOREGROUND_MOUNTAIN_GRASS_1 "\033[38;5;34m"

#define FOREGROUND_TREE "\033[38;5;106m"
#define FOREGROUND_FOREST "\033[38;5;28m"
#define FOREGROUND_WHEAT "\033[38;5;220m"
#define FOREGROUND_GRASS_2 "\033[38;5;76m"
#define FOREGROUND_FLOWER_PINK "\033[38;5;219m"
#define FOREGROUND_FLOWER_PURPLE "\033[38;5;13m"
#define FOREGROUND_MOUNTAIN_GRASS_2 "\033[38;5;112m"
#define FOREGROUND_MOUNTAIN_WHEAT "\033[38;5;221m"
#define FOREGROUND_MOUNTAIN_WEEDS "\033[38;5;22m"
#define FOREGROUND_ROCKS "\033[38;5;249m"

#define COLOR_RESET "\e[0m"

#define REGION_COUNT 11
#define FOLIAGE_COUNT 11

#define BORDER_TOP_LEFT "\u2554"        // ╔
#define BORDER_TOP_RIGHT "\u2557"       // ╗
#define BORDER_TOP_BOTTOM "\u2550"      // ═
#define BORDER_BOTTOM_LEFT "\u255A"     // ╚
#define BORDER_BOTTOM_RIGHT "\u255D"    // ╝
#define BORDER_LEFT_RIGHT "\u2551"      // ║

struct region {
    char background[13];
    char foreground[13];
    char symbol;
    float height;
};

struct foliage {
    char background[12];
    char foreground[12];
    char symbol[4];         // 4 characters long to support unicode characters.
    float spawnRange[2];
    float percantage;
};

struct region regions[REGION_COUNT] = {
        {.background = BACKGROUND_DARK_BLUE, .foreground = FOREGROUND_DARK_BLUE, .symbol = '~', .height = 0.02 }, // Deep Water
        {.background = BACKGROUND_BLUE, .foreground = FOREGROUND_BLUE, .symbol = '-', .height = 0.03 }, // Water
        {.background = BACKGROUND_LIGHT_BLUE, .foreground = FOREGROUND_LIGHT_BLUE, .symbol = '.', .height = 0.05 }, // Shallow Water
        {.background = BACKGROUND_SAND, .foreground = FOREGROUND_SAND, .symbol = '.', .height = 0.07 }, // Sand
        {.background = BACKGROUND_GRASS, .foreground = FOREGROUND_GRASS_1, .symbol = ',', .height = 0.15 }, // Grass
        {.background = BACKGROUND_MOUNTAIN_GRASS, .foreground = FOREGROUND_MOUNTAIN_GRASS_1, .symbol = ':', .height = 0.19 }, // Mountain Meadow
        {.background = BACKGROUND_CLIFF, .foreground = FOREGROUND_CLIFF, .symbol = '"', .height = 0.27 }, // Rocky
        {.background = BACKGROUND_MOUNTAIN, .foreground = FOREGROUND_MOUNTAIN, .symbol = '*', .height = 0.5 }, // Mountain
        {.background = BACKGROUND_MOUNTAIN_SNOW, .foreground = FOREGROUND_MOUNTAIN_SNOW, .symbol = '%', .height = 0.55 }, // Mountain tops
        {.background = COLOR_RESET, .foreground = COLOR_RESET, .symbol = '&', .height = 0.9 }, // Snow
        {.background = COLOR_RESET, .foreground = COLOR_RESET, .symbol = '#', .height = 1 }, // Ice
    };

struct foliage foliageTypes[FOLIAGE_COUNT] = {
    {.background = "", .foreground = FOREGROUND_TREE, .symbol = "\u2660", .spawnRange = {0.085, 0.135}, .percantage = .125},         // Club forests
    {.background = "", .foreground = FOREGROUND_FOREST, .symbol = "\u2663", .spawnRange = {0.08, 0.14}, .percantage = .012},    // Spade trees
    {.background = "", .foreground = FOREGROUND_WHEAT, .symbol = "\"", .spawnRange = {0.08, 0.1}, .percantage = .23},            // Yellow Grass/Wheat
    {.background = "", .foreground = FOREGROUND_GRASS_2, .symbol = "\"", .spawnRange = {0.08, 0.1}, .percantage = .2},            // Grass
    {.background = "", .foreground = FOREGROUND_FLOWER_PINK, .symbol = "\u273F", .spawnRange = {0.168, 0.177}, .percantage = .01},            // Yellow Grass/Wheat
    {.background = "", .foreground = FOREGROUND_FLOWER_PURPLE, .symbol = "\u273F", .spawnRange = {0.165, 0.174}, .percantage = .01},            // Yellow Grass/Wheat
    {.background = "", .foreground = FOREGROUND_MOUNTAIN_GRASS_2, .symbol = "\"", .spawnRange = {0.165, 0.176}, .percantage = .03},            // Yellow Grass/Wheat
    {.background = "", .foreground = FOREGROUND_MOUNTAIN_WHEAT, .symbol = "\"", .spawnRange = {0.155, 0.185}, .percantage = .03},            // Yellow Grass/Wheat
    {.background = "", .foreground = FOREGROUND_MOUNTAIN_WEEDS, .symbol = "\u2448", .spawnRange = {0.137, 0.8}, .percantage = .003},            // Yellow Grass/Wheat
    {.background = "", .foreground = FOREGROUND_ROCKS, .symbol = "\u25AA", .spawnRange = {0.2, 0.865}, .percantage = .01},      // Boulders
    {.background = "", .foreground = FOREGROUND_ROCKS, .symbol = "\u25AA", .spawnRange = {0.085, 0.13}, .percantage = .01}      // Boulders
    /*
     * ☘	2618	 	SHAMROCK
     * ☙	2619	 	REVERSED ROTATED FLORAL HEART BULLET
     * ♨	2668	 	HOT SPRINGS
     * ♣  	2663    	BLACK CLUB SUIT
     * ⛆	26C6	 	RAIN
     * ❝     275D      Inverted Fancy Quote
     * ❞     275E      Fancy Quote
     * ✿    273F       Flower
     * ❟     275F     Comma
     * ❠     2760      Double comma
     * ❦     2766
     * ❧     2767
     * ●      25CF
     * ⑆      2446
     * ⑇      2447
     * ⑈      2448
     * ▖    2596
     * ▗    2597
     * ▘    2598
     * ▝    259D
     * ■    25A0
     * ▪    25AA
     */
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

    GenerateGradient();

    printf("%s%s", COLOR_RESET, BORDER_TOP_LEFT);
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
            char drawChar = 'E';

            if (drawSymbols == 1) {                    
                int foliageDrawn = 0;
                for (int i = 0; i < REGION_COUNT; i++) {
                    if (gradientValue <= regions[i].height) {
                        for (int j = 0; j < FOLIAGE_COUNT; j++) {
                            struct foliage f = foliageTypes[j];
                            if (gradientValue > f.spawnRange[0] && gradientValue <= f.spawnRange[1]) {
                                if (RandomPercent() <= f.percantage) {
                                    if (strlen(f.background) > 0) {
                                        printf("%s%s%s", f.background, f.foreground, f.symbol);
                                    } else {
                                        printf("%s%s%s", regions[i].background, f.foreground, f.symbol);
                                    }
                                    foliageDrawn = 1;
                                }
                            }
                            if (foliageDrawn == 1) {
                                break;
                            }
                        }
                        if (foliageDrawn == 0) {
                            printf("%s%c", regions[i].background, ' ');
                        }
                        break;
                    }
                }
            } else if (drawSymbols == 2) {
                for (int i = 0; i < REGION_COUNT; i++) {
                    if (gradientValue <= regions[i].height) {
                        printf("%s%c", regions[i].background, ' '); // regions[i].symbol);
                        break;
                    }
                }
            } else if (drawSymbols == 3) {
                for (int i = 0; i < REGION_COUNT; i++) {
                    if (gradientValue <= regions[i].height) {
                        printf("%s%c", regions[i].foreground, regions[i].symbol);
                        break;
                    }
                }
            }else if (drawSymbols == 4) {
                for (int i = 0; i < REGION_COUNT; i++) {
                    if (gradientValue <= regions[i].height) {
                        printf("%s%s%c", regions[i].background, regions[i].foreground, regions[i].symbol);
                        break;
                    }
                }
            } else if (drawSymbols == 5) {
                for (int i = 0; i < REGION_COUNT; i++) {
                    if (gradientValue <= regions[i].height) {
                        printf("%s%c", regions[i].background, regions[i].symbol);
                        break;
                    }
                }
            } else {
                printf("%0.2f ", gradientValue);
            }
        }
        printf("%s%s\n", COLOR_RESET, BORDER_LEFT_RIGHT);
    }

    printf("%s", BORDER_BOTTOM_LEFT);
    for (int x = 0; x < mapWidth; x++) {
        printf("%s", BORDER_TOP_BOTTOM);
        if (drawSymbols == 0) {
            printf("%s%s%s%s", BORDER_TOP_BOTTOM, BORDER_TOP_BOTTOM, BORDER_TOP_BOTTOM, BORDER_TOP_BOTTOM);
        }
    }
    printf("%s\n", BORDER_BOTTOM_RIGHT);

    return 0;
}
