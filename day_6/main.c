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
typedef struct Vec2i Vec2i;
struct Vec2i
{
    int x;
    int y;
};
typedef struct State State;
struct State
{
    Vec2i pos;
    char dir;
};
Vec2i direction_to_vector(char guard)
{
    switch(guard)
    {
        case '<':
            return (Vec2i){-1,0};
        case '>':
            return (Vec2i){1,0};
        case '^':
            return (Vec2i){0,-1};
        case 'v':
            return (Vec2i){0,1};
    }
    return (Vec2i){0,0};
}
char direction_char_turn90(char direction)
{
    switch(direction)
    {
        case '<':
            return '^';
        case '>':
            return 'v';
        case '^':
            return '>';
        case 'v':
            return '<';
    }
    return 'H';
}
int walk(char* map, int32_t stride, int32_t lines, int32_t startx, int32_t starty,int32_t*visits, State* states, int32_t state_size)
{
    int curr_x = startx;
    int curr_y = starty;

    char direction = map[curr_y * stride + curr_x];

    Vec2i vector = direction_to_vector(direction);
    
    int kill_loop = 0;
    
    int states_count = 0;

    while(1)
    {
        kill_loop++;
        if(kill_loop > stride*lines)return 1;
       
        int curr_index = curr_y * stride + curr_x;

        if(map[curr_index] == '.')
        {
            map[curr_index] = 'X';
            *visits = *visits +1;
        }

        int next_x = curr_x + vector.x;
        int next_y = curr_y + vector.y;
        int next_index = next_y * stride + next_x;

         if(next_x < 0 || next_y < 0 || next_x >= stride || next_y >= lines)
        {
            return 0;
        }

        if(map[next_index] == 'O')
        {
            State state;
            state.dir = direction;
            state.pos.x = curr_x;
            state.pos.y = curr_y;

            for (size_t i = 0; i < states_count; i++)
            {
                if(states[i].dir == state.dir && states[i].pos.x == state.pos.x && states[i].pos.y == state.pos.y)
                {
                    return 1;
                }
            }
            states[states_count].pos = state.pos;
            states[states_count].dir = state.dir;
            states_count ++;
        }

        int rotations = 0;
        while((map[next_index] == '#' || map[next_index] == 'O') && rotations < 4)
        {
            direction = direction_char_turn90(direction);
            vector    = direction_to_vector(direction);
            next_x = curr_x + vector.x;
            next_y = curr_y + vector.y;
            next_index = next_y * stride + next_x;
            rotations++;
        }
        if(rotations >= 4)return 1;

        curr_x = next_x;
        curr_y = next_y;
    }
    return 0;
}
void part1()
{
    char* input_raw = load_input(path_input_file);
    int32_t input_size = strlen(input_raw);

    int32_t line_count = 1; //last char is \0 instead of \n
    for (size_t i = 0; i < input_size; i++)
    {
        if(input_raw[i] == '\n')line_count++;
    }
    
    int32_t lines  = 1;
    int32_t stride = 0;
    int32_t index  = 0;

    while(input_raw[index]!='\0')
    {
        if(input_raw[index] == '\n')
        {   
            if(stride == 0)
            {
                stride = index;
            }
            lines++;
        }
        index++;
    }

    int32_t map_size = stride*lines;
    char* map = malloc(map_size);

    index = 0;
    int32_t map_index = 0;
    int32_t startx    = 0;
    int32_t starty    = 0;

    while(input_raw[index]!='\0')
    {
        if(input_raw[index] != '\n')
        {   
            map[map_index] = input_raw[index];
            if(map[map_index] == '^'||map[map_index] == 'v'|| map[map_index] == '>'||map[map_index] == '<')
            {
                starty = map_index / stride;
                startx = map_index % stride;
            }
            map_index++;
        }
        
        index++;
    }

    free(input_raw);
    int32_t visits = 1;
    walk(map,stride,lines,startx,starty,&visits, NULL,0);

    printf("part 1: %i\n", visits);
    

}
void part2()
{
    char* input_raw = load_input(path_input_file);
    int32_t input_size = strlen(input_raw);

    int32_t line_count = 1; //last char is \0 instead of \n
    for (size_t i = 0; i < input_size; i++)
    {
        if(input_raw[i] == '\n')line_count++;
    }
    
    int32_t lines  = 1;
    int32_t stride = 0;
    int32_t index  = 0;

    while(input_raw[index]!='\0')
    {
        if(input_raw[index] == '\n')
        {   
            if(stride == 0)
            {
                stride = index;
            }
            lines++;
        }
        index++;
    }

    int32_t map_size = stride*lines;
    char* map = malloc(map_size);

    index = 0;
    int32_t map_index = 0;
    int32_t startx = 0;
    int32_t starty = 0;
    while(input_raw[index]!='\0')
    {
        if(input_raw[index] != '\n')
        {   
            map[map_index] = input_raw[index];
            if(map[map_index] == '^'||map[map_index] == 'v'|| map[map_index] == '>'||map[map_index] == '<')
            {
                starty = map_index / stride;
                startx = map_index % stride;
            }
            map_index++;
        }
        
        index++;
    }

    free(input_raw);

    char* map_temp = malloc(map_size);

    State* states = malloc(sizeof(State)*map_size);
    
    int32_t loops = 0;
    for (int i = 0; i < stride*lines; i++)
    {
        memcpy(map_temp,map,sizeof(char)*map_size);

        if(map_temp[i] == '.')
        {
            map_temp[i] = 'O';
            int32_t visits = 1;

            loops += walk(map_temp,stride,lines,startx,starty,&visits,states,map_size);
        }

    }
    printf("part 2: %i\n",loops);
}
int main()
{
    part1();//Example: 41       Input: 4826
    part2();//Example: 6        Input: 1721
}