#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

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
    char* input_raw = load_input("input.txt");
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
            antenna_index++;

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
    uint32_t a_count = 0;

    for (size_t i = 0; i < antenna_count; i++)
    {
        Antenna* a = &antennas[i];

        for (size_t j = 0; j < antenna_count; j++)
        {
            if(i == j) continue;
            Antenna* n = &antennas[j];
            if(n->freq == a->freq)
            {
                int32_t dx = n->pos.x - a->pos.x;
                int32_t dy = n->pos.y - a->pos.y;

                Vec2i antinode1 = {a->pos.x - dx, a->pos.y - dy};
                Vec2i antinode2 = {n->pos.x + dx, n->pos.y + dy};

                uint8_t found1 = 0;
                uint8_t found2 = 0;
                
                for (size_t k = 0; k < a_count; k++)
                {
                    Vec2i* ak = &antinodes[k];

                    if(antinode1.x == ak->x && antinode1.y == ak->y) 
                    {
                        found1 = 1;
                    }
                    if(antinode2.x == ak->x && antinode2.y == ak->y)
                    {
                        found2 = 1;
                    }
                }

                uint8_t valid1 = (antinode1.x >= 0 && antinode1.x < xmax) && (antinode1.y >= 0 && antinode1.y <= ymax);
                uint8_t valid2 = (antinode2.x >= 0 && antinode2.x < xmax) && (antinode2.y >= 0 && antinode2.y <= ymax);
            
                if(!found1 && valid1)
                {
                    antinodes[a_count] = antinode1;
                    a_count++;
                }
                if(!found2 && valid2)
                {
                    antinodes[a_count] = antinode2;
                    a_count++;
                }
                
            }
        }
    }
    *antinode_count = a_count;
}
uint8_t antinode_is_valid(Vec2i antinode, Vec2i* antinodes,uint32_t antinode_count, uint32_t xmax, uint32_t ymax)
{
    uint8_t valid = 1;

    for (size_t i = 0; i < antinode_count; i++)
    {
        if(antinode.x == antinodes[i].x && antinode.y == antinodes[i].y) 
        {
            valid = 0;
            break;
        }
    }

    if(valid)
    {
        valid = (antinode.x >= 0 && antinode.x < xmax) && (antinode.y >= 0 && antinode.y <= ymax);
    }
    return valid;
}
void generate_antinodes2(Antenna* antennas,uint32_t antenna_count,uint32_t xmax,uint32_t ymax,Vec2i* antinodes,uint32_t* antinode_count)
{
    uint32_t a_count = 0;

    for (size_t i = 0; i < antenna_count; i++)
    {
        Antenna* a = &antennas[i];

        for (size_t j = 0; j < antenna_count; j++)
        {
            if(i == j) continue;
            Antenna* n = &antennas[j];
            if(n->freq == a->freq)
            {
                int32_t dx = n->pos.x - a->pos.x;
                int32_t dy = n->pos.y - a->pos.y;

                Vec2i antinode1 = {a->pos.x - dx, a->pos.y - dy};
                Vec2i antinode2 = {n->pos.x + dx, n->pos.y + dy};

                uint8_t valid1 = antinode_is_valid(antinode1,antinodes,a_count,xmax,ymax);
                uint8_t valid2 = antinode_is_valid(antinode2,antinodes,a_count,xmax,ymax);
            
                if(valid1)
                {
                    antinodes[a_count] = antinode1;
                    a_count++;
                }
                if(valid2)
                {
                    antinodes[a_count] = antinode2;
                    a_count++;
                }

                int32_t sx = a->pos.x;
                int32_t sy = a->pos.y;

                while(1)
                {
                    Vec2i between = (Vec2i){sx,sy};
                    uint8_t valid = antinode_is_valid(between,antinodes,a_count,xmax,ymax);
                    if(valid)
                    {
                        antinodes[a_count] = between;
                        a_count++;
                    }
                    sx += dx;
                    sy += dy;

                    if(sx < 0 || sx >= xmax)break;
                    if(sy < 0 || sy >= ymax)break;
                }
                 while(1)
                {
                    Vec2i between = (Vec2i){sx,sy};
                    uint8_t valid = antinode_is_valid(between,antinodes,a_count,xmax,ymax);
                    if(valid)
                    {
                        antinodes[a_count] = between;
                        a_count++;
                    }
                    sx -= dx;
                    sy -= dy;

                    if(sx < 0 || sx >= xmax)break;
                    if(sy < 0 || sy >= ymax)break;
                }
                
            }
        }
    }
    *antinode_count = a_count;
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
    
    for (size_t y = 0; y <= ymax; y++)
    {
        for (size_t x = 0; x < xmax; x++)
        {
            uint8_t is_antenna = 0;
            uint8_t is_antinode = 0;
           

           char freq = '.';
            for (size_t i = 0; i < antenna_count; i++)
            {
                Antenna* a = &antennas[i];
                if(a->pos.x == x && a->pos.y == y)
                {
                    freq = a->freq;
                    is_antenna = 1;
                    break;
                }
            }

            for (size_t i = 0; i < antinode_count; i++)
            {
                Vec2i* a = &antinodes[i];
                if(a->x == x && a->y == y)
                {
                    is_antinode = 1;
                    break;
                }
            }

            
             if(is_antinode)
            {
                printf("#");
            }
            else if(is_antenna)
            {
                printf("%c",freq);
            }
            else
            {
                printf(".");
            }
            
               
            
        }
        printf("\n");
    }

    printf("antinodes: %i\n", antinode_count);
}
void part2()
{
    uint32_t xmax = 0;
    uint32_t ymax = 0;
    uint32_t antenna_count = 0;
    Antenna* antennas = parse(&antenna_count,&xmax,&ymax);

    Vec2i* antinodes = malloc(sizeof(Vec2i)*(xmax*ymax));

    uint32_t antinode_count = 0;
    generate_antinodes2(antennas,antenna_count,xmax,ymax, antinodes,&antinode_count);
    
    for (size_t y = 0; y <= ymax; y++)
    {
        for (size_t x = 0; x < xmax; x++)
        {
            uint8_t is_antenna = 0;
            uint8_t is_antinode = 0;
           

           char freq = '.';
            for (size_t i = 0; i < antenna_count; i++)
            {
                Antenna* a = &antennas[i];
                if(a->pos.x == x && a->pos.y == y)
                {
                    freq = a->freq;
                    is_antenna = 1;
                    break;
                }
            }

            for (size_t i = 0; i < antinode_count; i++)
            {
                Vec2i* a = &antinodes[i];
                if(a->x == x && a->y == y)
                {
                    is_antinode = 1;
                    break;
                }
            }

            
             if(is_antinode)
            {
                printf("#");
            }
            else if(is_antenna)
            {
                printf("%c",freq);
            }
            else
            {
                printf(".");
            }
            
               
            
        }
        printf("\n");
    }

    printf("antinodes: %i\n", antinode_count);
}
int main()
{
    //part1();
    part2();
}