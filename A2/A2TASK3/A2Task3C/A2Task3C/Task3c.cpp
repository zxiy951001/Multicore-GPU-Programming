//Using OpenCL 1.2
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
//enable exceptions
#define __CL_ENABLE_EXCEPTIONS

//C++ library and STL headers
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

//OpenCL header dependant on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#define NUM_ITERATIONS 1000


#include "common.h"
#include "bmpfuncs.h"
using namespace std;
using namespace cl;

int main()
{
	//get selected platform
	Device devices;
	Context details;
	Program programming;
	cl::Platform platform;
	Kernel kernel[2];
	CommandQueue queue;
	enum Kernels { BLUR_IMAGE, BLUR_VERTICAL };

	//declare data and memory object
//	int size = 0;
	unsigned char* inImage;
//	unsigned char* inImage1;
//	unsigned char* inImage2;
	unsigned char* outImage;
	int width, height;

	//create buffer
	ImageFormat imageFormat;
	Image2D inImageBuffer, outImageBuffer;
	Buffer optionBuffer;

	bool deviceSelected = true;
	try
	{
		if (!select_one_device(&platform, &devices))
		{
			// if no device selected
			quit_program("Device not selected.");
		}
		else
		{
			//create details
			details = Context(devices);
			cout << endl;
			cout << "Checking if programming is built successfully..." << endl;
			bool programBuilt = build_program(&programming, &details, "/Users/isaacyeo/Desktop/A2Task3C/A2Task3C/Task3c.cl");
			if (programBuilt == false)
			{
				// if OpenCL program build error
				quit_program("OpenCL program build error.");
			}
			else
			{
				//-----user to choose which filter option to use -----//
				//for final choice
				int option = 0;
				//variable to contain temporary choice from user
				string choice = "0";
				
				string fileName = "/Users/isaacyeo/Desktop/A2Task3C/A2Task3C/peppers.bmp";

				//average luminance value from part 2
				float luminanceThreshold = 0;
                cout<< "Enter luminance Treshold: ";
                cin>>luminanceThreshold;
			

				//keep pixel colour if above average luminance value, else change to black
				GetGlowingPixels(fileName, luminanceThreshold);
				cout << " image showing the glowing pixels exported" << endl;

				//perform horizontal then vertical blur pass and export image-----
				//create kernel
				kernel[BLUR_IMAGE] = Kernel(programming, "blur_image");
				kernel[BLUR_VERTICAL] = Kernel(programming, "blur_vertical");

				//create command queue and enable profiling
				queue = CommandQueue(details, devices);
				inImage = read_BMP_RGB_to_RGBA("/Users/isaacyeo/Desktop/A2Task3C/A2Task3C/glow.bmp", &width, &height);

				//allocate memory
				int size = width * height * 4;
				outImage = new unsigned char[size];

				imageFormat = ImageFormat(CL_RGBA, CL_UNORM_INT8);
				//create image object
				optionBuffer = Buffer(details, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * 1, &option);
				inImageBuffer = Image2D(details, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imageFormat, width, height, 0, (void*)inImage);
				outImageBuffer = Image2D(details, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imageFormat, width, height, 0, (void*)outImage);

				// set kernel arguments
				kernel[BLUR_IMAGE].setArg(0, optionBuffer);
				kernel[BLUR_IMAGE].setArg(1, inImageBuffer);
				kernel[BLUR_IMAGE].setArg(2, outImageBuffer);

				//read image from device to host memory
				cl::size_t<3> origin, region;
				origin[0] = origin[1] = origin[2] = 0;
				region[0] = width;
				region[1] = height;
				region[2] = 1;

				NDRange offset(0, 0);
				NDRange globalSize(width, height);
				//initialize timeTotal to 0

				//horizontal pass
				queue.enqueueNDRangeKernel(kernel[BLUR_IMAGE], offset, globalSize, NullRange);
				queue.enqueueReadImage(outImageBuffer, CL_TRUE, origin, region, 0, 0, outImage);

					//get all after horizontal pass image 
					write_BMP_RGBA_to_RGB("/Users/isaacyeo/Desktop/A2Task3C/A2Task3C/Afterhorizontalblur.bmp", outImage, width, height);
					cout << "After horizontal blur image exported" << endl;
					free(inImage);
					free(outImage);

					//get after vertical pass image
					inImage = read_BMP_RGB_to_RGBA("/Users/isaacyeo/Desktop/A2Task3C/A2Task3C/Afterhorizontalblur.bmp", &width, &height);
					outImage = new unsigned char[size];
					inImageBuffer = Image2D(details, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imageFormat, width, height, 0, (void*)inImage);
					kernel[BLUR_VERTICAL].setArg(0, optionBuffer);
					kernel[BLUR_VERTICAL].setArg(1, inImageBuffer);
					kernel[BLUR_VERTICAL].setArg(2, outImageBuffer);

					queue.enqueueNDRangeKernel(kernel[BLUR_VERTICAL], offset, globalSize, NullRange);
					queue.enqueueReadImage(outImageBuffer, CL_TRUE, origin, region, 0, 0, outImage);
					write_BMP_RGBA_to_RGB("/Users/isaacyeo/Desktop/A2Task3C/A2Task3C/Afterverticalblur.bmp", outImage, width, height);
					cout << "After vertical blur image exported." << endl;
			

					//get bloom effect
					AddPixelsTogether(fileName, "/Users/isaacyeo/Desktop/A2Task3C/A2Task3C/Afterverticalblur.bmp");
					cout << "final.bmp exported" << endl;
					free(inImage);
					free(outImage);
				



				


			}
		}
		cout << endl;
		
	}
	catch (Error e)
	{
		handle_error(e);
		cout << e.err() << " (" << lookup_error_code(e.err()) << ")" << endl;
	}
	system("Pause");
	return 0;
}
