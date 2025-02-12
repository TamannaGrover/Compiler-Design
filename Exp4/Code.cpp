#include <stdio.h>
#include <string.h>
#define SIZE 100 // Increased size to handle larger inputs
 
void removeLeftRecursion(char production[10][SIZE], int num) {
    char non_terminal;
    char alpha[SIZE], beta[SIZE];
    int index;
 
    for (int i = 0; i < num; i++) {
        printf("\nGRAMMAR: %s", production[i]);
        non_terminal = production[i][0];
        index = 3; // Start checking after "A->"
 
        if (non_terminal == production[i][index]) {
            printf(" is left recursive.\n");
 
            // Extract alpha and beta
            int j = 0, k = 0;
            index++; // Move past the recursive non-terminal
 
            // Extract alpha (recursive part)
            while (production[i][index] != 0 && production[i][index] != '|') {
                alpha[j++] = production[i][index++];
            }
            alpha[j] = '\0'; // Null-terminate the string
 
            if (production[i][index] == '|') {
                index++; // Move past '|'
                j = 0;
 
                // Extract beta (non-recursive part)
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
 
void removeLeftFactoring(char *gram) {
    char part1[SIZE], part2[SIZE], modifiedGram[SIZE], newGram[SIZE];
    int i, j = 0, k = 0, l = 0, pos = -1;
 
    // Extract first alternative (before '|')
    for (i = 0; gram[i] != '|' && gram[i] != '\0'; i++) {
        part1[j++] = gram[i];
    }
    part1[j] = '\0';
 
    if (gram[i] == '|') {
        i++; // Move past '|'
    }
 
    // Extract second alternative (after '|')
    for (j = 0; gram[i] != '\0'; i++, j++) {
        part2[j] = gram[i];
    }
    part2[j] = '\0';
 
    // Find common prefix
    for (i = 0; i < strlen(part1) && i < strlen(part2); i++) {
        if (part1[i] == part2[i]) {
            modifiedGram[k++] = part1[i];
            pos = i + 1;
        } else {
            break;
        }
    }
    modifiedGram[k] = 'X'; // New non-terminal
    modifiedGram[++k] = '\0';
 
    // Create new productions
    j = 0;
    for (i = pos; part1[i] != '\0'; i++) {
        newGram[j++] = part1[i];
    }
    if (j > 0) {
        newGram[j++] = '|';
    }
    for (i = pos; part2[i] != '\0'; i++) {
        newGram[j++] = part2[i];
    }
    newGram[j] = '\0';
 
    printf("\nA -> %s\n", modifiedGram);
    printf("X -> %s | E\n", newGram);
}
 
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
 
    removeLeftRecursion(production, num);
 
    printf("\nEnter Production for Left Factoring (Format: A->xy|xz): ");
    fgets(grammar, SIZE, stdin);
    grammar[strcspn(grammar, "\n")] = '\0'; // Remove trailing newline
 
    removeLeftFactoring(grammar);
 
    return 0;
}