#include <stdio.h>  /* perror */
#include <stdlib.h> /* EXIT_FAILURE, EXIT_SUCCESS */
#include <string.h> /* strlen, strrchr */
#include <stdbool.h>/* bool, true, false */
#include <stdint.h> /* uint8_t */
#include <libgen.h> /* basename */
#include <math.h>   /* pow */
#include <ctype.h>  /* isalnum */
#include "trace.h"  /* TRACE_MSG */

#define MAX_LINE_SIZE 128
#define PIXEL_WIDTH 7

void chomp(char *s)
{
    size_t len, i;

    len = strlen(s);
    for (i = len; i > 0; )
        if (s[--i] == '\r' || s[i] == '\n')
            s[i] = '\0';
}

void reverse(char *s)
{
    size_t len, i, j;
    char c;

    len = strlen(s);
    for (i = 0, j = len; i < j && j > 1; ++i) {
        --j;
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

int convertReverseBinaryStringToInt(char *s)
{
    size_t i, len;
    int value = 0;

    len = strlen(s);
    for (i = 0; i < len; ++i)
        if (s[i] == '1')
            value += pow(2, i);
        else if (s[i] != '0')
            break;

    return value;
}

char *makeIdentName(char *s)
{
    size_t i, len;

    len = strlen(s);
    for (i = 0; i < len; i++)
        if (! isalnum(s[i]) && s[i] != '_')
            s[i] = '_';

    return s;
}

char *removeExtension(char *fileName)
{
    char *ext;

    ext = strrchr(fileName, '.');
    if (ext != NULL)
        *ext = '\0';

    return fileName;
}

char *readLine(FILE *f, char *name)
{
    static char line[MAX_LINE_SIZE];
    char *linePtr;

    if ((linePtr = fgets(line, MAX_LINE_SIZE, f)) != NULL)
        chomp(linePtr);
    else if (ferror(f))
        perror(name);

    return linePtr;
}

char *replaceWithOnesAndZeros(char *line, size_t len)
{
    size_t i;
    for (i = 0; i < len; ++i) {
        switch (line[i]) {
            case '#':
                line[i] = '1';
                break;
            case '.':
                line[i] = '0';
                break;
        }
    }
    return line;
}

bool makeGlyph(FILE *f, char *name, uint16_t *lineNum, uint8_t pixelHeight)
{
    char *line;
    uint8_t i;
    bool ok = true;
    size_t len;

    printf("\t{\n");
    for (i = 0; i < pixelHeight; ++i)
        if ((line = readLine(f, name)) != NULL) {
            (*lineNum)++;
            if ((len = strlen(line)) == PIXEL_WIDTH) {
                replaceWithOnesAndZeros(line, len);
                printf("\t0x%x,\n", convertReverseBinaryStringToInt(line));
            }
            else {
                fprintf(stderr, "Line %d, containing '%s', is wrong length: %d\n", *lineNum, line, len);
                ok = false;
                break;
            }
        }
        else
            break;

    printf("    },\n");
    return ok;
}

bool makeFontFromStream(FILE *f, char *name)
{
    uint8_t pixelHeight, asciiCode, i, prevAsciiCode = 0;
    char *line, *arrayName;
    bool ok = true;
    uint16_t lineNum = 0;
    size_t len;

    if ((line = readLine(f, name)) != NULL) {
        lineNum++;
        pixelHeight = atoi(line);
        arrayName = makeIdentName(removeExtension(basename(name)));
        printf("uint8_t %s[128][%d] = {\n", arrayName, pixelHeight);
        printf("    /* ASCII code 0x0 */ {0},\n");
        while ((line = readLine(f, name)) != NULL) {
            lineNum++;
            if ((len = strlen(line)) != 1) {
                fprintf(stderr, "Line %d, containing '%s', is wrong length: %d\n", lineNum, line, len);
                ok = false;
                break;
            }
            asciiCode = (uint8_t) line[0];
            for (i = prevAsciiCode + 1; i < asciiCode; ++i)
                printf("    /* ASCII code 0x%x */ {0},\n", i);
            printf("    /* Char '%c', ASCII code 0x%x */\n", asciiCode, asciiCode);
            if (makeGlyph(f, name, &lineNum, pixelHeight))
                prevAsciiCode = asciiCode;
            else {
                ok = false;
                break;
            }
        }
        printf("};\n\n");
    }
    return ok;
}

bool makeFontFromFile(char *fileName)
{
    FILE *f;
    bool ok = false;

    if ((f = fopen(fileName, "r")) != NULL) {
        if (makeFontFromStream(f, fileName))
            ok = true;
        fclose(f);
    }
    else
        perror(fileName);
    
    return ok;
}

int main(int argc, char *argv[])
{
    int i;

    for (i = 1; i < argc; ++i)
        if (! makeFontFromFile(argv[i]))
            return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

