#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

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
int xytoi(int x, int y, int s)
{
    return y*s + x;
}
int8_t part1()
{
    char* input_raw = load_input(path_input_file);
    int32_t input_size = strlen(input_raw);

    int stride = 0;
    int lines = 0;
    int index = 0;
    while(input_raw[index] != '\0')
    {
        if(input_raw[index] == '\n')
        {
            if(stride == 0)
            {
                stride = index;
            }
            lines++;
        }
        index ++;
    }
    lines++;
    stride --;

    size_t length = (size_t)stride*(size_t)lines;
    char* data = (char*)malloc(length);
    index = 0;
    for (int i = 0; i < input_size; i++)
    {
        if(input_raw[i] != '\n' && input_raw[i] != 13 && input_raw[i] != '\0')
        {
            data[index++] = input_raw[i];
        }
    }
    
    int count = 0;
    
    for (int y = 0; y < lines; y++)
    {
        for (int x = 0; x < stride; x++)
        {
            int index = xytoi(x,y,stride);
            if(data[index] == 'X')
            {
                if(x + 3 < stride)
                {
                    int I[3] = {xytoi(x+1,y,stride),xytoi(x+2,y,stride),xytoi(x+3,y,stride)};
                    if(data[I[0]] == 'M' && data[I[1]] == 'A' && data[I[2]] == 'S')
                    {
                        count ++;
                    }
                }
                if(x - 3 >= 0)
                {
                    int I[3] = {xytoi(x-1,y,stride),xytoi(x-2,y,stride),xytoi(x-3,y,stride)};
                    if(data[I[0]] == 'M' && data[I[1]] == 'A' && data[I[2]] == 'S')
                    {
                        count ++;
                    }
                }
                //Vertical
                if(y + 3 < lines)
                {
                    int I[3] = {xytoi(x,y+1,stride),xytoi(x,y+2,stride),xytoi(x,y+3,stride)};
                    if(data[I[0]] == 'M' && data[I[1]] == 'A' && data[I[2]] == 'S')
                    {
                        count ++;
                    }
                }
                if(y - 3 >= 0)
                {
                    int I[3] = {xytoi(x,y-1,stride),xytoi(x,y-2,stride),xytoi(x,y-3,stride)};
                    if(data[I[0]] == 'M' && data[I[1]] == 'A' && data[I[2]] == 'S')
                    {
                        count ++;
                    }
                }

                //Diagonal 1,1
                if(x + 3 < stride && y + 3 < lines)
                {
                    int I[3] = {xytoi(x+1,y+1,stride),xytoi(x+2,y+2,stride),xytoi(x+3,y+3,stride)};
                    if(data[I[0]] == 'M' && data[I[1]] == 'A' && data[I[2]] == 'S')
                    {
                        count ++;
                    }
                }
                //Diagonal 1,-1
                if(x + 3 < stride && y - 3 >= 0)
                {
                    int I[3] = {xytoi(x+1,y-1,stride),xytoi(x+2,y-2,stride),xytoi(x+3,y-3,stride)};
                    if(data[I[0]] == 'M' && data[I[1]] == 'A' && data[I[2]] == 'S')
                    {
                        count ++;
                    }
                }
                //Diagonal -1,1
                if(x - 3 >= 0 && y + 3 < lines)
                {
                    int I[3] = {xytoi(x-1,y+1,stride),xytoi(x-2,y+2,stride),xytoi(x-3,y+3,stride)};
                    if(data[I[0]] == 'M' && data[I[1]] == 'A' && data[I[2]] == 'S')
                    {
                        count ++;
                    }
                }
                //Diagonal -1,-1
                if(x - 3 >= 0 && y - 3 >= 0)
                {
                    int I[3] = {xytoi(x-1,y-1,stride),xytoi(x-2,y-2,stride),xytoi(x-3,y-3,stride)};
                    if(data[I[0]] == 'M' && data[I[1]] == 'A' && data[I[2]] == 'S')
                    {
                        count ++;
                    }
                }
            }
            
        }
    }
   
    printf("part 1: %i\n",count);

    return 1;
}
int8_t part2()
{
    char* input_raw = load_input(path_input_file);
    int32_t input_size = strlen(input_raw);

    int stride = 0;
    int lines = 0;
    int index = 0;
    while(input_raw[index] != '\0')
    {
        if(input_raw[index] == '\n')
        {
            if(stride == 0)
            {
                stride = index;
            }
            lines++;
        }
        index ++;
    }
    lines++;
    stride --;

    size_t length = (size_t)stride*(size_t)lines;
    char* data = (char*)malloc(sizeof(char)*length);
    index = 0;
    for (int i = 0; i < input_size; i++)
    {
        if(input_raw[i] != '\n' && input_raw[i] != 13 && input_raw[i] != '\0')
        {
            data[index++] = input_raw[i];
        }
    }
    
    int count = 0;


    
    for (int y = 0; y < lines; y++)
    {
        for (int x = 0; x < stride; x++)
        {
            int index = xytoi(x,y,stride);
            if(data[index] == 'A')
            {

                //Diagonal 1,1
                if((x - 1 >= 0 && y - 1 >= 0) && (x + 1 < stride && y + 1 < lines))
                {
                    int tl = xytoi(x-1,y-1,stride);
                    int br = xytoi(x+1,y+1,stride);

                    int tr = xytoi(x+1,y-1,stride);
                    int bl = xytoi(x-1,y+1,stride);

                    int lol = (data[tl] == 'M' && data[br] == 'S') ||(data[tl] == 'S' && data[br] == 'M');
                    int lol2 = (data[tr] == 'M' && data[bl] == 'S') ||(data[tr] == 'S' && data[bl] == 'M');

                    if(lol && lol2)
                    {
                        count++;
                    }  
                    
                }
            }
            
        }
    }
    printf("part 2:%i\n", count);

}

int8_t main()
{
    part1();//Example: 18      Input:2468
    part2();//Example: 9       Input:1864
}