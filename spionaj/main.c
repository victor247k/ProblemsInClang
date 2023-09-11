#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

char *uncrypt(char *code, int length);
char binnary_to_letter(char *binnary, int start, int end);

int main(int argc, char *argv[])
{
    // check for correct usage
    if (argc != 2)
    {
        printf("Usage: %s input.txt\n", argv[0]);
        return 1;
    }

    // open file with the crypted message
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("Couldn't open %s\n", argv[1]);
        return 1;
    }


    char *buffer = malloc(sizeof(char) * MAX + sizeof(char));
    if (buffer == NULL)
    {
        printf("Not enough memory\n");
        fclose(fp);
        return 1;
    }

    fgets(buffer, MAX, fp);

    int size = 0;
    while (buffer[size] != ' ' && buffer[size] != EOF)
        size++;
    
    buffer[size] = '\0';

    // printf("Debug: %s\n", buffer);

    // uncrypt the message
    char *message = uncrypt(buffer, size);

    // write the message in a new file
    FILE *fo = fopen("output.txt", "w");
    if (fo == NULL)
    {
        printf("Couldn't open output.txt\n");
        return 1;
    }
    // printf("Message: %s\n", message);
    fprintf(fo, "%s\n", message);

    // free anyhting if necesary
    free(buffer);
    fclose(fp);
    fclose(fo);
    free(message);

    return 0;
}

char *uncrypt(char *code, int length)
{
    char *message = malloc(sizeof(char) * (MAX + 1));
    int size_of_message = 0;
    char sign = code[0] == '0' ? '1' : '0';

    // from code to binnary
    int size = 0;
    // how much space to alocare
    for (int i = 1; i < length; i++)
        size += code[i] - 48;

    // printf("Code: %s\n", code);
    // printf("size: %d\n", size);
    // printf("Code[1]: %d\n", (code[1]));

    char *binnary = malloc(size * sizeof(char));
    // binnary[size] = '\0';

    int pos = 0;
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < code[i] - 48; j++)
        {
            // printf("pos: %d\nj: %d\nsign: %c\n", pos, j, sign);
            binnary[pos + j] = sign;       
        }

        sign = sign == '0' ? '1' : '0';
        pos += code[i] - 48;
    }

    binnary[size] = '\0';
    // printf("binnary: %s\n", binnary);

    for (int i = 0; i < pos; i+=8)
    {
        message[size_of_message] = binnary_to_letter(binnary, i, i + 8); 
        size_of_message++;
    }

    free(binnary);

    message[size_of_message] = '\0';
    return message;
}

char binnary_to_letter(char *binnary, int start, int end)
{
    int sum = 0;
    int multiplier = 128;

    for (int i = start; i <= end; i++)
    {
        // printf("binary[%d]: %d\n", i, binnary[i] - 48);
        sum += (binnary[i] - 48) * multiplier;
        multiplier /= 2;
    }
    // printf("Letter: %c\n", sum);
    return sum;
}