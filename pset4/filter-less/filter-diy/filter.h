#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

// Aliases for types
typedef uint8_t     BYTE;
typedef uint16_t    WORD;
typedef uint32_t    DWORD;
typedef int32_t     LONG;     

// Structures for BMP headers
#pragma pack(push, 1)     // Ensures the struct is packed without padding
typedef struct BMPFileHeader
{
    WORD    type;       // file type
    DWORD   size;       // file size
    WORD    reserved1;  // reserved 1
    WORD    reserved2;  // reserved 2
    DWORD   offset;     // offset
}
BMPFileHeader;

typedef struct BMPInfoHeader
{
    DWORD   header_size;        // header size
    LONG    width;              // width
    LONG    height;             // height
    WORD    planes;             // planes
    WORD    bit_count;          // bit count
    DWORD   compression;        // compression
    DWORD   image_size;         // image size
    LONG    x_pixels_per_meter; // x pixel per meter
    LONG    y_pixels_per_meter; // y pixels per meter
    DWORD   colours_used;       // colours used
    DWORD   important_colours;  // important colours
}
BMPInfoHeader;
#pragma pack(pop)

// Structure for pixel
#pragma pack(push , 1)      // Ensure that struct is packed without padding
typedef struct RGBPixel
{
    BYTE blue;
    BYTE green;
    BYTE red;
}
RGBPixel;
#pragma pack(pop)

typedef RGBPixel** BMPImagePixelData;


// Struct for a BMP Image
typedef struct BMPImage
{
    BMPFileHeader *file_header;     // the image file header
    BMPInfoHeader *info_header;     // the image info header
    BMPImagePixelData pixel_data;   // the pixel data
}
BMPImage;

// Create BMPImage from a file.
// Free image memory after operation, use `destroyBMPImage()`
BMPImage *createBMPImage(const char *filename);

// Frees the memory for the BMP Image
void destroyBMPImage(BMPImage *img);

// Write BMPImage to a file
void writeBMPImage(BMPImage *img, const char *filename);

// Print basic BMP information
void print_bmp_info(BMPImage img);

// Get the height of an image
int get_height(BMPImage img);

// Get the width of an image
int get_width(BMPImage img);

// Convert image to grayscale
void grayscale(BMPImage *img);

// Convert image to sepia
void sepia(BMPImage *img);

// Reflect image horizontally
void reflect(BMPImage *img);

// Blur image
void blur(BMPImage *img);

// Detect edges
void edges(BMPImage *img);

#endif