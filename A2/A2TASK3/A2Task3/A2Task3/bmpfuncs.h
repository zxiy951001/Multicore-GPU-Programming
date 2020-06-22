#ifndef __BMPFUNCS__
#define __BMPFUNCS__

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

// reads the contents of a 24-bit RGB bitmap file and returns it in RGBA format
void BMP_RGB_to_Luminous(string filename, int* widthOut, int* heightOut, vector<float> &luminance);


#endif
