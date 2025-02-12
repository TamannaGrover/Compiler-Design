#include <stdio.h>
#include <string.h>
#define SIZE 100

//DIRECT LEFT RECURSION CASE
void removeLeftRecursion(char production[10][SIZE], int num) {
    char non_terminal;
    char alpha[SIZE], beta[SIZE];
    int index;

    for (int i = 0; i < num; i++) {
        printf("\nGRAMMAR: %s", production[i]);
        non_terminal = production[i][0];
        index = 3; // Skip "A->"

        if (non_terminal == production[i][index]) {
            printf(" is left recursive.\n");

            int j = 0, k = 0;
            index++; // Move past recursive non-terminal

            while (production[i][index] != 0 && production[i][index] != '|') {
                alpha[j++] = production[i][index++];
            }
            alpha[j] = '\0';

            if (production[i][index] == '|') {
                index++;
                j = 0;

                while (production[i][index] != '\0') {
                    beta[j++] = production[i][index++];
                }
                beta[j] = '\0';

                printf("Grammar without left recursion:\n");
                printf("%c->%s%c'\n", non_terminal, beta, non_terminal);
                printf("%c'->%s%c'|ε\n", non_terminal, alpha, non_terminal);
            } else {
                printf("Can't be reduced (no alternative production)\n");
            }
        } else {
            printf(" is not left recursive.\n");
        }
    }
}


//-----------------------------------------------WRITTEN TILL ABOVE------------------------------------

//INDIRECT LEFT RECURSION CASE
void handleIndirectLeftRecursion(char production[10][SIZE], int num) {
    for (int i = 0; i < num; i++) {
        char A = production[i][0]; // Current non-terminal

        for (int j = 0; j < i; j++) {
            char B = production[j][0]; // Previous non-terminal

            if (production[i][3] == B) { // If A->B...
                printf("\nIndirect recursion detected: %s", production[i]);

                char temp[SIZE];
                strcpy(temp, production[i] + 3); // Remove "A->"
                strcpy(production[i], ""); // Clear current production

                char *token = strtok(temp, "|");
                while (token) {
                    char newProd[SIZE];
                    if (token[0] == B) {
                        // Replace B with its productions
                        sprintf(newProd, "%c->%s%s", A, production[j] + 3, token + 1);
                    } else {
                        sprintf(newProd, "%c->%s", A, token);
                    }
                    printf("\nNew Production: %s", newProd);
                    token = strtok(NULL, "|");
                }
            }
        }
    }
}



//REMOVING LEFT FACTORING
void removeLeftFactoring(char *gram) {
    char part1[SIZE], part2[SIZE], modifiedGram[SIZE], newGram[SIZE];
    int i, j = 0, k = 0, pos = -1;

    for (i = 0; gram[i] != '|' && gram[i] != '\0'; i++) {
        part1[j++] = gram[i];
    }
    part1[j] = '\0';

    if (gram[i] == '|') i++;

    for (j = 0; gram[i] != '\0'; i++, j++) {
        part2[j] = gram[i];
    }
    part2[j] = '\0';

    for (i = 0; i < strlen(part1) && i < strlen(part2); i++) {
        if (part1[i] == part2[i]) {
            modifiedGram[k++] = part1[i];
            pos = i + 1;
        } else {
            break;
        }
    }
    modifiedGram[k] = 'X';
    modifiedGram[++k] = '\0';

    j = 0;
    for (i = pos; part1[i] != '\0'; i++) {
        newGram[j++] = part1[i];
    }
    if (j > 0) newGram[j++] = '|';
    for (i = pos; part2[i] != '\0'; i++) {
        newGram[j++] = part2[i];
    }
    newGram[j] = '\0';

    printf("\nA -> %s\n", modifiedGram);
    printf("X -> %s | E\n", newGram);
}




// Main function to run the program
int main() {
    int num;
    char production[10][SIZE];
    char grammar[SIZE];

    printf("Enter Number of Productions: ");
    scanf("%d", &num);
    getchar(); // Consume newline character

    printf("Enter the grammar as A->A'ALPHA'|'beta' :\n");
    for (int i = 0; i < num; i++) {
        fgets(production[i], SIZE, stdin);
        production[i][strcspn(production[i], "\n")] = '\0'; // Remove trailing newline
    }

    handleIndirectLeftRecursion(production, num);
    removeLeftRecursion(production, num);

    printf("\nEnter Production for Left Factoring (Format: A->xy|xz): ");
    fgets(grammar, SIZE, stdin);
    grammar[strcspn(grammar, "\n")] = '\0'; // Remove trailing newline

    removeLeftFactoring(grammar);

    return 0;
}
