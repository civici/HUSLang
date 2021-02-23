#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ENTRY_POINT 0x12
#define VARIABLE 0x03

struct text{

    char* value;
    int len;
    char* name;

};

struct function{

    int startingLine;
    int endingLine;
    int returnType;
};

static const char* dataTypes[] = {"None", "text", "decimal"};

int definedTextsCount = 0;
struct text* definedTexts;

void addText(struct text userText)
{
    struct text* temp = malloc((definedTextsCount + 1) * sizeof(struct text));

    for (int i = 0; i < definedTextsCount; i++)
    {
        temp[i] = definedTexts[i];
    }
    temp[definedTextsCount] = userText;

    free(definedTexts);
    definedTexts = temp;
    definedTextsCount++;

}

void append(char** string, char aChar)
{
    int len = *string != NULL ? strlen(*string) : 0;
    char* temp = malloc(len + 2);
    for (int i = 0; i < len; i++)
    {
        temp[i] = (*string)[i];
    }
    temp[len] = aChar;
    temp[len + 1] = 0;
    len++;
    free(*string);
    *string = temp;
}

void analyzeLine(char** lineArgs, int argCount)
{
    for (int i = 0; i < argCount; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (strcmp(dataTypes[j], lineArgs[i]) == 0)
            {
                if (strcmp(lineArgs[i + 2], "=") == 0)
                {
                    if (lineArgs[i + 3][0] == '"' && lineArgs[i + 3][strlen(lineArgs[i + 3]) - 1] == '"')
                    {
                        struct text* userText = malloc(sizeof(struct text));
                        userText->name = lineArgs[i + 1];
                        char* realVal = NULL;
                        for (int k = 0; k < strlen(lineArgs[i + 3]); k++)
                        {
                            if (k != 0 && k != strlen(lineArgs[i + 3]) - 1)
                            {
                                append(&realVal, lineArgs[i + 3][k]);
                            }
                        }
                        userText->value = realVal;
                        userText->len = strlen(realVal);
                        addText(*userText);
                        break;
                    }
                }
            }
        
        }

        for (int j = 0; j < strlen(lineArgs[i]); j++)
        {
            if (lineArgs[i][j] == '(')
            {
                char* funcName = NULL;
                for (int b = 0; b < j; b++)
                {
                    append(&funcName, lineArgs[i][b]);
                }
                char* funcArgs = NULL;
                j++;
                for ( ; j < strlen(lineArgs[i]); j++)
                {
                    if (lineArgs[i][j] == ')')
                    {
                        if (strcmp(funcName, "print") == 0)
                        {
                            for (int textIndex = 0; textIndex < definedTextsCount; textIndex++)
                            {
                                if (strcmp(definedTexts[textIndex].name, funcArgs) == 0)
                                {
                                    puts(definedTexts[textIndex].value);
                                }
                            }
                        }
                        break;
                    }
                    append(&funcArgs, lineArgs[i][j]);
                }
            }
        }

    }
}

void splitLine(char* line)
{
    char** args;
    int argCount = 1;
    for (int i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == ' ')
            argCount++;
    }
    args = malloc(sizeof(char*) * argCount);
    int argsIndex = 0;

    char* temp = NULL;

    int lineLen = strlen(line);
    for (int i = 0; i < lineLen; i++)
    {
        if (line[i] == ' ')
        {
            if (i == 0){
                if (line[i + 1] == ' '){
                    continue;
                }
            } else if (i == lineLen - 1){
                if (line[i - 1] == ' '){
                    continue;
                }
            } else {
                if (line[i - 1] == ' ' || line[i + 1] == ' '){
                    continue;
                }
            }
            args[argsIndex] = temp;
            argsIndex++;
            temp = NULL;
            continue;
        }
        append(&temp, line[i]);
        if (i == lineLen - 1)
        {
            args[argsIndex] = temp;
            temp = NULL;
        }
    }

    analyzeLine(args, argsIndex + 1);

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
            append(&lineBuffer, currentChar);
            currentChar = fgetc(file);
        }
        splitLine(lineBuffer);
        lineCount++;
        lineBuffer = NULL;

    }
}

int main()
{
    parse();
    puts("done");
    return 0;
}