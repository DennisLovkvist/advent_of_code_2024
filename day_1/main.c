#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define EXAMPLE
#ifdef EXAMPLE
    const uint8_t digits = 1;
    const uint8_t stride = (digits * 2) + 4;
    char* path_input_file = "input(example).txt";
#else
    const uint8_t digits = 5;
    const uint8_t stride = (digits * 2) + 4;
    char* path_input_file = "input.txt";
#endif

static char* load_input(char *path)
{
    FILE *f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *source_code = malloc(fsize + sizeof(char));
    fread(source_code, fsize, 1, f);
    fclose(f);
    source_code[fsize] = '\0';
    return source_code;
}
static int32_t parse_number(char *line, int32_t s, int32_t e)
{
    int32_t length = e - s;
    char *digit = alloca(sizeof(char) * (length + 1));
    memcpy(digit, &line[s], sizeof(char) * length);
    digit[length] = '\0';
    return atoi(digit);
}
int compare(const void* a, const void* b) 
{
    return (*(int32_t*)a - *(int32_t*)b);
}

int8_t part1()
{
    char* input_raw = load_input(path_input_file);
    int32_t input_size = strlen(input_raw);

    int32_t line_count = 1; //last char is \0 instead of \n
    for (size_t i = 0; i < input_size; i++)
    {
        if(input_raw[i] == '\n')line_count++;
    }

    int32_t* l = malloc(sizeof(int32_t)*line_count);
    int32_t* r = malloc(sizeof(int32_t)*line_count);
    
    int32_t n = 0;

    for (int32_t i = 0; i < line_count; i++)
    {
        char* line = &input_raw[stride*i];
        int32_t number_left = parse_number(line,0,digits);
        int32_t number_right = parse_number(line,digits+3,stride);

        l[i] = number_left;
        r[i] = number_right;
    }

    free(input_raw);

    qsort(l, line_count, sizeof(int32_t), compare);
    qsort(r, line_count, sizeof(int32_t), compare);

    int32_t delta = 0;

    for (size_t i = 0; i < line_count; i++)
    {
        delta += abs(l[i]-r[i]);
    }
    
    free(l);
    free(r);


    printf("part 1: %i\n",delta);
    return 1;
}

int8_t part2()
{
    char* input_raw = load_input(path_input_file);
    int32_t input_size = strlen(input_raw);

    int32_t line_count = 1; //last char is \0 instead of \n
    for (size_t i = 0; i < input_size; i++)
    {
        if(input_raw[i] == '\n')line_count++;
    }

    int32_t* l = malloc(sizeof(int32_t)*line_count);
    int32_t* r = malloc(sizeof(int32_t)*line_count);
    
    uint8_t digits = 5;
    uint8_t stride = (digits * 2) + 4;
    int32_t n = 0;

    for (int32_t i = 0; i < line_count; i++)
    {
        char* line = &input_raw[stride*i];
        int32_t number_left = parse_number(line,0,digits);
        int32_t number_right = parse_number(line,digits+3,stride);

        l[i] = number_left;
        r[i] = number_right;
    }
    
    free(input_raw);

    int32_t similarity = 0;

    for (size_t i = 0; i < line_count; i++)
    {
        int n = 0;
        for (size_t j = 0; j < line_count; j++)
        {
            if(i != j)
            {
                if(l[i] == r[j])
                {
                    n++;
                }
            }
        }
        similarity += (l[i] * n);
    }
    
    
    free(l);
    free(r);


    printf("part 2: %i\n",similarity);
    return 1;
}
int8_t main()
{
    part1();//Example: 11   Input: 1341714
    part2();//Example: 31   Input: 27384707
}