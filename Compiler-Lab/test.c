#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define nonTerminal s[i]>='A' && s[i]<='Z'
#define terminal s[i]>='a' && s[i]<='z'

int numberOfGrammer;
char grammer[12][12];

void first(char s[], int i)
{
    for(i = 2; i < strlen(s); i++)
    {
        if (terminal || s[i]=='|')
        {
            fprintf(stdout, "%c, ", s[i]);
            break;
        }
        else if (nonTerminal)
        {
            for(int j=0; j<numberOfGrammer; j++)
            {
                if (s[i] == grammer[j][0])
                {
                    first(grammer[j], 2);
                }
            }
        }
    }   
}

int main()
{
    freopen("grammar.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    fscanf(stdin, "%d", &numberOfGrammer);
    
    for (int i = 0; i < numberOfGrammer; i++)
    {
        fscanf(stdin, "%s", grammer[i]);
        
    }
    
    for (int i = 0; i < numberOfGrammer; i++)
    {
        fprintf(stdout, "First of %c: ", grammer[i][0]);
        first(grammer[i], 2);
        fprintf(stdout, "\n");
    }
    
}
