#include "bmpfuncs.h"

// reads the contents of a 24-bit RGB bitmap file and returns it in RGBA format
unsigned char* read_BMP_RGB_to_RGBA(string filename, int* widthOut, int* heightOut)
{
	char fileHeader[54];	// to store the file header, bmp file format bmpheader (14 bytes) + bmpheaderinfo (40 bytes) = 54 bytes 
	int width, height;		// width and height of image
	int offset;				// offset where image data starts in the file
	int imageSize;			// image size in bytes
	int padSize;			// in the bmp file format, each row must be a multiple of 4
	int rowSize;			// size per row in bytes
	int i, j;
	char colour[3];
	unsigned char* imageData;	// pointer to store image data

	// open file stream
	ifstream textureFileStream(filename, ios::in | ios::binary);
		
	// check whether file opened successfully
	if (!textureFileStream.is_open())
	{
		cout << "Failed to open texture file - " << filename << endl;
		return NULL;
	}

	// get file header
	textureFileStream.read(fileHeader, 54);

	// get offset, width and height information
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

	// allocate RGBA image data
	imageSize = width * height * 4;
	imageData = new unsigned char[imageSize];

	// move read position by offset
	textureFileStream.seekg(offset, ios::beg);

	// compute amount of row padding
	padSize = width % 4;
	if (padSize != 0) {
		padSize = 4 - padSize;
	}

	// bitmaps are stored in upside-down raster order
	rowSize = width * 4;

	// read each RGB pixel colour
	for (i = 0; i < height; i++) {
		for (j = 0; j < rowSize; j += 4) {
			textureFileStream.read(colour, 3);
			imageData[i*rowSize + j] = colour[0];
			imageData[i*rowSize + j + 1] = colour[1];
			imageData[i*rowSize + j + 2] = colour[2];
			imageData[i*rowSize + j + 3] = 255;
		}
		// in the bmp format, each row has to be a multiple of 4
		// read in the junk data and throw it away
		for (j = 0; j < padSize; j++) {
			textureFileStream.read(colour, 3);
		}
	}

	// close file stream
	textureFileStream.close();

	// record width and height, and return pointer to image data
	*widthOut = width;
	*heightOut = height;

	return imageData;
}


void GetGlowingPixels(string filename, float average)
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

	// compute amount of row padding

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
	ofstream outFileStream("/Users/isaacyeo/Desktop/A2Task3C/A2Task3C/glow.bmp", ios::out | ios::binary);
	if (outFileStream.is_open() == false)
	{
		cout << "Failed to open output file - glowImage.bmp" << endl;
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
			//using luminance formula '0.299*R + 0.587*G + 0.114*B' then set each rgb value to the luminance value
			tempLuminance = (float)(0.299*r + 0.587*g + 0.114*b);
			//change pixel to black if luminance value less than average value
			if (tempLuminance < (int)average)
			{
				colour[0] = 0;
				colour[1] = 0;
				colour[2] = 0;
				outFileStream.write(colour, 3);
			}
			else //keep original colour
			{
				colour[0] = imageData[i*rgbRow + j];
				colour[1] = imageData[i*rgbRow + j + 1];
				colour[2] = imageData[i*rgbRow + j + 2];
				outFileStream.write(colour, 3);
			}

		}
	}

	outFileStream.close();
}
// writes imageData (in RGBA format) to a 24-bit RGB bitmap file
void write_BMP_RGBA_to_RGB(string filename, unsigned char* imageData, int width, int height)
{
	char fileHeader[54] = {
		// BITMAPHEADER
		'B','M',		// bmp file
		0, 0, 0, 0,		// file size in bytes
		0, 0,			// reserved
		0, 0,			// reserved
		54, 0, 0, 0,	// offset	
		// BITMAPINFOHEADER
		40, 0, 0, 0,	// size of info header
		0, 0, 0, 0,		// width
		0, 0, 0, 0,		// heigth
		1, 0,			// number colour planes
		24, 0,			// number of bits per pixel
		0, 0, 0, 0,		// compression
		0, 0, 0, 0,		// image size
		0, 0, 0, 0,		// horizontal resolution
		0, 0, 0, 0,		// vertical resolution
		0, 0, 0, 0,		// number of colours in palette
		0, 0, 0, 0,		// number of important colours
	};
	int imageSize;		// image size in bytes
	int padSize;		// in the bmp file format, each row must be a multiple of 4
	int rowSize;		// size per row in bytes
	int fileSize;		// file size in bytes (image size + header size)
	int i, j;
	char colour[3];

	// compute amount of padding so that each row is a multiple of 4
	padSize = width % 4;
	if (padSize != 0) {
		padSize = 4 - padSize;
	}

	// compute image size
	imageSize = (width + padSize) * height * 3;

	// open output stream
	ofstream outFileStream(filename, ios::out | ios::binary);

	// check whether output stream opened successfully
	if (!outFileStream.is_open())
	{
		cout << "Failed to open output file - " << filename << endl;
		return;
	}

	// compute file size (image size + header size)
	fileSize = 54 + imageSize;

	// fill in appropriate bmp header fields in little endian order
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

	// write file header to out stream
	outFileStream.write(fileHeader, 54);

	// bitmaps are stored in upside-down raster order
	rowSize = width * 4;

	// output each RGB pixel colour
	for (i = 0; i < height; i++) {
		for (j = 0; j < rowSize; j += 4) {
			colour[0] = (unsigned char)imageData[i*rowSize + j];
			colour[1] = (unsigned char)imageData[i*rowSize + j + 1];
			colour[2] = (unsigned char)imageData[i*rowSize + j + 2];
			outFileStream.write(colour, 3);
		}

		// in bmp format rows must be a multiple of 4-bytes, add junk padding if required
		for (j = 0; j < padSize; j++) {
			outFileStream.write(colour, 3);
		}
	}

	// close output file stream
	outFileStream.close();
}

//combine pixels of both image for glow effect
void AddPixelsTogether(string originalFile, string toAdd)
{
	char fileHeader[54];
	int width, height;
	int offset, imageSize, fileSize;
	char colour[3];
	unsigned char *imageData, *addData;

	// open image file
	ifstream getImageFile(originalFile, ios::in | ios::binary);
	// check whether file opened successfully
	if (getImageFile.is_open() == false)
	{
		cout << "Failed to open texture file - " << originalFile << endl;
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

	// open second image file
	ifstream getAddFile(toAdd, ios::in | ios::binary);
	// check whether file opened successfully
	if (getAddFile.is_open() == false)
	{
		cout << "Failed to open texture file - " << toAdd << endl;
	}
	//process file header
	getAddFile.read(fileHeader, 54);
	//create imageData to store RGB values of each pixels
	addData = new unsigned char[width*height * 3];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < rgbRow; j += 3) {
			getAddFile.read(colour, 3);
			addData[i*rgbRow + j] = colour[0];
			addData[i*rgbRow + j + 1] = colour[1];
			addData[i*rgbRow + j + 2] = colour[2];
		}
	}
	//close file 
	getAddFile.close();


	//create file to be exported
	ofstream outFileStream("/Users/isaacyeo/Desktop/A2Task3C/A2Task3C/final.bmp", ios::out | ios::binary);
	if (outFileStream.is_open() == false)
	{
		cout << "Failed to open output file - final.bmp" << endl;
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
	//set each RGB pixel colour to image
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < rgbRow; j += 3) {
			//get rgb information and add the pixels together
			r = imageData[i*rgbRow + j] + addData[i*rgbRow + j];
			g = imageData[i*rgbRow + j + 1] + addData[i*rgbRow + j + 1];
			b = imageData[i*rgbRow + j + 2] + addData[i*rgbRow + j + 2];
			//keep max value to 255 if the sum exceeds
			if (r > 255)
			{
				colour[0] = 255;
			}
			else
			{
				colour[0] = r;
			}

			if (g > 255)
			{
				colour[1] = 255;
			}
			else
			{
				colour[1] = g;
			}
			if (b > 255)
			{
				colour[2] = 255;
			}
			else
			{
				colour[2] = b;
			}
			outFileStream.write(colour, 3);
		}
	}

	outFileStream.close();
	delete[] imageData;
	delete[] addData;
}
