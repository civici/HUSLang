#include <stdio.h>
#include <stdlib.h>

#define ENTRY_POINT 0x12

void executeScope(char* scope, int len)
{
    if (scope[0] == '-' && scope[1] == '-' && scope[len - 1] == '-' && scope[len - 2] == '-'){
        puts("entryponit");
    }

    for (int i = 0; scope[i] != '\0'; i++)
    {
        if (! (scope[i] <= 'z' && scope[i] >= '0' ) && scope[i] != ' '){
            if (scope[i] == '('){
                for ( ; scope[i] != '\0'; i++)
                {
                    if (scope[i] == ')'){
                        puts("func");
                    }
                }
            }            
        }
    }
}

void append(char** string, char aChar, int* len)
{
    char* temp = malloc(*len + 2);
    for (int i = 0; i < *len; i++)
    {
        temp[i] = (*string)[i];
    }
    temp[*len] = aChar;
    temp[*len + 1] = 0;
    (*len)++;
    free(*string);
    *string = temp;
}

void parse()
{
    FILE* file = fopen("main.hus", "r");
    int lineCount = 0;
    int lineIndex;
    char currentChar = 0;
    while (currentChar != EOF)
    {
        currentChar = fgetc(file);
        char* lineBuffer = NULL;
        int lineBufferCount = 0;
        for (int lineIndex = 1; currentChar != EOF && currentChar != '\n'; lineIndex++)
        {
            append(&lineBuffer, currentChar, &lineBufferCount);
            currentChar = fgetc(file);
        }
        if (currentChar != EOF)
        {
            //printf("line is %s\n", lineBuffer);
            lineCount++;
            executeScope(lineBuffer, lineBufferCount);
        }
    }
}

int main()
{
    parse();
    puts("done");
    return 0;
}