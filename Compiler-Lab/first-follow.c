#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX 10

int n;
char production[MAX][MAX][MAX];
char nonTerminals[MAX];
char first[MAX][MAX];
char follow[MAX][MAX];
bool visitedFirst[MAX] = { false };
bool visitedFollow[MAX] = { false };
int firstCount[MAX] = { 0 };
int followCount[MAX] = { 0 };
int alternatives[MAX] = { 0 };

void addUnique(char* set, int* count, char c) {
    for(int i = 0; i < *count; i++) {
        if(set[i] == c) return;
    }
    set[(*count)++] = c;
}

void findFirst(int index) {
    if (visitedFirst[index]) return;
    visitedFirst[index] = true;

    for(int j = 0; j < alternatives[index]; j++) {
        for(int k = 0; k < strlen(production[index][j]); k++) {
            char symbol = production[index][j][k];
            if (islower(symbol) || symbol == 'i') {
                addUnique(first[index], &firstCount[index], symbol);
                break;
            } else if (symbol == 'e') {
                addUnique(first[index], &firstCount[index], '$');
                break;
            } else {
                int nextIndex = -1;
                for (int l = 0; l < n; l++) {
                    if(nonTerminals[l] == symbol) {
                        nextIndex = l;
                        break;
                    }
                }
                if (nextIndex != -1) {
                    findFirst(nextIndex);
                    bool epsilonFound = false;
                    for (int m = 0; m < firstCount[nextIndex]; m++) {
                        if(first[nextIndex][m] == '$') {
                            epsilonFound = true;
                        } else {
                            addUnique(first[index], &firstCount[index], first[nextIndex][m]);
                        }
                    }
                    if (!epsilonFound) break;
                }
            }
        }
    }
}

void findFollow(int index) {
    if (visitedFollow[index]) return;
    visitedFollow[index] = true;

    if (index == 0) {
        addUnique(follow[index], &followCount[index], '$');
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < alternatives[i]; j++) {
            for(int k = 0; k < strlen(production[i][j]); k++) {
                if(production[i][j][k] == nonTerminals[index]) {
                    if(production[i][j][k+1] != '\0') {
                        for(int l = 0; l < n; l++) {
                            if(nonTerminals[l] == production[i][j][k+1]) {
                                for(int m = 0; m < firstCount[l]; m++) {
                                    if(first[l][m] != '$') {
                                        addUnique(follow[index], &followCount[index], first[l][m]);
                                    }
                                }
                                if(strchr(first[l], '$')) {
                                    findFollow(i);
                                    for(int m = 0; m < followCount[i]; m++) {
                                        addUnique(follow[index], &followCount[index], follow[i][m]);
                                    }
                                }
                                break;
                            } else {
                                addUnique(follow[index], &followCount[index], production[i][j][k+1]);
                                break;
                            }
                        }
                    } else if(production[i][j][k+1] == '\0' && nonTerminals[i] != nonTerminals[index]) {
                        findFollow(i);
                        for(int m = 0; m < followCount[i]; m++) {
                            addUnique(follow[index], &followCount[index], follow[i][m]);
                        }
                    }
                }
            }
        }
    }
}

int main() {
    printf("Enter the number of non-terminals: ");
    scanf("%d", &n);

    for(int i = 0; i < n; i++) {
        printf("Enter the non-terminal: ");
        scanf(" %c", &nonTerminals[i]);
        printf("Enter the number of alternatives for %c: ", nonTerminals[i]);
        scanf("%d", &alternatives[i]);

        for(int j = 0; j < alternatives[i]; j++) {
            printf("Enter alternative %d for %c: ", j+1, nonTerminals[i]);
            scanf("%s", production[i][j]);
        }
    }

    for(int i = 0; i < n; i++) {
        findFirst(i);
    }

    printf("First:\n");
    for(int i = 0; i < n; i++) {
        printf("%c: ", nonTerminals[i]);
        for(int j = 0; j < firstCount[i]; j++) {
            printf("%c ", first[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i < n; i++) {
        findFollow(i);
    }

    printf("Follow:\n");
    for(int i = 0; i < n; i++) {
        printf("%c: ", nonTerminals[i]);
        for(int j = 0; j < followCount[i]; j++) {
            printf("%c ", follow[i][j]);
        }
        printf("\n");
    }

    return 0;
}
