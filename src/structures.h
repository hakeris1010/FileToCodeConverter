#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

#include <string>

#define FILETYPE_BINARY 0
#define FILETYPE_BITMAP 1
#define FILETYPE_TEXT 2

struct BitMap
{
    //essentials
    long width;
    long height;
    int bits_per_pixel;
    int where_pixels_start;

    //format 1
    unsigned char *pixels;
    //format 2
    unsigned char *RED;
    unsigned char *GREEN;
    unsigned char *BLUE;
    unsigned char *ALPHA;
};

struct TextFile
{
    std::string encoding;
};

struct AdditionalInfo
{
    int file_type;
    bool supported;
    int till_where_to_read;

    BitMap pic;
};

#endif // STRUCTURES_H_INCLUDED
