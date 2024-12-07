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
typedef struct Equation Equation;
struct Equation
{
    uint64_t operands[32];
    uint64_t count;
    uint64_t answer;
};
Equation parse_equation(char* line)
{
    Equation equation;
    uint64_t index = 0;
    
    char c = line[index];

    while(c != ':')
    {
        c = line[index];
        index++;
    }
    line[index] = '\0';
    equation.answer = strtoull(line, NULL, 10);

    index++;

    char *token;

    token = strtok(&line[index], " ");
    uint64_t operand_index = 0;
    while (token != NULL) 
    {
        equation.operands[operand_index] = strtoull(token, NULL, 10);
        
        operand_index++;
        token = strtok(NULL, " ");
    }
    equation.count = operand_index;

    return equation;

}
Equation* parse(char* input, uint64_t input_size,uint64_t *equation_count)
{
    uint64_t index = 0;
    uint64_t start = 0;
    uint64_t end = 0;

    uint64_t lines = 1;

    while(input[index] != '\0')
    {
        if(input[index] == '\n')lines++;

        index ++;
    }

    Equation* equations = malloc(sizeof(Equation)*lines);

    index = 0;
    *equation_count = 0;
    while (1)
    {
        char c = input[index];
        if(c == '\n' || c == '\0')
        {
            end = index+1;
            char line[64];
            snprintf(line,(end-start),"%s",&input[start]);
            Equation e = parse_equation(line);
            equations[*equation_count] = e;
            *equation_count = *equation_count + 1;
            start = end;
        }
        if(c == '\0')
        {
            break;
        }
        index++;
    }

    return equations;
    
}
uint64_t solve(uint64_t* operands, uint64_t count, uint64_t index, uint64_t acc, uint64_t target)
{
    if(index == count)
    {
       return (acc == target);
    }
    else
    {
        if(solve(operands,count, index+1,acc+operands[index],target))return 1;
        if(solve(operands,count, index+1,acc*operands[index],target))return 1;
        return 0;
    }
}
uint64_t solve2(uint64_t* operands, uint64_t count, uint64_t index, uint64_t acc, uint64_t target)
{
    if(index == count)
    {
       return (acc == target);
    }
    else
    {
        if(solve2(operands,count, index+1,acc+operands[index],target))return 1;
        if(solve2(operands,count, index+1,acc*operands[index],target))return 1;

        char digits[64];
        snprintf(digits, 64, "%llu%llu",acc,operands[index]);
        int64_t number = strtoull(digits, NULL, 10);

        if(solve2(operands,count, index+1,number,target))return 1;

        return 0;
    }
    
}
void part1()
{
    char* input_raw = load_input(path_input_file);
    uint64_t input_size = strlen(input_raw);

    uint64_t line_count = 1; //last char is \0 instead of \n
    for (size_t i = 0; i < input_size; i++)
    {
        if(input_raw[i] == '\n')line_count++;
    }
    
    uint64_t equation_count = 0;
    Equation* equations = parse(input_raw,input_size,&equation_count);

    uint64_t total = 0;
    
    for (size_t i = 0; i < equation_count; i++)
    {
        Equation *equation = &equations[i];
        if(solve(equation->operands,equation->count,1,equation->operands[0],equation->answer))
        {
            total+=equation->answer;
        }
    }
    printf("part 1: %llu\n",total);
}
void part2()
{
    char* input_raw = load_input(path_input_file);
    uint64_t input_size = strlen(input_raw);

    uint64_t line_count = 1; //last char is \0 instead of \n
    for (size_t i = 0; i < input_size; i++)
    {
        if(input_raw[i] == '\n')line_count++;
    }
    
    uint64_t equation_count = 0;
    Equation* equations = parse(input_raw,input_size,&equation_count);

    uint64_t total = 0;
    
    for (size_t i = 0; i < equation_count; i++)
    {
        Equation *e = &equations[i];
        if(solve2(e->operands,e->count,1,e->operands[0],e->answer))
        {
            total+=e->answer;
        }

    }
    printf("part 2: %llu\n",total);
}

int main()
{
    part1();//Example: 3749     Input: 1545311493300
    part2();//Example: 11387    Input: 169122112716571
}