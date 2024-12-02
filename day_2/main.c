#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
int8_t part1()
{
    char* input_raw = load_input("input.txt");
    int32_t input_size = strlen(input_raw);

    int32_t line_count = 1; //last char is \0 instead of \n
    for (size_t i = 0; i < input_size; i++)
    {
        if(input_raw[i] == '\n')line_count++;
    }

    uint16_t index = 0;

    uint16_t safe_count = 0;

    while(index < input_size)
    {
        uint8_t step = 0;
        while(input_raw[index+step] != '\n' && input_raw[index+step] != '\0')
        {
            step++;
        }

        input_raw[index+step] = '\0';

        char *token = strtok(&input_raw[index], " ");

        uint8_t safe = 0;
        int8_t current = -1;
        int8_t previous = -1;

        int8_t delta_prev = 0;

        uint8_t first = 1;

        while (token != NULL) 
        {
            current = atoi(token);
            safe = 0;
            int8_t delta = 0;

            if(previous == -1)
            {
                safe = 1;
            }
            else
            {
                delta = current - previous;
                if(first)
                {
                    delta_prev = delta;
                    first = 0;
                }

                if((delta > 0 && delta_prev > 0) ||  (delta < 0 && delta_prev < 0)) 
                {
                    if((abs(delta) < 4) && (abs(delta) > 0))
                    {
                        safe = 3;
                    }
                } 
                
            }

            if(!safe)
            {
                delta_prev = 0;
                previous = -1;
                break;
            }

            delta_prev = delta; 
            previous = current;

            token = strtok(NULL, " ");
        }

        safe_count += safe ? 1:0;

        index += step+1;
    }

    printf("part 1: %i\n",safe_count);

    return 0;
}
uint8_t compare(int8_t a, int8_t b, int8_t* direction)
{
    int8_t delta = abs(a-b);
    int8_t dir = a < b ? 1:-1;
    return (dir == *direction && delta > 0 && delta < 5);
}
int8_t calc_majority_direction(int8_t sequence[16], uint8_t count)
{
    uint8_t positive = 0;
    uint8_t negative = 0;

    for (size_t i = 0; i < count-1; i++)
    {
        positive += (sequence[i] < sequence[i+1]);
        negative += (sequence[i] > sequence[i+1]);
    }
    
    if(positive > negative)return 1;
    if(positive < negative)return -1;
    if(positive == negative)return 0;
}
uint8_t saftey_check(int8_t sequence[16], uint8_t count,int8_t majority_direction, int8_t exclude_index)
{
    for (size_t i = 0; i < count; i++)
    {
        if(i+1 >= count)break;

        if(i == exclude_index)continue;

        int8_t next = i+1;

        if(next == exclude_index)
        {
            if(next+1 <count)
            {
                next = next+1;
            }
            else
            {
                goto end_reached;
            }
        }

        int8_t delta = abs(sequence[i]-sequence[next]);

        int8_t direction = 0;

        if((sequence[i] == sequence[next]))direction =  0;
        if((sequence[i] <  sequence[next]))direction =  1;
        if((sequence[i] >  sequence[next]))direction = -1;

        uint8_t criterion = (majority_direction == direction && ((delta < 4 && delta > 0)));

        if(!criterion){return 0;}
        
    }
    end_reached:
    return 1;
}

uint8_t saftey_check_error_correction(int8_t sequence[16], uint8_t count)
{
    int8_t majority_direction = calc_majority_direction(sequence,count);

    if(majority_direction != 0)
    {
        if(saftey_check(sequence,count,majority_direction,-1))
        {
            return 1;
        }
        else
        {
            uint8_t safe_exclusions = 0;

            for (size_t i = 0; i < count; i++)
            {
                safe_exclusions += saftey_check(sequence,count,majority_direction,i);
            }
            return (safe_exclusions >= 1);
        }
    }
    return 0;
}
int8_t part2()
{
    char* input_raw = load_input("input.txt");
    int32_t input_size = strlen(input_raw);

    int32_t line_count = 1; //last char is \0 instead of \n
    for (size_t i = 0; i < input_size; i++)
    {
        if(input_raw[i] == '\n')line_count++;
    }

    uint16_t index = 0;

    int32_t safe_count = 0;
    while(index < input_size)
    {
        uint8_t step = 0;
        while(input_raw[index+step] != '\n' && input_raw[index+step] != '\0')
        {
            step++;
        }
        input_raw[index+step] = '\0';

        char *token = strtok(&input_raw[index], " ");

        uint8_t tokens = 0;
        int8_t sequence[16];

        while (token != NULL) 
        {
            sequence[tokens++] = atoi(token);
            token = strtok(NULL, " ");
        }
        index += step+1;

        if(saftey_check_error_correction(sequence,tokens))
        {
            safe_count ++;
        }

    }
    
    printf("part 2: %i\n",safe_count);

    return 0;
}
int8_t main()
{
    part1();
    part2();
}

//689