#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define EXAMPLE
#ifdef EXAMPLE
    char* path_input_file1 = "input(example1).txt";
    char* path_input_file2 = "input(example2).txt";
#else
    char* path_input_file1 = "input.txt";
    char* path_input_file2 = "input.txt";
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
int8_t part1()
{
    char* input_raw = load_input(path_input_file1);
    int32_t input_size = strlen(input_raw);

    uint16_t caret = 0;

    uint32_t total = 0;

    while (input_raw[caret] != '\0')
    {
        if(strncmp(&input_raw[caret],"mul(", sizeof(char)*4) == 0)
        {
            int index = caret + 4;
            uint8_t valid = 0;
            while(1)
            {
                char c = input_raw[index];
                uint8_t is_number = (c >= 48 && c <= 57);

                if(c != ',' && !is_number)
                {
                    valid = c == ')';
                    break;
                }
                index ++;
            }

            int length = index - caret;
            
            if(valid)
            {
                char* temp = alloca(length+3);
                snprintf(temp,length+1,"%s",&input_raw[caret]);

                int32_t operand_left  = 0;
                int32_t operand_right = 0;

                char *token = strtok(&temp[4], ",");

                operand_left = atoi(token);
                token = strtok(NULL, ",");
                operand_right = atoi(token);

                total += (operand_left * operand_right);
            }

            caret += length;
        }

        caret++;
    }
    
    printf("part 1: %i\n", total);
    return 1;
}

int8_t part2()
{
    char* input_raw = load_input(path_input_file2);
    int32_t input_size = strlen(input_raw);

    uint16_t caret = 0;

    uint32_t total = 0;

    uint8_t ignore = 0;

    while (input_raw[caret] != '\0')
    {
        if(strncmp(&input_raw[caret],"don't()", sizeof(char)*7) == 0)
        {
            ignore = 1;
        }
        if(strncmp(&input_raw[caret],"do()", sizeof(char)*4) == 0)
        {
            ignore = 0;
        }

        if(strncmp(&input_raw[caret],"mul(", sizeof(char)*4) == 0)
        {
            int index = caret + 4;
            uint8_t valid = 0;
            while(1)
            {
                char c = input_raw[index];
                uint8_t is_number = (c >= 48 && c <= 57);

                if(c != ',' && !is_number)
                {
                    valid = c == ')';
                    break;
                }
                index ++;
            }

            int length = index - caret;
            
            if(valid && !ignore)
            {
                char* temp = alloca(length+3);
                snprintf(temp,length+1,"%s",&input_raw[caret]);

                int32_t operand_left  = 0;
                int32_t operand_right = 0;

                char *token = strtok(&temp[4], ",");

                operand_left = atoi(token);
                token = strtok(NULL, ",");
                operand_right = atoi(token);

                total += (operand_left * operand_right);
                
            }

            caret += length;
        }

        caret++;
    }
    
    
    printf("part 2: %i\n", total);
    return 1;
}

int8_t main()
{
    part1();//Example: 161     Input: 153469856
    part2();//Example: 48      Input: 77055967
}