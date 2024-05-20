#include <stdio.h>
#include <string.h>

#define MAX_GRAMMAR 10
#define MAX_RULE_LEN 10

// Function prototypes
void findFirst(char grammar[][MAX_RULE_LEN], int numberOfGrammar, char firstSets[][MAX_GRAMMAR]);
void findFollow(char grammar[][MAX_RULE_LEN], int numberOfGrammar, char firstSets[][MAX_GRAMMAR], char followSets[][MAX_GRAMMAR]);

int main() {
    int numberOfGrammar;
    printf("Enter the number of grammar rules: ");
    scanf("%d", &numberOfGrammar);

    char grammar[numberOfGrammar][MAX_RULE_LEN];
    for (int i = 0; i < numberOfGrammar; i++) {
        printf("Enter the grammar rule (format: A->BC): ");
        scanf("%s", grammar[i]);
    }

    char firstSets[numberOfGrammar][MAX_GRAMMAR];
    char followSets[numberOfGrammar][MAX_GRAMMAR];

    // Initialize first and follow sets to empty
    for (int i = 0; i < numberOfGrammar; i++) {
        for (int j = 0; j < MAX_GRAMMAR; j++) {
            firstSets[i][j] = '\0';
            followSets[i][j] = '\0';
        }
    }

    findFirst(grammar, numberOfGrammar, firstSets);
    findFollow(grammar, numberOfGrammar, firstSets, followSets);

    printf("\nFirst Sets:\n");
    for (int i = 0; i < numberOfGrammar; i++) {
        printf("First(%c) = { ", grammar[i][0]);
        for (int j = 0; firstSets[i][j] != '\0'; j++) {
            printf("%c ", firstSets[i][j]);
        }
        printf("}\n");
    }

    printf("\nFollow Sets:\n");
    for (int i = 0; i < numberOfGrammar; i++) {
        printf("Follow(%c) = { ", grammar[i][0]);
        for (int j = 0; followSets[i][j] != '\0'; j++) {
            printf("%c ", followSets[i][j]);
        }
        printf("}\n");
    }

    return 0;
}

void findFirst(char grammar[][MAX_RULE_LEN], int numberOfGrammar, char firstSets[][MAX_GRAMMAR]) {
    int changes;
    do {
        changes = 0;
        for (int i = 0; i < numberOfGrammar; i++) {
            char nonTerminal = grammar[i][0];
            int index = 3; // Skip 'A->'
            int firstIndex = 0;

            while (grammar[i][index] != '\0') {
                char symbol = grammar[i][index];
                if (symbol >= 'A' && symbol <= 'Z') { // Non-terminal
                    int found = 0;
                    for (int j = 0; j < numberOfGrammar; j++) {
                        if (grammar[j][0] == symbol) {
                            int k = 3; // Skip 'B->'
                            while (grammar[j][k] != '\0') {
                                if (grammar[j][k] >= 'a' && grammar[j][k] <= 'z') { // Terminal
                                    if (firstSets[i][firstIndex] != grammar[j][k]) {
                                        firstSets[i][firstIndex++] = grammar[j][k];
                                        changes = 1;
                                    }
                                    found = 1;
                                    break;
                                } else {
                                    // If the non-terminal has epsilon production, add epsilon to first set
                                    if (grammar[j][k] == 'e') {
                                        if (firstSets[i][firstIndex] != 'e') {
                                            firstSets[i][firstIndex++] = 'e';
                                            changes = 1;
                                        }
                                    } else {
                                        // If non-terminal doesn't have epsilon, find its first set
                                        int l = 0;
                                        while (grammar[j][k] != grammar[l][0]) {
                                            l++;
                                        }
                                        int m = 3;
                                        while (grammar[l][m] != '\0') {
                                            if (grammar[l][m] >= 'a' && grammar[l][m] <= 'z') {
                                                if (firstSets[i][firstIndex] != grammar[l][m]) {
                                                    firstSets[i][firstIndex++] = grammar[l][m];
                                                    changes = 1;
                                                }
                                                break;
                                            }
                                            m++;
                                        }
                                    }
                                    if (!found) break;
                                }
                            }
                        }
                        if (!found) break;
                    }
                } else { // Terminal
                    if (firstSets[i][firstIndex] != symbol) {
                        firstSets[i][firstIndex++] = symbol;
                        changes = 1;
                    }
                    break;
                }
                index++;
            }
            firstSets[i][firstIndex] = '\0'; // Null-terminate the first set
        }
    } while (changes);
}

void findFollow(char grammar[][MAX_RULE_LEN], int numberOfGrammar, char firstSets[][MAX_GRAMMAR], char followSets[][MAX_GRAMMAR]) {
    // Initialize follow sets
    for (int i = 0; i < numberOfGrammar; i++) {
        for (int j = 0; j < MAX_GRAMMAR; j++) {
            followSets[i][j] = '\0';
        }
    }
    
    // Add '$' (end of input marker) to the follow set of the start symbol
    followSets[0][0] = '$';

    // Iterate until the follow sets stabilize
    int changes;
    do {
        changes = 0;
        for (int i = 0; i < numberOfGrammar; i++) {
            char nonTerminal = grammar[i][0];
            int index = 3; // Skip 'A->'

            while (grammar[i][index] != '\0') {
                char symbol = grammar[i][index];

                if (symbol >= 'A' && symbol <= 'Z') { // Non-terminal
                    int nextSymbol = grammar[i][index + 1];
                    int isEpsilon = 0;
                    int hasEpsilon = 0;

                    if (nextSymbol == '\0' || nextSymbol == '|') {
                        hasEpsilon = 1;
                    } else if (nextSymbol == 'e') {
                        hasEpsilon = 1;
                        isEpsilon = 1;
                    }

                    if (hasEpsilon) {
                        for (int j = 0; j < numberOfGrammar; j++) {
                            if (grammar[j][0] == nonTerminal) {
                                int k = index + 1;
                                int followIndex = 0;

                                while (grammar[j][k] != '\0') {
                                    if (grammar[j][k] >= 'A' && grammar[j][k] <= 'Z') { // Non-terminal
                                        for (int l = 0; firstSets[grammar[j][k] - 'A'][l] != '\0'; l++) {
                                            char firstSymbol = firstSets[grammar[j][k] - 'A'][l];
                                            if (firstSymbol != 'e') {
                                                int exists = 0;
                                                for (int m = 0; followSets[i][m] != '\0'; m++) {
                                                    if (followSets[i][m] == firstSymbol) {
                                                        exists = 1;
                                                        break;
                                                    }
                                                }
                                                if (!exists) {
                                                    followSets[i][followIndex++] = firstSymbol;
                                                    changes = 1;
                                                }
                                            }
                                        }
                                    } else { // Terminal
                                        int exists = 0;
                                        for (int m = 0; followSets[i][m] != '\0'; m++) {
                                            if (followSets[i][m] == grammar[j][k]) {
                                                exists = 1;
                                                break;
                                            }
                                        }
                                        if (!exists) {
                                            followSets[i][followIndex++] = grammar[j][k];
                                            changes = 1;
                                        }
                                    }
                                    k++;
                                }
                            }
                        }
                    }
                }
                index++;
            }
        }
    } while (changes);
}
