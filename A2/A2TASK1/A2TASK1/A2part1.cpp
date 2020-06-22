#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>
#include <ctime>
#include <algorithm>

// OpenCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

// to avoid having to use prefixes
//using namespace std;
//using namespace cl;




int main(void)
{


	int err;
	cl_platform_id platformId;
	cl_device_id deviceId;
	cl_context context;

	///seeting up device connection
	clGetPlatformIDs(1, &platformId, NULL);
	clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, 1, &deviceId, NULL);
	context = clCreateContext(0, 1, &deviceId, NULL, NULL, &err);

	cl_command_queue queue = clCreateCommandQueue(context, deviceId, 0, &err);



	//building the program
	cl_program program;
	std::string str;
	std::ifstream fs;
	fs.open("prg.cl");
	getline(fs, str, '\0');
	const char *str_ptr = str.c_str();
	size_t s = str.size();
	program = clCreateProgramWithSource(context, 1, (const char **)&str_ptr, &s, &err);

	err = clBuildProgram(program, 0, NULL, "", NULL, NULL);


	cl_kernel kernel = clCreateKernel(program, "part1", &err);

	//initialise arrays

	unsigned int array1[8];

	srand((unsigned)time(0));

	std::cout << "Array 1:" << std::endl;
	for (int i = 0; i < 8; i++)
	{
		if (i == 7)
		{
			array1[i] = (rand() % (21 - 10) + 10);
			std::cout << array1[i];
		}
		else
		{
			array1[i] = (rand() % (21 - 10) + 10);
			std::cout << array1[i] << ", ";
		}
	}


	int array2[16] = { 2,3,4,5,6,7,8,9,-9,-8,-7,-6,-5,-4,-3,-2 };

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Array 2:" << std::endl;
	for (int i = 0; i < 16; i++)
	{
		if (i == 15)
		{
			std::cout << array2[i];
		}
		else
		{
			std::cout << array2[i] << ", ";
		}
	}
	std::cout << std::endl;
	std::cout << std::endl;

	//make buffers
	cl_mem buffer_array1 = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(array1), array1, &err);
	cl_mem buffer_array2 = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(array2), array2, &err);
	cl_mem buffer_output = clCreateBuffer(context, CL_MEM_READ_ONLY, 32 * sizeof(int), NULL, &err);//32 bytes for results

	size_t workgroups[] = { 1,0,0 };


	err = clSetKernelArg(kernel, 0, sizeof(buffer_array1), &buffer_array1);
	err = clSetKernelArg(kernel, 1, sizeof(buffer_array2), &buffer_array2);
	err = clSetKernelArg(kernel, 2, sizeof(buffer_output), &buffer_output);

	err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL,
		workgroups, NULL, 0, NULL, NULL);


	int outputArray[32] = {};

	///read results
	clEnqueueReadBuffer(queue, buffer_output, true, 0, sizeof(outputArray), outputArray, 0, NULL, NULL);


	//wait for copy
	clFinish(queue);


	//run reference calculations

	int v[8];
	int v1[8];
	int v2[8];

	for (int i = 0; i < 8; i++)
	{
		v[i] = array1[i];
	}

	for (int i = 0; i < 8; i++)
	{
		v1[i] = array2[i];
	}

	for (int i = 0; i < 8; i++)
	{
		v2[i] = array2[8 + i];
	}


	int results[32];
	for (int i = 0; i < 8; i++)
	{
		results[i] = v[i];
	}
	for (int i = 0; i < 8; i++)
	{
		results[8 + i] = v1[i];
	}
	for (int i = 0; i < 8; i++)
	{
		results[16 + i] = v2[i];
	}


	int above_15 = false;

	for (int i = 0; i < 8; i++)
	{
		if (v[i] > 15)
		{
			above_15 = true;
		}
	}

	//if element in v > 15, copy corresponding element from v1
	if (above_15)
	{
		for (int i = 0; i < 8; i++)
		{
			if (v[i] > 15)
			{
				results[24 + i] = v1[i];
			}
			else
			{
				results[24 + i] = v2[i];
			}
		}

	}
	//if element in v < 15, copy corresponding element from v2
	else
	{
		for (int i = 0; i < 4; i++)
		{
			results[24 + i] = v1[i];
		}

		for (int i = 0; i < 4; i++)
		{
			results[24 + 4 + i] = v2[i];
		}
	}

	//output results
	std::cout << "Output Array:" << std::endl;
	for (int k = 0; k < 32; k += 8)
	{
		for (int i = 0; i < 8; i++)
		{
			printf("%3d", results[k + i]);
		}

		std::cout << std::endl;
	}
	std::cout << std::endl;

	getchar();

	return 0;
}
