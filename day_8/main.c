#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Vec2i Vec2i;
struct Vec2i
{
    int32_t x,y;
};
typedef struct Antenna Antenna;
struct Antenna
{
    char freq;
    Vec2i pos;
};

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
Antenna* parse(uint32_t* count,uint32_t* xmax,uint32_t* ymax)
{
    char* input_raw = load_input("input(example).txt");
    uint32_t input_size = strlen(input_raw);

    uint32_t x = 0;
    uint32_t y = 0;


    int32_t antenna_count = 0;

    *xmax = 0;
    *ymax = 0;

    for (size_t i = 0; i < input_size; i++)
    {
        char c = input_raw[i];
        antenna_count += (c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122);

        *xmax = (*xmax == 0 && c == '\n') ? i : *xmax;
    }

    Antenna* antennas = malloc(sizeof(Antenna)*antenna_count);
    uint32_t antenna_index = 0;
    for (size_t i = 0; i < input_size; i++)
    {
        char c = input_raw[i];
        if((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
        {
            Antenna* antenna = &antennas[antenna_index];
            antenna->freq = c;
            antenna->pos.x = x;
            antenna->pos.y = y;
            antenna_count++;

            printf("%i,%i,%c\n",x,y,c);
        }
        
        x ++;
        if(input_raw[i] == '\n')
        {
            x = 0;
            y++;
        }
    }
    *ymax = y;
    *count = antenna_count;
    return antennas;
}
void generate_antinodes(Antenna* antennas,uint32_t antenna_count,uint32_t xmax,uint32_t ymax,Vec2i* antinodes,uint32_t* antinode_count)
{
    for (size_t i = 0; i < antenna_count; i++)
    {
        Antenna* a = &antennas[i];

        for (size_t j = 0; j < antenna_count; j++)
        {
            Antenna* n = &antennas[j];
            if(n->freq == a->freq)
            {
                
            }
        }
    }
}
void part1()
{
    uint32_t xmax = 0;
    uint32_t ymax = 0;
    uint32_t antenna_count = 0;
    Antenna* antennas = parse(&antenna_count,&xmax,&ymax);

    Vec2i* antinodes = malloc(sizeof(Vec2i)*(xmax*ymax));

    uint32_t antinode_count = 0;
    generate_antinodes(antennas,antenna_count,xmax,ymax, antinodes,&antinode_count);
    
    int32_t valid_antinodes = 0;
    for (size_t i = 0; i < antinode_count; i++)
    {
        Vec2i* antinode = &antinodes[i];
        if(antinode->x >= 0 && antinode->x < xmax)
        {
            if(antinode->y >= 0 && antinode->y < ymax)
            {
                valid_antinodes ++;
            }   
        }
    }
    
    printf("antinodes: %i\n", valid_antinodes);
}
int main()
{
    part1();
}