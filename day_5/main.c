#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define EXAMPLE
#ifdef EXAMPLE
    char* path_input_file = "input(example).txt";
#else
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

typedef struct Rule Rule;
struct Rule
{
    uint8_t left;
    uint8_t right;
};
typedef struct RuleSet RuleSet;
struct RuleSet
{
    Rule* rules;
    uint32_t count;
};

typedef struct Sequence Sequence;
struct Sequence
{
    uint8_t numbers[64];
    uint8_t count;
};
typedef struct SequenceSet SequenceSet;
struct SequenceSet
{
    Sequence* secuences;
    uint32_t count;
};

void parse(char* input, int length,RuleSet* rule_set, SequenceSet* sequence_set)
{
    
    uint32_t index = 0;
    char prev = '\n';
    while(1)
    {
        char curr = input[index];
        if(curr == prev)
        {
            if(curr == '\n')
            {
                break;
            }
        }
        prev = curr;
        index++;
    }
    
    if(index > 1)index--;

    char* input_rules = malloc(index+1);
    memcpy(input_rules,input,index);

    uint32_t count_pairs = 1;
    for (size_t i = 0; i < index; i++)
    {
        if(input_rules[i] == '\n')
        {
            count_pairs ++;
            input_rules[i] = '|';
        }
    }


    rule_set->count = count_pairs;
    rule_set->rules = malloc(sizeof(Rule)*rule_set->count);

    uint32_t rule_index = 0;

    char *token = strtok(input_rules, "|");

    int right = 0;
    while (token != NULL) 
    {
        if(right)
        {
            right = 0;
            rule_set->rules[rule_index].right = atoi(token);
            rule_index++;
        }
        else
        {
            rule_set->rules[rule_index].left = atoi(token);

            right = 1;
        }
        token = strtok(NULL, "|");
    }
    
    free(input_rules);

    index +=2;

    uint32_t len = length-index;
    char* input_sequences = malloc(len);
    memcpy(input_sequences,&input[index],len);


    uint32_t line_count = 1;
    index = 0;
    while (input_sequences[index] != '\0')
    {
        if(input_sequences[index] == '\n')
        {
            line_count++;
        }
        index++;
    }
    
    sequence_set->count = line_count;
    sequence_set->secuences = malloc(sizeof(Sequence)*sequence_set->count);

    token = strtok(input_sequences, "\n");

    uint32_t sequence_index  = 0;

    while (token != NULL) 
    {
        uint32_t token_length = strlen(token);

        char digits[6] = {'\0'};
        uint8_t digit_place = 0;

        uint8_t number_index = 0;

        for (size_t i = 0; i < token_length+1; i++)
        {
            if(token[i] == ',' || token[i] == '\0' )
            {
                sequence_set->secuences[sequence_index].numbers[number_index] = atoi(digits);
                number_index ++;
                digit_place = 0;
                memset(digits,'\0',6);
            }
            else
            {
                digits[digit_place] = token[i];
                digit_place++;
            }
        }
        sequence_set->secuences[sequence_index].count = number_index;

        sequence_index ++;

        token = strtok(NULL, "\n");
    }

    free(input_sequences);
}
int validate_sequence(Sequence* s,RuleSet* rs)
{
    for (uint32_t i = 0; i < rs->count; i++)
    {
        uint8_t left  = rs->rules[i].left;
        uint8_t right  = rs->rules[i].right;

        
        for (uint8_t j = 0; j < s->count; j++)
        {

            if(s->numbers[j] == left)
            {
                for (uint8_t k = 0; k < s->count; k++)
                {

                    if(s->numbers[k]== right)
                    {
                        if(j > k)
                        {
                            return 0;
                        }
                    }
                }
            }

            if(s->numbers[j] == right)
            {
                for (uint8_t k = 0; k < s->count; k++)
                {
                    if(s->numbers[k]== left)
                    {
                        if(j < k)
                        {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;

    
}
void order_sequence(Sequence* s,RuleSet* rs)
{
    for (uint32_t i = 0; i < rs->count; i++)
    {
        uint8_t left  = rs->rules[i].left;
        uint8_t right  = rs->rules[i].right;

        
        for (uint8_t j = 0; j < s->count; j++)
        {
            if(s->numbers[j] == left)
            {
                for (uint8_t k = 0; k < s->count; k++)
                {
                    if(s->numbers[k]== right)
                    {
                        if(j > k)
                        {
                            uint8_t temp = s->numbers[j];
                            s->numbers[j] = s->numbers[k];
                            s->numbers[k] = temp;
                        }
                    }
                }
            }

            if(s->numbers[j] == right)
            {
                for (uint8_t k = 0; k < s->count; k++)
                {
                    if(s->numbers[k]== left)
                    {
                        if(j < k)
                        {
                            uint8_t temp = s->numbers[j];
                            s->numbers[j] = s->numbers[k];
                            s->numbers[k] = temp;
                        }
                    }
                }
            }
        }
    }

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

    RuleSet rule_set;
    SequenceSet sequence_set;
    parse(input_raw,input_size,&rule_set,&sequence_set);

    free(input_raw);

    uint32_t total = 0;

    for (size_t i = 0; i < sequence_set.count; i++)
    {
        if(validate_sequence(&sequence_set.secuences[i],&rule_set))
        {
            Sequence* s = &sequence_set.secuences[i];

            uint8_t middle = s->count / 2;
            total += s->numbers[middle];
            
        }
    }

    printf("part 1 :%i\n",total);
    return 0;
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

    RuleSet rule_set;
    SequenceSet sequence_set;
    parse(input_raw,input_size,&rule_set,&sequence_set);

    free(input_raw);

    uint32_t total = 0;

    for (size_t i = 0; i < sequence_set.count; i++)
    {
        Sequence* s = &sequence_set.secuences[i];

        int valid = 1;

        while(!validate_sequence(s,&rule_set))
        {   
            valid = 0;
            order_sequence(s,&rule_set);
            
        }
        if(!valid)
        {

            int middle = s->count / 2;
            total += s->numbers[middle];
        }
    }

    printf("part 2: %i\n",total);
    return 0;
}
int main()
{
    part1();//Example: 143      Input: 4959
    part2();//Example: 123      Input: 4655
    
}