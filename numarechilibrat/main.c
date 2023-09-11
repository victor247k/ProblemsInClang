#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 255

long get_equal_number(long number);
int is_equal(long number);
int get_length(long number);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s input.txt output.txt\n", argv[0]);
        return 1;
    }

    // open the infile
    char *input = argv[1];
    FILE *input_pointer = fopen(input, "r");
    // check if we could open the file
    if (input_pointer == NULL)
    {
        printf("Couldn't open %s\n", input);
        return 1;
    }
    
    // create the buffer
    char *buffer = malloc(sizeof(char) * MAX);
    // check if the buffer has enough memory
    if (buffer == NULL)
    {
        printf("Not enough memory\n");
        fclose(input_pointer);
        return 1;
    }

    // read into a buffer
    fgets(buffer, MAX, input_pointer);
    // find out the size of the number
    int size = 0;
    while (buffer[size] != ' ' && buffer[size] != EOF)
        size++;

    buffer[size] = '\0';
    // printf("Debug: %s\nSize: %d\n", buffer, size); // debug 1

    // convert buffer to long type integer
    // long long_number = strtol(buffer, NULL, 10);
    char *endptr;
    long long_number = strtol(buffer, &endptr, 10);
    if (*endptr != '\0' && !isspace(*endptr)) {
        printf("Invalid number in the input file.\n");
        fclose(input_pointer);
        free(buffer);
        return 1;
    }

    // get an equal number | one higher than current
    long number = get_equal_number(long_number + 1);

    // open a file to write in
    char *output = argv[2];
    FILE *output_pointer = fopen(output, "w");

    // check if we could open the file
    if (output_pointer == NULL)
    {
        printf("Couldn't open %s\n", output);
        fclose(input_pointer);
        return 1;
    }

    // write that number into a file
    size = get_length(number) + 1;
    char *result = malloc(sizeof(char) * size + 1);
    if (result == NULL)
    {
        printf("Error allocating memory to result\n");
        fclose(input_pointer);
    }
    sprintf(result, "%ld", number);
    result[size - 1] = '\0';

    // printf("Result: %s\n", result);

    fwrite(result, size - 1, 1, output_pointer);

    // close and free everything
    fclose(input_pointer);
    fclose(output_pointer);
    free(result);

    return 0;
}

long get_equal_number(long number)
{
    // check if the number is equal
    if (is_equal(number))
        return number;
    else
        return get_equal_number(number + 1);    
}

int is_equal(long number)
{
    int even_sum = 0, odd_sum = 0;
    int length = get_length(number);

    // printf("is_equal\n    number: %ld\n    lenght: %d\n", number, length);

    int is_even_position = 1;  // Start with the first digit (even position).
    while (number > 0)
    {
        int digit = number % 10;  // Get the last digit.
        if (is_even_position)
        {
            even_sum += digit;
        }
        else
        {
            odd_sum += digit;
        }
        is_even_position = !is_even_position;  // Switch between even and odd positions.
        number /= 10;  // Remove the last digit.
    }

    return even_sum == odd_sum ? 1 : 0;
}

int get_length(long number)
{
    // set length to 1
    int length = 1;
    // check if the new number is lower than 2 digits(10)
    while (number >= 10)
    {
        // increase the length and decrease the digits on the number
        length++;
        number /= 10;
    }
    return length;
}