
//Using OpenCL 1.2
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
//enable exceptions
#define __CL_ENABLE_EXCEPTIONS

//C++ library and STL headers
#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <ctime>

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
	//Platform
	cl::Platform platform;
	//Device
	cl::Device devices;
	Context details;
	Program programming;
	Kernel kernel;
	CommandQueue queue;

	//declare data and memory object
	unsigned char* inputImage;
	unsigned char* OutputImage;
	int width, height, size;

	//create buffer
	ImageFormat imageFormat;
	Image2D inImageBuffer, outImageBuffer;
	Buffer optionBuffer;

	//create profiling event and record average time
	Event profileEvent;
	cl_ulong timeStart, timeEnd, timeTotal;

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
			bool programBuilt = build_program(&programming, &details, "/Users/isaacyeo/Desktop/A2Task3B/A2Task3B/Task3b.cl");
			if (programBuilt == false)
			{
				// if OpenCL program build error
				quit_program("OpenCL program build error.");
			}
			else
			{
			
				int option = 0;
//
				string outputname;

					outputname = "/Users/isaacyeo/Desktop/A2Task3B/A2Task3B/output.bmp";

				string fileName = "";

					fileName = "/Users/isaacyeo/Desktop/A2Task3B/A2Task3B/peppers.bmp";


				//create kernel
				kernel = Kernel(programming, "blur_image");
				//create command queue and enable profiling
				queue = CommandQueue(details, devices, CL_QUEUE_PROFILING_ENABLE);
				inputImage = read_BMP_RGB_to_RGBA(fileName, &width, &height);

				//allocate memory
				size = width * height * 4;
				OutputImage = new unsigned char[size];

				imageFormat = ImageFormat(CL_RGBA, CL_UNORM_INT8);

				//create image object
				optionBuffer = Buffer(details, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * 1, &option);
				inImageBuffer = Image2D(details, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imageFormat, width, height, 0, (void*)inputImage);
				outImageBuffer = Image2D(details, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imageFormat, width, height, 0, (void*)OutputImage);

				// set kernel arguments
				kernel.setArg(0, optionBuffer);
				kernel.setArg(1, inImageBuffer);
				kernel.setArg(2, outImageBuffer);

				//read image from devices to host memory
				cl::size_t<3> origin, region;
				origin[0] = origin[1] = origin[2] = 0;
				region[0] = width;
				region[1] = height;
				region[2] = 1;

				NDRange offset(0, 0);
				//for filter window and first pass in horizontal direction
				NDRange globalSize(width, height);
				//initialize timeTotal to 0
				timeTotal = 0;
				for (int i = 0; i < NUM_ITERATIONS; i++)
				{
					queue.enqueueNDRangeKernel(kernel, offset, globalSize, NullRange, NULL, &profileEvent);
					queue.enqueueReadImage(outImageBuffer, CL_TRUE, origin, region, 0, 0, OutputImage);

					timeStart = profileEvent.getProfilingInfo<CL_PROFILING_COMMAND_START>();
					timeEnd = profileEvent.getProfilingInfo<CL_PROFILING_COMMAND_END>();
					timeTotal += timeEnd - timeStart;
				}

				// output results to image file
				write_BMP_RGBA_to_RGB(outputname, OutputImage, width, height);

				cout << endl;
				cout << "Image exported." << endl;
				

				// deallocate memory
				delete inputImage;
				delete OutputImage;
			}
		}
		
	}
	catch (Error e)
	{
		handle_error(e);
		cout << e.err() << " (" << lookup_error_code(e.err()) << ")" << endl;
	}
	system("Pause");
	return 0;
}
