#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

// ANSI Escape Codes for colors and cursor control
#define CLEAR_SCREEN "\033[2J"
#define CURSOR_HOME "\033[H"
#define RESET "\033[0m"
#define BRIGHT_BLACK "\033[90m"
#define WHITE "\033[37m"
#define BRIGHT_WHITE "\033[97m"
#define ORANGE "\033[38;5;202m"
#define BRIGHT_ORANGE "\033[38;5;214m"

// ASCII Art for Ubuntu Logo
const char *ubuntu_logo[] = {
    "               .-.",
    "         .-'``(|||)",
    "      ,`\\ \\    `-`.               88                         88",
    "     /   \\ '``-.   `              88                         88",
    "   .-.  ,       `___:    88   88  88,888,  88   88  ,88888, 88888  88   88",
    "  (:::) :        ___     88   88  88   88  88   88  88   88  88    88   88",
    "   `-`  `       ,   :    88   88  88   88  88   88  88   88  88    88   88",
    "     \\   / ,..-`   ,     88   88  88   88  88   88  88   88  88    88   88",
    "      `./ /    .-.`      '88888'  '88888'  '88888'  88   88  '8888 '88888'",
    "         `-..-(   )",
    "               `-`"
};

const char *brightness_levels[] = {
    "\033[90m", // Bright Black (Dark Grey)
    "\033[37m", // White (Grey)
    "\033[97m", // Bright White
};

void clear_screen() {
    printf("%s", CLEAR_SCREEN);
    printf("%s", CURSOR_HOME);
}

// Function to get terminal size
void get_terminal_size(int *rows, int *cols) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *rows = w.ws_row;
    *cols = w.ws_col;
}

// Function to print Ubuntu logo gradually at the center of the terminal with increasing brightness
void print_ubuntu_logo_gradually() {
    int rows, cols;
    get_terminal_size(&rows, &cols);

    int logo_height = sizeof(ubuntu_logo) / sizeof(ubuntu_logo[0]);
    int logo_width = strlen(ubuntu_logo[0]);

    int start_row = (rows - logo_height) / 2;
    int start_col = (cols - logo_width) / 2;

    clear_screen();

    for (int i = 0; i < start_row; i++) {
        printf("\n");
    }

    // Print the logo line by line with increasing brightness
    for (int b = 0; b < 3; b++) {
        for (int i = 0; i < logo_height; i++) {
            printf("%*s%s%s%s\n", start_col, "", brightness_levels[b], ubuntu_logo[i], RESET);
        }
        fflush(stdout); // Flush the output buffer
        usleep(500000); // Sleep for 0.5 seconds
        clear_screen();
    }

    // Final print with bright white
    for (int i = 0; i < logo_height; i++) {
        printf("%*s%s%s%s\n", start_col, "", BRIGHT_WHITE, ubuntu_logo[i], RESET);
    }

    // Print "Booting up..." message and progress bar
    printf("\n\n%*sBooting up...\n", (cols - 12) / 2, "");
    printf("%*s[", (cols - 32) / 2, "");
    fflush(stdout);
    for (int i = 0; i < 30; i++) {
        printf("#");
        fflush(stdout);
        usleep(200000); // Sleep for 0.2 seconds to simulate loading
    }
    printf("]\n");
}

int main() {
    print_ubuntu_logo_gradually();
    sleep(2); // Display for 2 seconds after completion
    clear_screen();
    return 0;
}
