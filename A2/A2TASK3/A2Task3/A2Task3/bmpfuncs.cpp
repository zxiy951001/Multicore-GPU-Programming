#include "bmpfuncs.h"

// reads the contents of a 24-bit RGB bitmap file and returns it in RGBA format
void BMP_RGB_to_Luminous(string filename, int* widthOut, int* heightOut, vector<float> &luminance)
{
	char fileHeader[54];
	int width, height;
	int offset, imageSize, fileSize;
	char colour[3];
	unsigned char* imageData;

	// open image file
	ifstream getImageFile(filename, ios::in | ios::binary);
	// check whether file opened successfully
	if (getImageFile.is_open() == false)
	{
		cout << "Failed to open texture file - " << filename << endl;
	}
	//process file header
	getImageFile.read(fileHeader, 54);

	//get information for offset, width and height
	offset = fileHeader[10];
	width = fileHeader[21] << 24;
	width |= fileHeader[20] << 16;
	width |= fileHeader[19] << 8;
	width |= fileHeader[18];
	width = abs(width);
	height = fileHeader[25] << 24;
	height |= fileHeader[24] << 16;
	height |= fileHeader[23] << 8;
	height |= fileHeader[22];
	height = abs(height);

	//create imageData to store RGB values of each pixels
	imageData = new unsigned char[width*height * 3];
	//get each rgb value
	int rgbRow = width * 3;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < rgbRow; j += 3) {
			getImageFile.read(colour, 3);
			imageData[i*rgbRow + j] = colour[0];
			imageData[i*rgbRow + j + 1] = colour[1];
			imageData[i*rgbRow + j + 2] = colour[2];
		}
	}
	//close file 
	getImageFile.close();

	//create file to be exported
	ofstream outFileStream("/Users/isaacyeo/Desktop/CSCI376_A2_6342425_YeoZhengXuIsaac/A2TASK3/A2Task3/A2Task3/output.bmp", ios::out | ios::binary);
	if (outFileStream.is_open() == false)
	{
		cout << "Failed to open output file - " << filename << endl;
	}
    else{
        cout << "Output File Generated in Assignment Folder - " << filename << endl;
        
    }
	//get size of RGB information
	imageSize = width * height * 3;
	//compute file size
	fileSize = 54 + imageSize;

	//fill in appropriate bmp header fields
	fileHeader[2] = (unsigned char)fileSize;
	fileHeader[3] = (unsigned char)(fileSize >> 8);
	fileHeader[4] = (unsigned char)(fileSize >> 16);
	fileHeader[5] = (unsigned char)(fileSize >> 24);

	fileHeader[18] = (unsigned char)width;
	fileHeader[19] = (unsigned char)(width >> 8);
	fileHeader[20] = (unsigned char)(width >> 16);
	fileHeader[21] = (unsigned char)(width >> 24);

	fileHeader[22] = (unsigned char)height;
	fileHeader[23] = (unsigned char)(height >> 8);
	fileHeader[24] = (unsigned char)(height >> 16);
	fileHeader[25] = (unsigned char)(height >> 24);

	fileHeader[34] = (unsigned char)imageSize;
	fileHeader[35] = (unsigned char)(imageSize >> 8);
	fileHeader[36] = (unsigned char)(imageSize >> 16);
	fileHeader[37] = (unsigned char)(imageSize >> 24);

	//write file
	outFileStream.write(fileHeader, 54);
	int r = 0, g = 0, b = 0;
	float tempLuminance = 0.0;
	//set each RGB pixel colour to image
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < rgbRow; j += 3) {
			//get rgb information
			r = imageData[i*rgbRow + j];
			g = imageData[i*rgbRow + j + 1];
			b = imageData[i*rgbRow + j + 2];
			//using luminance formula '0.299*R + 0.587*G + 0.114*B' set each color to luminance value
			tempLuminance = (float)(0.299*r + 0.587*g + 0.114*b);
			luminance.push_back(tempLuminance);
			colour[0] = tempLuminance;
			colour[1] = tempLuminance;
			colour[2] = tempLuminance;
			outFileStream.write(colour, 3);
		}
	}

	outFileStream.close();

	// record width and height, and return pointer to image data
	*widthOut = width;
	*heightOut = height;
}




