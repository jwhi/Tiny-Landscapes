/****************************************
 * Test file to decide colors.
 ***************************************/

#include <stdio.h>

// foreground ESC[38;5;#m   background: ESC[48;5;#m
#define COLOR_RED "\033[38;5;196m"
#define COLOR_WHITE "\033[38;5;255m"
#define COLOR_GREEN "\033[38;5;40m"
#define COLOR_BLUE "\033[38;5;33m"

#define COLOR_RESET "\e[0m"
// #define COLOR_RESET "\e[39;49m"

int main() {

    for (int x = 0; x < 256; x++) {
        char backgroundColor[100];
        snprintf(backgroundColor, sizeof(backgroundColor), "\033[%d;5;%dm", 48, x);

        char foregroundColor[100];
        snprintf(foregroundColor, sizeof(foregroundColor), "\033[%d;5;%dm", 38, 255);

        
        printf("%s%s%d", backgroundColor, foregroundColor, x);
        if (x % 20 == 0) {
            printf("%s\n", COLOR_RESET);
        }
    }


    for (int x = 0; x < 256; x++) {
        char backgroundColor[100];
        snprintf(backgroundColor, sizeof(backgroundColor), "\033[%d;5;%dm", 48, 0);

        char foregroundColor[100];
        snprintf(foregroundColor, sizeof(foregroundColor), "\033[%d;5;%dm", 38, x);

        
        printf("%s%s%d", backgroundColor, foregroundColor, x);
        if (x % 20 == 0) {
            printf("%s\n", COLOR_RESET);
        }
    }
    return 0;
}