#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned char byte;

typedef struct
{
    byte id_length;
    byte color_map_type;
    byte image_type;
    byte color_map[5];
    byte x_origin[2];
    byte y_origin[2];
    byte width[2];
    byte height[2];
    byte depth;
    byte descriptor;
} TGAHeader;

typedef struct  
{
    byte B;
    byte G;
    byte R;
} Colour;

typedef struct 
{
    byte R;
    byte G;
    byte B;
    byte A;
} ColorOpacity;

typedef struct
{
    int width;
    int height;
    Colour *colour;
} Canvas;

void drawPixel(Canvas *image, int x, int y, ColorOpacity colour)
{
    float alpha = (float)colour.A / 255;
    Colour colourPuvodni;
    colourPuvodni.R = image->colour[x + y * image->width].R;
    colourPuvodni.G = image->colour[x + y * image->width].G;
    colourPuvodni.B = image->colour[x + y * image->width].B;

    if (x >= 0 && x < image->width && y >= 0 && y < image->height) {
        image->colour[x + y * image->width].R = alpha * colour.R + (1.0 - alpha) * colourPuvodni.R;
        image->colour[x + y * image->width].G = alpha * colour.G + (1.0 - alpha) * colourPuvodni.G;
        image->colour[x + y * image->width].B = alpha * colour.B + (1.0 - alpha) * colourPuvodni.B;
    }
}

void drawLine(Canvas *image, int x1, int y1, int x2, int y2, ColorOpacity colour)
{
    int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;
    dx = x2-x1;
    dy = y2-y1;
    dx1 = abs(dx);
    dy1 = abs(dy);
    px = 2 * dy1 - dx1;
    py = 2 * dx1 - dy1;
    if(dy1 <= dx1)
    {
        if(dx >= 0)
        {
            x = x1;
            y = y1;
            xe = x2;
        } else
        {
            x = x2;
            y = y2;
            xe = x1;
        }
        drawPixel(image,x,y,colour);

        for(int i = 0; x < xe; i++)
        {
            x = x + 1;
            if(px < 0)
            {
                px = px + 2 * dy1;
            } else
            {
                if((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                {
                    y = y + 1;
                } else
                {
                    y = y-1;
                }
                px = px + 2 * (dy1 - dx1);
            }
            drawPixel(image,x,y,colour);
        }
    } else
    {
        if(dy >= 0)
        {
            x = x1;
            y = y1;
            ye = y2;
        } else
        {
            x = x2;
            y = y2;
            ye = y1;
        }
        drawPixel(image,x,y,colour);

        for(int i = 0; y < ye; i++)
        {
            y = y + 1;
            if(py <= 0)
            {
                py = py + 2 * dx1;
            } else
            {
                if((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                {
                    x = x+1;
                } else
                {
                    x = x-1;
                }
                py = py + 2 * (dx1 - dy1);
            }
            drawPixel(image,x,y,colour);
        }
    }
}

void drawRectangle(Canvas *image,int x, int y, int width, int height, ColorOpacity colour)
{
    for(int i = y;i < y + height;i++)
    {
        for(int j = x; j < x + width;j++)
        {
            drawPixel(image,j,i,colour);
        }
    }

}
void vykresliKruh(Canvas *image, int xc, int yc, int x, int y,ColorOpacity colour)
{
    drawPixel(image, xc+x, yc+y, colour);
    drawPixel(image, xc-x, yc+y, colour);
    drawPixel(image, xc+x, yc-y, colour);
    drawPixel(image, xc-x, yc-y, colour);
    drawPixel(image, xc+y, yc+x, colour);
    drawPixel(image, xc-y, yc+x, colour);
    drawPixel(image, xc+y, yc-x, colour);
    drawPixel(image, xc-y, yc-x, colour);
}
void drawCircle(Canvas *image, int x, int y, int p, ColorOpacity colour)
{
    int xc = 0, yc = p;
    int d = 3 - 2 * p;
    vykresliKruh(image,x,y,xc,yc,colour);
    while(yc >= xc)
    {
        xc++;
        if(d > 0)
        {
            yc--;
            d = d + 4 * (xc - yc) + 10;
        }
        else
        {   
            d = d + 4 * xc + 6;
        }
        vykresliKruh(image, x, y, xc, yc, colour);
    }
}
// void drawTriangle(Canvas *image,int x, int y, int s, ColorOpacity colour)
// {
    
// }
void drawRotatedRectangle(Canvas *image, int angle, int x,int y,int width, int height, ColorOpacity colour)
{
    float PI = 3.14159265358979323846;
    double anglePI = angle / (180/PI);
    
    for(int i = -(height / 2);i < height /2; i++)
    {
        for(int j = -(width/2);j < width /2;j++)
        {
            drawPixel(image, j * cos(anglePI) - i * sin(anglePI) + x, i * cos(anglePI) + j * sin(anglePI) + y, colour);
        }
    }
}

int main(int argc, char *argv[])
{
    //zde si naimplementuju output, sirku a vysku abych si to mohl ulozit do promennych a mohl s tim dale pracovat
    char *outputPath = NULL;
    int width = 0;
    int height = 0;
    int countOutput = 0;
    int countHeight = 0;
    int countWidth = 0;

    //zde si do promennych nacitam hodnoty zadane uzivatelem, muzou byt v jakemkoliv poradi
    for(int i = 1;i < argc; i++)
    {
        if(strcmp(argv[i - 1], "--output") == 0)
        {
            outputPath = argv[i];
            countOutput++;
        }else if(strcmp(argv[i - 1], "--height") == 0)
        {
            height = atoi(argv[i]);
            countHeight++;
        }else if(strcmp(argv[i - 1], "--width") == 0)
        {
            width = atoi(argv[i]);
            countWidth++;
        }
    }
    //error hlaska kdyz uzivatel zada vice nez potrebny pocet argumentu
    if(argc != 7)
    {
        printf("Wrong input parameters\n");
        exit(1);
    }
    if(countHeight != 1)
    { //zde kdyz uzivatel zada vice nez jeden height
        printf("Too many heights\n");
        exit(1);
    }
    if(countOutput != 1)
    {//zde kdyz uzivatel zada vice nez jeden output path
        printf("Too many output paths\n");
        exit(1);
    }
    if(countWidth != 1)
    {//zde kdyz uzivatel zada vice nez jeden width
        printf("Too many widths\n");
        exit(1);
    }

    TGAHeader header = { 0, 0, 2, { 0, 0, 0, 0, 0 }, { 0, 0 }, { 0, 0 }, { 0,0 }, { 0,0 }, 24, 32 };
    memcpy(header.height,&height,2);
    memcpy(header.width,&width,2);

    Canvas *image = malloc(sizeof(Canvas));
    image->width = width;
    image->height = height;
    image->colour = calloc(sizeof(Colour), width * height);

    //zde zacina cyklus ve kterem uzivatel kresli, neboli volam jednotlive funkce pro kresleni v image
    
    char buffer[100];
    char command[50];
    char *token;
    while(1)
    {
        memset(buffer,0,sizeof(buffer)); // vynulovani bufferu

        fgets(buffer, sizeof(buffer),stdin);
        token = strtok(buffer," ");
        strcpy(command,token);

        if(strcmp(command,"line") == 0)
        {
            int x1,x2,y1,y2,r,g,b;
            float a;
            
            if(sscanf(strtok(NULL," "),"%d,%d,%d,%d,%d,%d,%d,%f",
                                        &x1, &x2, &y1, &y2, &r, &g, &b, &a) == 8)
            {
                ColorOpacity colour;
                colour.R = r;
                colour.G = g;
                colour.B = b;
                colour.A = a;
                if((r >= 0 && r <= 255) && (g >= 0 && g <= 255) && (b >= 0 && b <= 255))
                {
                    drawLine(image,x1,x2,y1,y2,colour);
                }else
                {
                    printf("Wrong color input\n");
                }
            }else
            {
                printf("Wrong function input :(\n");
            }
        }
        else if(strcmp(command,"rectangle") == 0)
        {
            int x,y,width,height,r,g,b;
            float a;
            if(sscanf(strtok(NULL," "),"%d,%d,%d,%d,%d,%d,%d,%f",
                                        &x, &y, &height, &width, &r, &g, &b, &a) == 8)
            {
                ColorOpacity colour;
                colour.R = r;
                colour.G = g;
                colour.B = b;
                colour.A = a;

                if((r >= 0 && r <= 255) && (g >= 0 && g <= 255) && (b >= 0 && b <= 255))
                {
                    drawRectangle(image,x,y,height,width,colour);
                }else
                {
                    printf("Wrong color input\n");
                }

            }else
            {
                printf("Wrong function input, try again\n");
            }
        }
        else if(strcmp(command,"circle") == 0)
        {
            int x,y,p,r,g,b;
            float a;
            
            if(sscanf(strtok(NULL," "),"%d,%d,%d,%d,%d,%d,%f",
                                        &x, &y, &p, &r, &g, &b, &a) == 7)
            {
                ColorOpacity colour;
                colour.R = r;
                colour.G = g;
                colour.B = b;
                colour.A = a;
                
                if((r >= 0 && r <= 255) && (g >= 0 && g <= 255) && (b >= 0 && b <= 255))
                {
                    drawCircle(image,x,y,p,colour);
                }else
                {
                    printf("Wrong color input\n");
                }
            }else
            {
                printf("Wrong function input :(\n");
            }
        }

        // else if(strcmp(command,"triangle") == 0)
        // {
        //     int x,y,s,r,g,b;
        //     float a;
        //     if(sscanf(strtok(NULL," "),"%d,%d,%d,%d,%d,%d,%f",
        //                                 &x, &y, &s, &r, &g, &b, &a) == 6)
        //     {
        //         ColorOpacity colour;
        //         colour.R = r;
        //         colour.G = g;
        //         colour.B = b;
        //         colour.A = a;

        //         if((r >= 0 && r <= 255) && (g >= 0 && g <= 255) && (b >= 0 && b <= 255))
        //         {
        //             drawTriangle(image,x,y,s,colour);
        //         }else
        //         {
        //             printf("Wrong color input\n");
        //         }
        //     }
            // else
            // {
            //     printf("Wrong function input :(\n");
            // }

        //}
        else if(strcmp(command,"rotated-rectangle") == 0)
        {
            int angle,x,y,width,height,r,g,b;
            float a;
            
            if(sscanf(strtok(NULL," "),"%d,%d,%d,%d,%d,%d,%d,%d,%f",
                                        &angle, &x, &y, &height, &width, &r, &g, &b, &a) == 9)
            {
                ColorOpacity colour;
                colour.R = r;
                colour.G = g;
                colour.B = b;
                colour.A = a;

                if((r >= 0 && r <= 255) && (g >= 0 && g <= 255) && (b >= 0 && b <= 255))
                {
                    drawRotatedRectangle(image,angle,x,y,height,width,colour);
                }else
                {
                    printf("Wrong color input\n");
                }
            }else
            {
                printf("Wrong function input :(\n");
            }

        }
        else if(strcmp(buffer, "save\n") == 0)
        {
            FILE* output = fopen(outputPath, "wb");
            fwrite(&header, sizeof(header),1,output);
            fwrite(image->colour, sizeof(Colour), height * width, output);
            fclose(output);
        }
        else if(strcmp(buffer, "exit\n") == 0)
        {
            FILE* output = fopen(outputPath, "wb");
            fwrite(&header, sizeof(header),1,output);
            fwrite(image->colour, sizeof(Colour), height * width, output);
            fclose(output);
            break;
        }else
        {
            printf("Wrong called function, try again.\n");
        }
        
    }
    
    free(image);
    return 0;
}
