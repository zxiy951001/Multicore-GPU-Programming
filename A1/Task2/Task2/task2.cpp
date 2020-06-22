#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h> 

// OpenCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include "common.h"



int main(void) 
{
	cl::Platform platform;			// device's platform
	cl::Device device;				// device used
	cl::Context context;			// context for the device
	cl::Program program;			// OpenCL program object
	cl::Kernel kernel;				// a single kernel object
	cl::CommandQueue queue;			// commandqueue for a context and device

	// declare data and memory objects
    std::vector<cl_uchar> Array1({ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M','N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p',
    'q','r','s','t','u','v','w','x','y','z'}); //Array1 of 52 element array of char
	std::vector<unsigned int> Array2(512); //Array2 of 512 element array of ints
    
    std::vector<cl_uchar> ArrayDisplay1(52);
    std::vector<cl_uint> ArrayDisplay2(512);


	cl::Buffer bufferA, bufferB, bufferC;


	
	// initialise values for array1
	for (int i = 0; i < Array1.size(); i++)
	{	
		
		


	}
	std::cout << std::endl;

	
	// initialise values for array2
    
	for (int i = 0; i < Array2.size(); i++)
        Array2[i] = i + 1;
	{
        for (int j = 0; j < Array2.size(); j++)
        {
            
      
        }
        std::cout << std::endl;
	}


	try {
		// select an OpenCL device
		if (!select_one_device(&platform, &device))
		{
			// if no device selected
			quit_program("Device not selected.");
		}

		// create a context from device
		context = cl::Context(device);

		// build the program
		if(!build_program(&program, &context, "/Users/isaacyeo/Downloads/Task2/Task2/task2.cl"))
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		// create a kernel
		kernel = cl::Kernel(program, "task2");

		// create command queue
		queue = cl::CommandQueue(context, device);

		// create buffers
		bufferA = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_uchar) * Array1.size(), &Array1[0]);
		bufferB = cl::Buffer(context, CL_MEM_WRITE_ONLY , sizeof(cl_uchar) * Array1.size());
		bufferC = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_uint) * Array2.size());
        
        //Enqueue two OpenCL commands
        //copy content from the first buffer(bufferA) into the secondbuffer(bufferB)
        queue.enqueueCopyBuffer(bufferA, bufferB, 0,0, sizeof(cl_uchar) * Array1.size());
        //writing contents from the vector of 512 integers into the thirdbuffer(bufferC)
        queue.enqueueWriteBuffer(bufferC, CL_TRUE, 0, sizeof(cl_uint) * Array2.size(), &Array2[0]);

		// set kernel arguments
        float a = 12.34;
		kernel.setArg(0, a);
		kernel.setArg(1, bufferB); //second buffer
		kernel.setArg(2, bufferC); //third buffer
        
//		 enqueue kernel for execution
        queue.enqueueTask(kernel);
			   
		std::cout << "Kernel enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;
        
        queue.enqueueReadBuffer(bufferB, CL_TRUE, 0, sizeof(cl_uchar) * Array1.size(), &Array1[0]);
        queue.enqueueReadBuffer(bufferC, CL_TRUE, 0, sizeof(cl_uint) * Array2.size(), &ArrayDisplay2[0]);

        
        //display contents of second and third buffer
        
        std::cout << "\nContents of second Buffer: " << std::endl;
        int y = 0;
        for (unsigned int i = 0; i < Array1.size() /13 ; i++)
      {
          for(unsigned int j = 0; j< Array1.size() /4; j++)
          {
              std::cout << (Array1[y] ) << " ";
              
             
            
              y++;
          }
      }
         std::cout << "\nContents of third Buffer : " << std::endl;
        int x = 0;
        for (unsigned int i = 0; i < Array2.size()   ; i++)
        {
                std::cout << (Array2[x]) << " " ;
                
                x++;
            }
        std::cout << " " << std::endl;;
       
        



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
