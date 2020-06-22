#ifndef __BMPFUNCS__
#define __BMPFUNCS__

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// reads the contents of a 24-bit RGB bitmap file and returns it in RGBA format
unsigned char* read_BMP_RGB_to_RGBA(string filename, int* widthOut, int* heightOut);

// writes imageData (in RGBA format) to a 24-bit RGB bitmap file
void write_BMP_RGBA_to_RGB(string filename, unsigned char* imageData, int width, int height);

void GetGlowingPixels(string filename, float average);

void AddPixelsTogether(string originalFile, string toAdd);
#endif
