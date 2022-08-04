#include "menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*********************************************************************
 * Function:     clearBuffer
 *--------------------------------------------------------------------
 * Description:  Clear the type-ahead buffer.
 ********************************************************************/
static void clearBuffer() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    char tmp;
    do {
        scanf("%c", &tmp);
    } while (tmp != '\n');
#pragma GCC diagnostic pop
}

int getNumber(const char *prompt, int min, int max, int *result) {
    char input[3] = {'\0'};
    fprintf(stdout, "%s\n", prompt);
    int valid = scanf("%2[0123456789]", input);
    clearBuffer();
    fprintf(stdout, "\n");

    if (valid) {
        *result = atoi(input);
        if ((*result < min) || (*result > max)) {
            valid = 0;
        }
    }
    return valid;
}

int getNfromUser() {
    int valid = 0, n;
    do {
        clear();
        valid = getNumber("Enter number of shares:\n<min> = 2\n<max> = 8\n", 2, 8, &n);
    } while (!valid);

    return n;
}

int getKfromUser(int n) {
    int valid = 0, k;
    char prompt[50];
    memset(prompt, '\0', sizeof(prompt));
    snprintf(prompt, sizeof(prompt), "Enter number for k:\n<min> = 2\n<max> = %d\n", n);
    do {
        clear();
        valid = getNumber(prompt, 2, n, &k);
    } while (!valid);

    return k;
}

int getMenu(const char *title, char **menuItem, int numChoices, const char *prompt) {
    int choice = 0;  // user input value
    int valid;

    do {
        clear();
        fprintf(stdout, "%s\n\n", title);

        for (int i = 0; i < numChoices; i++) {
            fprintf(stdout, "%i. ", i + 1);
            fprintf(stdout, "%s\n", *(menuItem + i));
        }
        fprintf(stdout, "\n");

        valid = getNumber(prompt, 1, numChoices, &choice);
    } while (!valid);

    return choice;
}
