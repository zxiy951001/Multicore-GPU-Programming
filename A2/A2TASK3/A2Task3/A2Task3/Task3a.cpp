
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
	Kernel kernel;
	CommandQueue queue;
	cl::Platform platform;

	std::size_t workGroupSize;
	std::size_t kWorkGroupSize;
	cl_ulong localMemSize;

	//declare data and memory object
	int width, height, size = 0;
	Buffer bufferA, bufferB;
	LocalSpaceArg localSpace;

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
			bool programBuilt = build_program(&programming, &details, "/Users/isaacyeo/Desktop/A2Task3/A2Task3/Task3a.cl");
			if (programBuilt == false)
			{
				// if OpenCL program build error
				quit_program("OpenCL program build error.");
			}
			else
			{
//
				string fileName = "/Users/isaacyeo/Desktop/A2Task3/A2Task3/peppers.bmp";
//				

				vector <float> luminanceVector;

				//convert the RGB values in an image to luminance values
				//set each RGB to luminance values and save image
				BMP_RGB_to_Luminous(fileName, &width, &height, luminanceVector);


				//find total number of pixels
				int numOfElements = width * height;
				//store all information to vector of cl_float
				vector <cl_float>imageData(numOfElements);
				//transfer all information from luminaceVector to imageData
				for (unsigned int i = 0; i < luminanceVector.size(); i++)
				{
					imageData[i] = luminanceVector[i];
				}

				//create kernel
				kernel = Kernel(programming, "reduction");
				//create command queue 
				queue = CommandQueue(details, devices);
				//get information
				workGroupSize = devices.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
				localMemSize = devices.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();
				kWorkGroupSize = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(devices);

				
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
