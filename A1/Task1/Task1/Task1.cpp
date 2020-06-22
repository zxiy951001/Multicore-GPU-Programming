#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

	// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>

	// OpenCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include "error.h"
	// to avoid having to use prefixes
	//using namespace std;
	//using namespace cl;




	int main(void)
	{

		std::vector<cl::Platform> platforms;	// available platforms
		std::vector<cl::Device> devices;		// devices available to a platform
		cl::Device device;
		cl::Context context;
		cl::Program program;
		cl::CommandQueue queue;					// commandqueue for a context and device
		std::string outputString;				// string for output
		unsigned int i, j, k;					// counters

		int user_input, user_input_p, user_input_d;
		bool isRunning = true;

		while (isRunning)
		{
			// main menu
			std::cout << "Name: Yeo Zheng Xu Isaac" << std::endl;
			std::cout << "Student ID: 6342425" << std::endl;
			std::cout << "---------------------------------------" << std::endl;
			std::cout << "Please select CPU or GPU device" << std::endl;
			std::cout << "1) CPU device" << std::endl;
			std::cout << "2) GPU device" << std::endl;
			std::cout << "3) Exit" << std::endl;
			std::cout << std::endl;
			std::cout << "Please enter option: ";
			std::cin >> user_input;

			switch (user_input)
			{
			case 1:
				// CPU option
				std::cout << std::endl;
				cl::Platform::get(&platforms);
				std::cout << "Number of OpenCL platforms: " << platforms.size() << std::endl;
				std::cout << std::endl;

				// for each platform
				for (i = 0; i < platforms.size(); i++)
				{
					std::cout << "--------------------" << std::endl;
					// output platform index
					std::cout << "  Platform " << i << ":" << std::endl;

					// get and output platform name
					platforms[i].getInfo(CL_PLATFORM_NAME, &outputString);
					std::cout << "\tName: " << outputString << std::endl;

					// get and output platform vendor name
					outputString = platforms[i].getInfo<CL_PLATFORM_VENDOR>();
					std::cout << "\tVendor: " << outputString << std::endl;

					// get and output OpenCL version supported by the platform
					outputString = platforms[i].getInfo<CL_PLATFORM_VERSION>();
					std::cout << "\tVersion: " << outputString << std::endl;

					// get all devices available to the platform
					platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices);

					//std::cout << "\nNumber of CPU devices available to platform " << i << ": " << devices.size() << std::endl;
					//std::cout << "--------------------" << std::endl;

					// for each device
					for (j = 0; j < devices.size(); j++)
					{
						// get and output device type
						cl_device_type type;
						devices[j].getInfo(CL_DEVICE_TYPE, &type);
						if (type == CL_DEVICE_TYPE_CPU)
						{
							int counter = 1;
							std::cout << "\nNumber of CPU devices available to platform " << i << ": " << counter << std::endl;
							std::cout << "--------------------" << std::endl;
							// output device index
							std::cout << "  Device " << j << std::endl;

							// get and output device name
							outputString = devices[j].getInfo<CL_DEVICE_NAME>();
							std::cout << "\tName: " << outputString << std::endl;
							std::cout << "\tType: " << "CPU" << std::endl;

							// get and output device vendor
							outputString = devices[j].getInfo<CL_DEVICE_VENDOR>();
							std::cout << "\tVendor: " << outputString << std::endl;

							// get and output OpenCL version supported by the device
							outputString = devices[j].getInfo<CL_DEVICE_VERSION>();
							std::cout << "\tVersion: " << outputString << std::endl;

							//get and output max compute units
							int computeUnits = devices[j].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
							std::cout << "\tMax Compute Units: " << computeUnits << std::endl;

							//get and output max workgroup size
							int workitemdimensions = devices[j].getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>();
							std::cout << "\tMax Work Item Dimensions: " << workitemdimensions << std::endl;
                            
                            int workGroup = devices[j].getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
                            std::cout << "\tMax Work Groups size: " << workGroup << std::endl;

							//get and output max work item sizes
							std::vector<size_t> workItem = devices[j].getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
							int convert = static_cast<int>(workItem.size());
							std::cout << "\tMax Work Items size: ";
							for (k = 0; k < workItem.size(); k++)
							{
								//std::cout << workGroup << " x ";
								if (k == workItem.size() - 1)
								{
									std::cout << workGroup;
								}
								else
								{
									std::cout << workGroup << " x ";
								}
							}
							std::cout << std::endl;

							//get and output local memory size
							int memSize = devices[j].getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();
							std::cout << "\tLocal Memory Size: " << memSize << std::endl;
							counter++;
                            int globalmemSize = devices[j].getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
                            std::cout << "\tGlobal Memory Size: " << globalmemSize << std::endl;
                            counter++;
						}
						else
						{
							continue;
						}

					}
					std::cout << std::endl;
				}

				std::cout << std::endl;
				std::cout << "Please select a platform: ";
				std::cin >> user_input_p;

				switch (user_input_p)
				{
				case 0:
					if (devices.size() > 0)
					{
						// get all devices available to the platform
						platforms[user_input_p].getDevices(CL_DEVICE_TYPE_ALL, &devices);

						for (j = 0; j < devices.size(); j++)
						{
							// get and output device type
							cl_device_type type;
							devices[j].getInfo(CL_DEVICE_TYPE, &type);
							if (type == CL_DEVICE_TYPE_CPU)
							{
								std::cout << "--------------------" << std::endl;
								// output platform index
								std::cout << "  Platform " << user_input_p << ":" << std::endl;

								// get and output platform name
								platforms[user_input_p].getInfo(CL_PLATFORM_NAME, &outputString);
								std::cout << "\tName: " << outputString << std::endl;

								// get and output platform vendor name
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VENDOR>();
								std::cout << "\tVendor: " << outputString << std::endl;

								// get and output OpenCL version supported by the platform
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VERSION>();
								std::cout << "\tVersion: " << outputString << std::endl;

								int counter = 1;
								std::cout << "\nNumber of CPU devices available to platform " << user_input_p << ": " << counter << std::endl;
								std::cout << "--------------------" << std::endl;
								// output device index
								std::cout << "  Device " << j << std::endl;

								// get and output device name
								outputString = devices[j].getInfo<CL_DEVICE_NAME>();
								std::cout << "\tName: " << outputString << std::endl;
								std::cout << std::endl;

								std::cout << "Please select a CPU device: ";
								std::cin >> user_input_d;


								std::cout << "--------------------" << std::endl;
								std::cout << "Creating a context for device " << user_input_d << "..." << std::endl;

								// create a context for all available devices on that platform
								context = cl::Context(devices[j]);


								// check devices in the context
								std::cout << "\nDevices in the context:" << std::endl;

								// get devices in the context
								std::vector<cl::Device> contextDevices = context.getInfo<CL_CONTEXT_DEVICES>();

								// create a command queue for all available devices on that platform
								queue = cl::CommandQueue(context, devices[j]);

								// output names of devices in the context
								for (k = 0; k < contextDevices.size(); k++)
								{
									// get and output device type
									cl_device_type type;
									contextDevices[k].getInfo(CL_DEVICE_TYPE, &type);
									if (type == CL_DEVICE_TYPE_CPU)
									{
										int counter = 1;
										outputString = contextDevices[k].getInfo<CL_DEVICE_NAME>();
										std::cout << "  Device " << counter << ": " << outputString << std::endl;
										// get and output platform vendor name
										//outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VENDOR>();
										//std::cout << "\tVendor: " << outputString << std::endl;
										std::cout << std::endl;
										std::cout << "Context created" << std::endl;
										std::cout << "Command queue created" << std::endl;
										std::cout << std::endl;
									


										// open input file stream to .cl file
										std::ifstream programFile("task1.cl");

										// check whether file was opened
										if (!programFile.is_open())
										{
											"File not found.";
										}

										// create program string and load contents from the file
										std::string programString(std::istreambuf_iterator<char>(programFile), (std::istreambuf_iterator<char>()));

										// create program source from one input string
										cl::Program::Sources source(1, std::make_pair(programString.c_str(), programString.length() + 1));
										// create program from source
										cl::Program program(context, source);

										// build the program for the devices in the context
										program.build(contextDevices);

										std::cout << "Program build: Successful" << std::endl;
										std::cout << "--------------------" << std::endl;

										if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_SUCCESS)
										{
											std::cout << "Build success!" << std::endl;
											std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;

											//all kernels
											std::vector<cl::Kernel> allKernel;

											//create all kernels in program
											program.createKernels(&allKernel);
											std::cout << "--------------------" << std::endl;
											std::cout << "Total number of kernels: " << allKernel.size() << std::endl;

											for (i = 0; i < allKernel.size(); i++)
											{
												outputString = allKernel[i].getInfo<CL_KERNEL_FUNCTION_NAME>();
												std::cout << "Kernel " << i << ": " << outputString << std::endl;
											}

											std::cout << allKernel.size() << " Kernels added" << std::endl;
											std::cout << "--------------------" << std::endl;
											std::cout << std::endl;
										}

										if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_ERROR)
										{
											std::cout << "Build error!" << std::endl;
											std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;
											std::cout << "--------------------" << std::endl;
											std::cout << std::endl;
										}
										counter++;
									}
									else
									{
										continue;
									}


								}
								counter++;

							}
							else
							{
								continue;
							}
						}

						std::cout << "Returning to main menu...\n" << std::endl;
						// break from loop, since required number of devices found
						break;
					}
					else
					{
						std::cout << "0 devices found" << std::endl;
						std::cout << "Returning to main menu...\n" << std::endl;
						break;
					}
					break;

				case 1:
					if (devices.size() > 0)
					{
						// get all devices available to the platform
						platforms[user_input_p].getDevices(CL_DEVICE_TYPE_ALL, &devices);

						for (j = 0; j < devices.size(); j++)
						{
							// get and output device type
							cl_device_type type;
							devices[j].getInfo(CL_DEVICE_TYPE, &type);
							if (type == CL_DEVICE_TYPE_CPU)
							{
								std::cout << "--------------------" << std::endl;
								// output platform index
								std::cout << "  Platform " << user_input_p << ":" << std::endl;

								// get and output platform name
								platforms[user_input_p].getInfo(CL_PLATFORM_NAME, &outputString);
								std::cout << "\tName: " << outputString << std::endl;

								// get and output platform vendor name
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VENDOR>();
								std::cout << "\tVendor: " << outputString << std::endl;

								// get and output OpenCL version supported by the platform
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VERSION>();
								std::cout << "\tVersion: " << outputString << std::endl;


								int counter = 1;
								std::cout << "\nNumber of CPU devices available to platform " << user_input_p << ": " << counter << std::endl;
								std::cout << "--------------------" << std::endl;
								// output device index
								std::cout << "  Device " << j << std::endl;

								// get and output device name
								outputString = devices[j].getInfo<CL_DEVICE_NAME>();
								std::cout << "\tName: " << outputString << std::endl;
								std::cout << std::endl;
								counter++;

								std::cout << "Please select a CPU device: ";
								std::cin >> user_input_d;


								std::cout << "--------------------" << std::endl;
								std::cout << "Creating a context for device " << user_input_d << "..." << std::endl;

								// create a context for all available devices on that platform
								context = cl::Context(devices[j]);

								// check devices in the context
								std::cout << "\nDevices in the context:" << std::endl;

								// get devices in the context
								std::vector<cl::Device> contextDevices = context.getInfo<CL_CONTEXT_DEVICES>();

								// create a command queue for all available devices on that platform
								queue = cl::CommandQueue(context, devices[j]);

								// output names of devices in the context
								for (k = 0; k < contextDevices.size(); k++)
								{
									// get and output device type
									cl_device_type type;
									contextDevices[k].getInfo(CL_DEVICE_TYPE, &type);
									if (type == CL_DEVICE_TYPE_CPU)
									{
										int counter = 1;
										outputString = contextDevices[k].getInfo<CL_DEVICE_NAME>();
										std::cout << "  Device " << k << ": " << outputString << std::endl;
										// get and output platform vendor name
										//outputString = platforms[k].getInfo<CL_PLATFORM_VENDOR>();
										//std::cout << "\tVendor: " << outputString << std::endl;
										std::cout << std::endl;
										std::cout << "Context created" << std::endl;
										std::cout << "Command queue created" << std::endl;
										std::cout << std::endl;


										// open input file stream to .cl file
										std::ifstream programFile("task1.cl");

										// check whether file was opened
										if (!programFile.is_open())
										{
											"File not found.";
										}

										// create program string and load contents from the file
										std::string programString(std::istreambuf_iterator<char>(programFile), (std::istreambuf_iterator<char>()));

										// create program source from one input string
										cl::Program::Sources source(1, std::make_pair(programString.c_str(), programString.length() + 1));
										// create program from source
										cl::Program program(context, source);

										// build the program for the devices in the context
										program.build(contextDevices);

										std::cout << "Program build: Successful" << std::endl;
										std::cout << "--------------------" << std::endl;

										if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_SUCCESS)
										{
											std::cout << "Build success!" << std::endl;
											std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;

											//all kernels
											std::vector<cl::Kernel> allKernel;

											//create all kernels in program
											program.createKernels(&allKernel);
											std::cout << "--------------------" << std::endl;
											std::cout << "Total number of kernels: " << allKernel.size() << std::endl;

											for (i = 0; i < allKernel.size(); i++)
											{
												outputString = allKernel[i].getInfo<CL_KERNEL_FUNCTION_NAME>();
												std::cout << "Kernel " << i << ": " << outputString << std::endl;
											}

											std::cout << allKernel.size() << " Kernels added" << std::endl;
											std::cout << "--------------------" << std::endl;
											std::cout << std::endl;
										}

										if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_ERROR)
										{
											std::cout << "Build error!" << std::endl;
											std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;
											std::cout << "--------------------" << std::endl;
											std::cout << std::endl;
										}
										counter++;
									}
									else
									{
										continue;
									}

								}
								counter++;
							}
							else
							{
								std::cout << "No CPU devices found" << std::endl;
							}
						}
						std::cout << "Returning to main menu...\n" << std::endl;
						// break from loop, since required number of devices found
						break;
					}
					else
					{
						std::cout << "No devices found" << std::endl;
						std::cout << "Returning to main menu...\n" << std::endl;
						break;
					}
					break;

				case 2:
					if (devices.size() > 0)
					{
						// get all devices available to the platform
						platforms[user_input_p].getDevices(CL_DEVICE_TYPE_ALL, &devices);

						for (j = 0; j < devices.size(); j++)
						{
							// get and output device type
							cl_device_type type;
							devices[j].getInfo(CL_DEVICE_TYPE, &type);
							if (type == CL_DEVICE_TYPE_CPU)
							{
								std::cout << "--------------------" << std::endl;
								// output platform index
								std::cout << "  Platform " << user_input_p << ":" << std::endl;

								// get and output platform name
								platforms[user_input_p].getInfo(CL_PLATFORM_NAME, &outputString);
								std::cout << "\tName: " << outputString << std::endl;

								// get and output platform vendor name
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VENDOR>();
								std::cout << "\tVendor: " << outputString << std::endl;

								// get and output OpenCL version supported by the platform
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VERSION>();
								std::cout << "\tVersion: " << outputString << std::endl;

								int counter = 1;
								std::cout << "\nNumber of CPU devices available to platform " << user_input_p << ": " << counter << std::endl;
								std::cout << "--------------------" << std::endl;
								// output device index
								std::cout << "  Device " << j << std::endl;

								// get and output device name
								outputString = devices[j].getInfo<CL_DEVICE_NAME>();
								std::cout << "\tName: " << outputString << std::endl;
								std::cout << std::endl;


								std::cout << "Please select a CPU device: ";
								std::cin >> user_input_d;


								std::cout << "--------------------" << std::endl;
								std::cout << "Creating a context for device " << user_input_d << "..." << std::endl;

								// create a context for all available devices on that platform
								context = cl::Context(CL_DEVICE_TYPE_ALL);

								// check devices in the context
								std::cout << "\nDevices in the context:" << std::endl;

								// get devices in the context
								std::vector<cl::Device> contextDevices = context.getInfo<CL_CONTEXT_DEVICES>();

								// create a command queue for all available devices on that platform
								queue = cl::CommandQueue(context, devices[j]);


								// output names of devices in the context
								for (k = 0; k < contextDevices.size(); k++)
								{
									int counter = 1;
									outputString = contextDevices[k].getInfo<CL_DEVICE_NAME>();
									std::cout << "  Device " << counter << ": " << outputString << std::endl;

									std::cout << std::endl;
									std::cout << "Context created" << std::endl;
									std::cout << "Command queue created" << std::endl;
									std::cout << std::endl;

									// open input file stream to .cl file
									std::ifstream programFile("task1.cl");

									// check whether file was opened
									if (!programFile.is_open())
									{
										"File not found.";
									}

									// create program string and load contents from the file
									std::string programString(std::istreambuf_iterator<char>(programFile), (std::istreambuf_iterator<char>()));

									// create program source from one input string
									cl::Program::Sources source(1, std::make_pair(programString.c_str(), programString.length() + 1));
									// create program from source
									cl::Program program(context, source);

									// build the program for the devices in the context
									program.build(contextDevices);

									std::cout << "Program build: Successful" << std::endl;
									std::cout << "--------------------" << std::endl;

									if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_SUCCESS)
									{
										std::cout << "Build success!" << std::endl;
										std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;

										//all kernels
										std::vector<cl::Kernel> allKernel;

										//create all kernels in program
										program.createKernels(&allKernel);
										std::cout << "--------------------" << std::endl;
										std::cout << "Total number of kernels: " << allKernel.size() << std::endl;

										for (i = 0; i < allKernel.size(); i++)
										{
											outputString = allKernel[i].getInfo<CL_KERNEL_FUNCTION_NAME>();
											std::cout << "Kernel " << i << ": " << outputString << std::endl;
										}

										std::cout << allKernel.size() << " Kernels added" << std::endl;
										std::cout << "--------------------" << std::endl;
										std::cout << std::endl;
									}

									if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_ERROR)
									{
										std::cout << "Build error!" << std::endl;
										std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;
										std::cout << "--------------------" << std::endl;
										std::cout << std::endl;
									}
									counter++;
								}
								//std::cout << "Returning to main menu...\n" << std::endl;
								counter++;
								// break from loop, since required number of devices found
								break;

							}
							else
							{
								std::cout << "No CPU devices found" << std::endl;

							}


						}
						std::cout << "Returning to main menu...\n" << std::endl;
						// break from loop, since required number of devices found
						break;
					}
					else
					{
						std::cout << "0 devices found" << std::endl;
						std::cout << "Returning to main menu...\n" << std::endl;
						break;
					}
					break;

				case 3:
					if (devices.size() > 0)
					{

						// get all devices available to the platform
						platforms[user_input_p].getDevices(CL_DEVICE_TYPE_ALL, &devices);

						for (j = 0; j < devices.size(); j++)
						{
							// get and output device type
							cl_device_type type;
							devices[j].getInfo(CL_DEVICE_TYPE, &type);
							if (type == CL_DEVICE_TYPE_CPU)
							{
								std::cout << "--------------------" << std::endl;
								// output platform index
								std::cout << "  Platform " << user_input_p << ":" << std::endl;

								// get and output platform name
								platforms[user_input_p].getInfo(CL_PLATFORM_NAME, &outputString);
								std::cout << "\tName: " << outputString << std::endl;

								// get and output platform vendor name
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VENDOR>();
								std::cout << "\tVendor: " << outputString << std::endl;

								// get and output OpenCL version supported by the platform
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VERSION>();
								std::cout << "\tVersion: " << outputString << std::endl;

								int counter = 1;
								std::cout << "\nNumber of CPU devices available to platform " << user_input_p << ": " << counter << std::endl;
								std::cout << "--------------------" << std::endl;
								// output device index
								std::cout << "  Device " << j << std::endl;

								// get and output device name
								outputString = devices[j].getInfo<CL_DEVICE_NAME>();
								std::cout << "\tName: " << outputString << std::endl;
								std::cout << std::endl;

								std::cout << "Please select a CPU device: ";
								std::cin >> user_input_d;


								std::cout << "--------------------" << std::endl;
								std::cout << "Creating a context for device " << user_input_d << "..." << std::endl;

								// create a context for all available devices on that platform
								context = cl::Context(devices[j]);

								// check devices in the context
								std::cout << "\nDevices in the context:" << std::endl;

								// get devices in the context
								std::vector<cl::Device> contextDevices = context.getInfo<CL_CONTEXT_DEVICES>();

								// create a command queue for all available devices on that platform
								queue = cl::CommandQueue(context, devices[j]);


								// output names of devices in the context
								for (k = 0; k < contextDevices.size(); k++)
								{
									// get and output device type
									cl_device_type type;
									contextDevices[k].getInfo(CL_DEVICE_TYPE, &type);
									if (type == CL_DEVICE_TYPE_CPU)
									{
										int counter = 1;
										outputString = contextDevices[k].getInfo<CL_DEVICE_NAME>();
										std::cout << "  Device " << k << ": " << outputString << std::endl;
										// get and output platform vendor name
										//outputString = contextDevices[k].getInfo<CL_DEVICE_VENDOR>();
										//std::cout << "\tVendor: " << outputString << std::endl;
										std::cout << std::endl;
										std::cout << "Context created" << std::endl;
										std::cout << "Command queue created" << std::endl;
										std::cout << std::endl;

										// open input file stream to .cl file
										std::ifstream programFile("task1.cl");

										// check whether file was opened
										if (!programFile.is_open())
										{
											"File not found.";
										}

										// create program string and load contents from the file
										std::string programString(std::istreambuf_iterator<char>(programFile), (std::istreambuf_iterator<char>()));

										// create program source from one input string
										cl::Program::Sources source(1, std::make_pair(programString.c_str(), programString.length() + 1));
										// create program from source
										cl::Program program(context, source);

										// build the program for the devices in the context
										program.build(contextDevices);

										std::cout << "Program build: Successful" << std::endl;
										std::cout << "--------------------" << std::endl;

										if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_SUCCESS)
										{
											std::cout << "Build success!" << std::endl;
											std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;

											//all kernels
											std::vector<cl::Kernel> allKernel;

											//create all kernels in program
											program.createKernels(&allKernel);
											std::cout << "--------------------" << std::endl;
											std::cout << "Total number of kernels: " << allKernel.size() << std::endl;
										

											for (i = 0; i < allKernel.size(); i++)
											{
												outputString = allKernel[i].getInfo<CL_KERNEL_FUNCTION_NAME>();
												std::cout << "Kernel " << i << ": " << outputString << std::endl;
											}
											std::cout << allKernel.size() << " Kernels added" << std::endl;
											std::cout << "--------------------" << std::endl;
											std::cout << std::endl;
										}

										if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_ERROR)
										{
											std::cout << "Build error!" << std::endl;
											std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;
											std::cout << "--------------------" << std::endl;
											std::cout << std::endl;
										}
										counter++;
									}
									else
									{
										continue;
									}
								}
								counter++;

							}
							else
							{
								std::cout << "No CPU devices found" << std::endl;
							}
						}
						std::cout << "Returning to main menu...\n" << std::endl;
						// break from loop, since required number of devices found
						break;
					}
					else
					{
						std::cout << "0 devices found" << std::endl;
						std::cout << "Returning to main menu...\n" << std::endl;
						break;
					}
					break;

				default:
					//invalid option
					std::cout << std::endl;
					std::cout << "Invalid option. Please select value 0 - " << platforms.size() - 1 << std::endl;
					std::cout << "Returning to main menu...\n" << std::endl;
				
				}
				break;
			
			case 2:

				// GPU option
				std::cout << std::endl;
				cl::Platform::get(&platforms);
				std::cout << "Number of OpenCL platforms: " << platforms.size() << std::endl;

				// for each platform
				for (i = 0; i < platforms.size(); i++)
				{
					std::cout << "--------------------" << std::endl;
					// output platform index
					std::cout << "  Platform " << i << ":" << std::endl;

					// get and output platform name
					platforms[i].getInfo(CL_PLATFORM_NAME, &outputString);
					std::cout << "\tName: " << outputString << std::endl;

					// get and output platform vendor name
					outputString = platforms[i].getInfo<CL_PLATFORM_VENDOR>();
					std::cout << "\tVendor: " << outputString << std::endl;

					// get and output OpenCL version supported by the platform
					outputString = platforms[i].getInfo<CL_PLATFORM_VERSION>();
					std::cout << "\tVersion: " << outputString << std::endl;

					// get all devices available to the platform
					platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices);

					//std::cout << "\nNumber of CPU devices available to platform " << i << ": " << devices.size() << std::endl;
					//std::cout << "--------------------" << std::endl;

					// for each device
					for (j = 0; j < devices.size(); j++)
					{
						// get and output device type
						cl_device_type type;
						devices[j].getInfo(CL_DEVICE_TYPE, &type);
						if (type == CL_DEVICE_TYPE_GPU)
						{
							int counter = 1;
							std::cout << "\nNumber of CPU devices available to platform " << i << ": " << counter << std::endl;
							std::cout << "--------------------" << std::endl;
							// output device index
							std::cout << "  Device " << j << std::endl;

							// get and output device name
							outputString = devices[j].getInfo<CL_DEVICE_NAME>();
							std::cout << "\tName: " << outputString << std::endl;
							std::cout << "\tType: " << "GPU" << std::endl;

							// get and output device vendor
							outputString = devices[j].getInfo<CL_DEVICE_VENDOR>();
							std::cout << "\tVendor: " << outputString << std::endl;

							// get and output OpenCL version supported by the device
							outputString = devices[j].getInfo<CL_DEVICE_VERSION>();
							std::cout << "\tVersion: " << outputString << std::endl;

							int computeUnits = devices[j].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
							std::cout << "\tMax Compute Units: " << computeUnits << std::endl;
                            
                            int workitemdimensions = devices[j].getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>();
                            std::cout << "\tMax Work Item Dimensions: " << workitemdimensions << std::endl;

							int workGroup = devices[j].getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
							std::cout << "\tMax Work Groups Size: " << workGroup << std::endl;


							std::vector<size_t> workItem = devices[j].getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
							int convert = static_cast<int>(workItem.size());
							std::cout << "\tMax Work Items Size: ";
							for (k = 0; k < workItem.size(); k++)
							{
								//std::cout << workGroup << " x ";
								if (k == workItem.size() - 1)
								{
									std::cout << workGroup;
								}
								else
								{
									std::cout << workGroup << " x ";
								}
							}
							std::cout << std::endl;

							int memSize = devices[j].getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();
                            
							std::cout << "\tLocal Memory Size: " << memSize << std::endl;
							counter++;
                            int globalmemSize = devices[j].getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
                            std::cout << "\tGlobal Memory Size: " << globalmemSize << std::endl;
						}
						else
						{
							continue;
						}

					}
					std::cout << std::endl;
				}

				std::cout << std::endl;
				std::cout << "Please select a platform: ";
				std::cin >> user_input_p;

				switch (user_input_p)
				{
				case 0:
					if (devices.size() > 0)
					{
						// get all devices available to the platform
						platforms[user_input_p].getDevices(CL_DEVICE_TYPE_ALL, &devices);

						for (j = 0; j < devices.size(); j++)
						{
							// get and output device type
							cl_device_type type;
							devices[j].getInfo(CL_DEVICE_TYPE, &type);
							if (type == CL_DEVICE_TYPE_GPU)
							{
								std::cout << "--------------------" << std::endl;
								// output platform index
								std::cout << "  Platform " << user_input_p << ":" << std::endl;

								// get and output platform name
								platforms[user_input_p].getInfo(CL_PLATFORM_NAME, &outputString);
								std::cout << "\tName: " << outputString << std::endl;

								// get and output platform vendor name
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VENDOR>();
								std::cout << "\tVendor: " << outputString << std::endl;

								// get and output OpenCL version supported by the platform
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VERSION>();
								std::cout << "\tVersion: " << outputString << std::endl;

								int counter = 1;
								std::cout << "\nNumber of CPU devices available to platform " << user_input_p << ": " << counter << std::endl;
								std::cout << "--------------------" << std::endl;
								// output device index
								std::cout << "  Device " << j << std::endl;

								// get and output device name
								outputString = devices[j].getInfo<CL_DEVICE_NAME>();
								std::cout << "\tName: " << outputString << std::endl;
								std::cout << std::endl;
								counter++;

								std::cout << "Please select a GPU device: ";
								std::cin >> user_input_d;


								std::cout << "--------------------" << std::endl;
								std::cout << "Creating a context for device " << user_input_d << "..." << std::endl;

								// create a context for all available devices on that platform
								context = cl::Context(devices[j]);


								// check devices in the context
								std::cout << "\nDevices in the context:" << std::endl;

								// get devices in the context
								std::vector<cl::Device> contextDevices = context.getInfo<CL_CONTEXT_DEVICES>();

								// create a command queue for all available devices on that platform
								queue = cl::CommandQueue(context, devices[j]);


								// output names of devices in the context
								for (k = 0; k < contextDevices.size(); k++)
								{
									// get and output device type
									cl_device_type type;
									contextDevices[k].getInfo(CL_DEVICE_TYPE, &type);
									if (type == CL_DEVICE_TYPE_GPU)
									{
										outputString = contextDevices[k].getInfo<CL_DEVICE_NAME>();
										std::cout << "  Device " << user_input_d << ": " << outputString << std::endl;
										std::cout << std::endl;
										std::cout << "Context created" << std::endl;
										std::cout << "Command queue created" << std::endl;
										std::cout << std::endl;


										// open input file stream to .cl file
										std::ifstream programFile("task1.cl");

										// check whether file was opened
										if (!programFile.is_open())
										{
											"File not found.";
										}

										// create program string and load contents from the file
										std::string programString(std::istreambuf_iterator<char>(programFile), (std::istreambuf_iterator<char>()));

										// create program source from one input string
										cl::Program::Sources source(1, std::make_pair(programString.c_str(), programString.length() + 1));
										// create program from source
										cl::Program program(context, source);

										// build the program for the devices in the context
										program.build(contextDevices);

										std::cout << "Program build: Successful" << std::endl;
										std::cout << "--------------------" << std::endl;

										if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_SUCCESS)
										{
											std::cout << "Build success!" << std::endl;
											std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;

											//all kernels
											std::vector<cl::Kernel> allKernel;

											//create all kernels in program
											program.createKernels(&allKernel);
											std::cout << "--------------------" << std::endl;
											std::cout << "Total number of kernels: " << allKernel.size() << std::endl;


											for (i = 0; i < allKernel.size(); i++)
											{
												outputString = allKernel[i].getInfo<CL_KERNEL_FUNCTION_NAME>();
												std::cout << "Kernel " << i << ": " << outputString << std::endl;
											}
											std::cout << allKernel.size() << " Kernels added" << std::endl;
											std::cout << "--------------------" << std::endl;
											std::cout << std::endl;
										}

										if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_ERROR)
										{
											std::cout << "Build error!" << std::endl;
											std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;
											std::cout << "--------------------" << std::endl;
											std::cout << std::endl;
										}
									}
									else
									{
										continue;
									}
								}
							}
							else
							{
								continue;
							}
						}

						std::cout << "Returning to main menu...\n" << std::endl;
						// break from loop, since required number of devices found
						break;
					}
					else
					{
						std::cout << "0 devices found" << std::endl;
						std::cout << "Returning to main menu...\n" << std::endl;
						break;
					}
					break;

				case 1:
					if (devices.size() > 0)
					{
						// get all devices available to the platform
						platforms[user_input_p].getDevices(CL_DEVICE_TYPE_ALL, &devices);

						for (j = 0; j < devices.size(); j++)
						{
							// get and output device type
							cl_device_type type;
							devices[j].getInfo(CL_DEVICE_TYPE, &type);
							if (type == CL_DEVICE_TYPE_GPU)
							{
								std::cout << "--------------------" << std::endl;
								// output platform index
								std::cout << "  Platform " << user_input_p << ":" << std::endl;

								// get and output platform name
								platforms[user_input_p].getInfo(CL_PLATFORM_NAME, &outputString);
								std::cout << "\tName: " << outputString << std::endl;

								// get and output platform vendor name
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VENDOR>();
								std::cout << "\tVendor: " << outputString << std::endl;

								// get and output OpenCL version supported by the platform
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VERSION>();
								std::cout << "\tVersion: " << outputString << std::endl;

								int counter = 1;
								std::cout << "\nNumber of CPU devices available to platform " << user_input_p << ": " << counter << std::endl;
								std::cout << "--------------------" << std::endl;
								// output device index
								std::cout << "  Device " << j << std::endl;

								// get and output device name
								outputString = devices[j].getInfo<CL_DEVICE_NAME>();
								std::cout << "\tName: " << outputString << std::endl;
								std::cout << std::endl;
								counter++;

								std::cout << "Please select a CPU device: ";
								std::cin >> user_input_d;


								std::cout << "--------------------" << std::endl;
								std::cout << "Creating a context for device " << user_input_d << "..." << std::endl;

								// create a context for all available devices on that platform
								context = cl::Context(devices[j]);


								// check devices in the context
								std::cout << "\nDevices in the context:" << std::endl;

								// get devices in the context
								std::vector<cl::Device> contextDevices = context.getInfo<CL_CONTEXT_DEVICES>();

								// create a command queue for all available devices on that platform
								queue = cl::CommandQueue(context, devices[j]);


								// output names of devices in the context
								for (k = 0; k < contextDevices.size(); k++)
								{
									// get and output device type
									cl_device_type type;
									contextDevices[k].getInfo(CL_DEVICE_TYPE, &type);
									if (type == CL_DEVICE_TYPE_GPU)
									{
										int counter = 1;
										outputString = contextDevices[k].getInfo<CL_DEVICE_NAME>();
										std::cout << "  Device " << counter << ": " << outputString << std::endl;
										std::cout << std::endl;
										std::cout << "Context created" << std::endl;
										std::cout << "Command queue created" << std::endl;
										std::cout << std::endl;


										// open input file stream to .cl file
										std::ifstream programFile("task1.cl");

										// check whether file was opened
										if (!programFile.is_open())
										{
											"File not found.";
										}

										// create program string and load contents from the file
										std::string programString(std::istreambuf_iterator<char>(programFile), (std::istreambuf_iterator<char>()));

										// create program source from one input string
										cl::Program::Sources source(1, std::make_pair(programString.c_str(), programString.length() + 1));
										// create program from source
										cl::Program program(context, source);

										// build the program for the devices in the context
										program.build(contextDevices);

										std::cout << "Program build: Successful" << std::endl;
										std::cout << "--------------------" << std::endl;

										if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_SUCCESS)
										{
											std::cout << "Build success!" << std::endl;
											std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;

											//all kernels
											std::vector<cl::Kernel> allKernel;

											//create all kernels in program
											program.createKernels(&allKernel);
											std::cout << "--------------------" << std::endl;
											std::cout << "Total number of kernels: " << allKernel.size() << std::endl;


											for (i = 0; i < allKernel.size(); i++)
											{
												outputString = allKernel[i].getInfo<CL_KERNEL_FUNCTION_NAME>();
												std::cout << "Kernel " << i << ": " << outputString << std::endl;
											}
											std::cout << allKernel.size() << " Kernels added" << std::endl;
											std::cout << "--------------------" << std::endl;
											std::cout << std::endl;
										}

										if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_ERROR)
										{
											std::cout << "Build error!" << std::endl;
											std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;
											std::cout << "--------------------" << std::endl;
											std::cout << std::endl;
										}
										counter++;
									}
									else
									{
										continue;
									}
								}
							}
							else
							{
								std::cout << "No GPU devices found" << std::endl;
							}
						}

						std::cout << "Returning to main menu...\n" << std::endl;
						// break from loop, since required number of devices found
						break;
					}
					else
					{
						std::cout << "0 devices found" << std::endl;
						std::cout << "Returning to main menu...\n" << std::endl;
						break;
					}
					break;

				case 2:
					if (devices.size() > 0)
					{
						// get all devices available to the platform
						platforms[user_input_p].getDevices(CL_DEVICE_TYPE_ALL, &devices);

						for (j = 0; j < devices.size(); j++)
						{
							// get and output device type
							cl_device_type type;
							devices[j].getInfo(CL_DEVICE_TYPE, &type);
							if (type == CL_DEVICE_TYPE_GPU)
							{
								std::cout << "--------------------" << std::endl;
								// output platform index
								std::cout << "  Platform " << user_input_p << ":" << std::endl;

								// get and output platform name
								platforms[user_input_p].getInfo(CL_PLATFORM_NAME, &outputString);
								std::cout << "\tName: " << outputString << std::endl;

								// get and output platform vendor name
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VENDOR>();
								std::cout << "\tVendor: " << outputString << std::endl;

								// get and output OpenCL version supported by the platform
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VERSION>();
								std::cout << "\tVersion: " << outputString << std::endl;

								int counter = 1;
								std::cout << "\nNumber of GPU devices available to platform " << user_input_p << ": " << counter << std::endl;
								std::cout << "--------------------" << std::endl;
								// output device index
								std::cout << "  Device " << j << std::endl;

								// get and output device name
								outputString = devices[j].getInfo<CL_DEVICE_NAME>();
								std::cout << "\tName: " << outputString << std::endl;
								std::cout << std::endl;
								counter++;

								std::cout << "Please select a GPU device: ";
								std::cin >> user_input_d;


								std::cout << "--------------------" << std::endl;
								std::cout << "Creating a context for device " << user_input_d << "..." << std::endl;

								// create a context for all available devices on that platform
								context = cl::Context(devices[j]);


								// check devices in the context
								std::cout << "\nDevices in the context:" << std::endl;

								// get devices in the context
								std::vector<cl::Device> contextDevices = context.getInfo<CL_CONTEXT_DEVICES>();


								// create a command queue for all available devices on that platform
								queue = cl::CommandQueue(context, devices[j]);
								//std::cout << outputString << std::endl;

								// output names of devices in the context
								for (k = 0; k < contextDevices.size(); k++)
								{

									// get and output device type
									cl_device_type type;
									contextDevices[k].getInfo(CL_DEVICE_TYPE, &type);
									if (type == CL_DEVICE_TYPE_GPU)
									{
										outputString = contextDevices[k].getInfo<CL_DEVICE_NAME>();
										std::cout << "  Device " << user_input_d << ": " << outputString << std::endl;
										std::cout << std::endl;
										std::cout << "Context created" << std::endl;
										std::cout << "Command queue created" << std::endl;
										std::cout << std::endl;

										// open input file stream to .cl file
										std::ifstream programFile("task1.cl");

										// check whether file was opened
										if (!programFile.is_open())
										{
											"File not found.";
										}

										// create program string and load contents from the file
										std::string programString(std::istreambuf_iterator<char>(programFile), (std::istreambuf_iterator<char>()));

										// create program source from one input string
										cl::Program::Sources source(1, std::make_pair(programString.c_str(), programString.length() + 1));
										// create program from source
										cl::Program program(context, source);

										// build the program for the devices in the context
										program.build(contextDevices);

										std::cout << "Program build: Successful" << std::endl;
										std::cout << "--------------------" << std::endl;

										if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_SUCCESS)
										{
											std::cout << "Build success!" << std::endl;
											std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;

											//all kernels
											std::vector<cl::Kernel> allKernel;

											//create all kernels in program
											program.createKernels(&allKernel);
											std::cout << "--------------------" << std::endl;
											std::cout << "Total number of kernels: " << allKernel.size() << std::endl;


											for (i = 0; i < allKernel.size(); i++)
											{
												outputString = allKernel[i].getInfo<CL_KERNEL_FUNCTION_NAME>();
												std::cout << "Kernel " << i << ": " << outputString << std::endl;
											}
											std::cout << allKernel.size() << " Kernels added" << std::endl;
											std::cout << "--------------------" << std::endl;
											std::cout << std::endl;
										}

										if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_ERROR)
										{
											std::cout << "Build error!" << std::endl;
											std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;
											std::cout << "--------------------" << std::endl;
											std::cout << std::endl;
										}
									}
									else
									{
										continue;
									}
								}
							}
							else
							{
								std::cout << "No GPU devices found" << std::endl;
							}
						}

						std::cout << "Returning to main menu...\n" << std::endl;
						// break from loop, since required number of devices found
						break;
					}
					else
					{
						std::cout << "0 devices found" << std::endl;
						std::cout << "Returning to main menu...\n" << std::endl;
						break;
					}
					break;

				case 3:
					if (devices.size() > 0)
					{
						// get all devices available to the platform
						platforms[user_input_p].getDevices(CL_DEVICE_TYPE_ALL, &devices);

						for (j = 0; j < devices.size(); j++)
						{
							// get and output device type
							cl_device_type type;
							devices[j].getInfo(CL_DEVICE_TYPE, &type);
							if (type == CL_DEVICE_TYPE_GPU)
							{
								std::cout << "--------------------" << std::endl;
								// output platform index
								std::cout << "  Platform " << user_input_p << ":" << std::endl;

								// get and output platform name
								platforms[user_input_p].getInfo(CL_PLATFORM_NAME, &outputString);
								std::cout << "\tName: " << outputString << std::endl;

								// get and output platform vendor name
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VENDOR>();
								std::cout << "\tVendor: " << outputString << std::endl;

								// get and output OpenCL version supported by the platform
								outputString = platforms[user_input_p].getInfo<CL_PLATFORM_VERSION>();
								std::cout << "\tVersion: " << outputString << std::endl;

								int counter = 1;
								std::cout << "\nNumber of CPU devices available to platform " << user_input_p << ": " << counter << std::endl;
								std::cout << "--------------------" << std::endl;
								// output device index
								std::cout << "  Device " << j << std::endl;

								// get and output device name
								outputString = devices[j].getInfo<CL_DEVICE_NAME>();
								std::cout << "\tName: " << outputString << std::endl;
								std::cout << std::endl;
								counter++;

								std::cout << "Please select a CPU device: ";
								std::cin >> user_input_d;


								std::cout << "--------------------" << std::endl;
								std::cout << "Creating a context for device " << user_input_d << "..." << std::endl;

								// create a context for all available devices on that platform
								context = cl::Context(devices[j]);


								// check devices in the context
								std::cout << "\nDevices in the context:" << std::endl;

								// get devices in the context
								std::vector<cl::Device> contextDevices = context.getInfo<CL_CONTEXT_DEVICES>();

								// create a command queue for all available devices on that platform
								queue = cl::CommandQueue(context, devices[j]);

								// output names of devices in the context
								for (k = 0; k < contextDevices.size(); k++)
								{
									// get and output device type
									cl_device_type type;
									contextDevices[k].getInfo(CL_DEVICE_TYPE, &type);
									if (type == CL_DEVICE_TYPE_GPU)
									{
										int counter = 1;
										outputString = contextDevices[k].getInfo<CL_DEVICE_NAME>();
										std::cout << "  Device " << counter << ": " << outputString << std::endl;
										std::cout << std::endl;
										std::cout << "Context created" << std::endl;
										std::cout << "Command queue created" << std::endl;
										std::cout << std::endl;

										// open input file stream to .cl file
										std::ifstream programFile("task1.cl");

										// check whether file was opened
										if (!programFile.is_open())
										{
											quit_program("File not found.");
										}

										// create program string and load contents from the file
										std::string programString(std::istreambuf_iterator<char>(programFile), (std::istreambuf_iterator<char>()));

										// create program source from one input string
										cl::Program::Sources source(1, std::make_pair(programString.c_str(), programString.length() + 1));
										// create program from source
										cl::Program program(context, source);

										// build the program for the devices in the context
										program.build(contextDevices);

										std::cout << "Program build: Successful" << std::endl;
										std::cout << "--------------------" << std::endl;

										if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_SUCCESS)
										{
											std::cout << "Build success!" << std::endl;
											std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;

											//all kernels
											std::vector<cl::Kernel> allKernel;

											//create all kernels in program
											program.createKernels(&allKernel);
											std::cout << "--------------------" << std::endl;
											std::cout << "Total number of kernels: " << allKernel.size() << std::endl;


											for (i = 0; i < allKernel.size(); i++)
											{
												outputString = allKernel[i].getInfo<CL_KERNEL_FUNCTION_NAME>();
												std::cout << "Kernel " << i << ": " << outputString << std::endl;
											}
											std::cout << allKernel.size() << " Kernels added" << std::endl;
											std::cout << "--------------------" << std::endl;
											std::cout << std::endl;
										}

										if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(contextDevices[k]) == CL_BUILD_ERROR)
										{
											std::cout << "Build error!" << std::endl;
											std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[k]) << std::endl;
											std::cout << "--------------------" << std::endl;
											std::cout << std::endl;
										}
										counter++;
									}
									else
									{
										continue;
									}
								}
							}
							else
							{
								std::cout << "No GPU devices found" << std::endl;
							}
						}

						std::cout << "Returning to main menu...\n" << std::endl;
						// break from loop, since required number of devices found
						break;
					}
					else
					{
						std::cout << "0 devices found" << std::endl;
						std::cout << "Returning to main menu...\n" << std::endl;
						break;
					}

				}
				break;

			case 3:
				// exit option
				std::cout << std::endl;
				isRunning = false;
				break;

			default:
				//invalid option
				std::cout << std::endl;
				std::cout << "Invalid choice. Please enter 1,2 or 3." << std::endl;
				std::cout << "Returning to main menu...\n" << std::endl;
				continue;
			}
		}

		return 0;
	}
