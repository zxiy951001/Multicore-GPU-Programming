#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h> 
#include <ctime>
#include <algorithm>  

// OpenCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include "common.h"

#define LENGTH 1024
#define LENGTH2 1


using namespace cl;


int main(void)
{
	cl::Platform platform;			// device's platform
	cl::Device device;				// device used
	cl::Context context;			// context for the device
	cl::Program program;			// OpenCL program object
	cl::Kernel kernel;				// a single kernel object
	cl::CommandQueue queue;			// commandqueue for a context and device

	// declare data and memory objects
	
    int Arraytest1 [1024] = {};

    cl::Buffer outputBuffer;

	try {
		// select an OpenCL device
		if (!select_one_device(&platform, &device))
		{
			// if no device selected
			quit_program("Device not selected.");
		}

		context = cl::Context(device);

		// build the program
		if (!build_program(&program, &context, "/Users/isaacyeo/Downloads/Task3/Task3/vecadd.cl"))
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		// create a kernel
		kernel = cl::Kernel(program, "vecadd");

		// create command queue
		queue = cl::CommandQueue(context, device);

		
        
        //prompts user input between 1-100
        int num = 0;
        std::cout << "Enter a input between 1-100"<<std::endl;
        std::cin >>num;
        
        // create buffers
        outputBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * LENGTH);
        
		// set kernel arguments
  
		kernel.setArg(0, num );
        kernel.setArg(1, outputBuffer);
        
        cl::NDRange offset(0);
        cl::NDRange globalSize(LENGTH);
        
//        kernel.setArg(2, resultBuffer);
		// enqueue kernel for execution
		//queue.enqueueTask(kernel);

		 
      
            
      
            if (num >= 1 && num <= 100)
            {
                queue.enqueueNDRangeKernel(kernel, offset, globalSize);

                std::cout << "Kernel enqueued." << std::endl;
                std::cout << "--------------------" << std::endl;

                // enqueue command to read from device to host memory
                queue.enqueueReadBuffer(outputBuffer, CL_TRUE, 0, sizeof(int) * LENGTH,
                        &Arraytest1[0]);
                
                    //display the array as correspondent to the user's input (num)
                    for(unsigned int i=0; i < LENGTH; i++)
                    {
                        std::cout << i << ". " << Arraytest1[i] <<std::endl;
                    }
           
            }
    
            else
            {
            std::cout << "please enter a valid number between 1-100";
            std::cout << "" <<std::endl;
                
            }
           
            }
    
    
      
	// catch any OpenCL function errors
	catch (cl::Error e) {
		// call function to handle errors
		handle_error(e);
	}

#ifdef _WIN32
	// wait for a keypress on Windows OS before exiting
	std::cout << "\npress a key to quit...";
	std::cin.ignore();
#endif
	return 0;
    }


