#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main()
{
    char s[5][5];
    for (int i = 0; i < 5; i++)
    {
        scanf("%s", s[i]);
    }
        for (int i = 0; i < 5; i++)
    {
        printf("%s", s[i]);
    }

}