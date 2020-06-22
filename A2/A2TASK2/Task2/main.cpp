#define CL_USE_DEPRECATED_OPENCL_2_0_APIS    // using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS                // enable OpenCL exemptions

// C++ standard library and STL headers
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <map>
#include <iterator>


// OpenCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include "common.h"

// function prototypes
std::vector<char> returnFileContents(std::string fileName);
std::vector<char> removeAlphabetical(std::string inString);
void encrypt(std::vector<char> loadedString, int shiftAmt);
void encryptCL(std::vector<char> loadedString, cl::Program* program, 
        cl::CommandQueue* queue, cl::Context* context,int shiftAmt);
void encryptLookup(std::vector<char> loadedString, cl::Program* program, 
        cl::CommandQueue* queue, cl::Context* context);
int charToInt(char getChar);



int main(int argc, const char * argv[]) {
    std::cout << "-- Loading plaintext.txt --" << std::endl;
    
    std::vector<char> fileChar;
    fileChar = returnFileContents("plaintext.txt");
    
    cl::Platform platform;            // device's platform
    cl::Device device;                // device used
    cl::Context context;            // context for the device
    cl::Program program;            // OpenCL program object
    cl::CommandQueue queue;            // commandqueue for a context and device
    
    std::cout << "plaintext.txt loaded" << std::endl;
    
    std::cout << "Name: Yeo Zheng Xu Isaac" << std::endl;
    std::cout << "Student ID: 6342425" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "\t 1. -- 2a. Run using C++ program" << std::endl;
    std::cout << "\t 2. -- 2b. Run using OpenCL program" << std::endl;
    std::cout << "\t 3. -- 2c. Run OpenCL Lookup table program" << std::endl;
        
    int input = getRangeInput(1,3, "Select an option");
    int shift;
    
    if(input == 1){
         shift = getInput("Select a shift amount");
        encrypt(fileChar,shift);
    }
    else
    {
        try{
            // select a device.
            if (!select_one_device(&platform, &device)){
                // if no device selected
                quit_program("Device not selected.");
            }
            
            std::cout << "\nCreating context..." << std::endl;
            // create a context from device
            context = cl::Context(device);
            std::cout << "Context created" << std::endl;
            
            // create a command queue
            std::cout << "\nCreating command queue..." << std::endl;
            queue = cl::CommandQueue(context, device);
            std::cout << "Command queue created" << std::endl;
            
            // build the program
            if(!build_program(&program, &context, "prg.cl"))
            {
                // if OpenCL program build error
                quit_program("OpenCL program build error.");
            }
            if(input == 2){
                shift = getInput("Select a shit amount");
                encryptCL(fileChar,&program,&queue,&context,shift);
            }
            else
                encryptLookup(fileChar,&program,&queue,&context);
        } catch (cl::Error e) {
            // call function to handle errors
            handle_error(e);
        }
    }
}

std::vector<char>  returnFileContents(std::string fileName){
    // loads the file.
    std::ifstream in(fileName.c_str());
    
    if (!in){
        quit_program("Unable to load input file!");
    }
    
    std::string fileString;
    std::string::iterator it;
    std::vector<char> vecOfChar;
    std::vector<char> fileChar;
    
    // load the file, store every char
    while(std::getline(in,fileString)){
        if(fileString.size() > 0){
            for(it = fileString.begin(); it < fileString.end(); it++){
                char character = *it;
                fileChar.push_back(toupper(character));
            }
        }
    }

    return fileChar;
}


void encrypt(std::vector<char> loadedString, int encryptedBuffer){
    std::vector<char>::iterator it;
    std::vector<char> encryptedString;
    std::vector<char> decryptedString;
    


    
    std::cout << "\nEncrypting into output.txt " << std::endl;
    std::ofstream file;
    file.open ("output.txt", std::ios_base::app);
    
    for(it = loadedString.begin(); it < loadedString.end(); it++){
        char getChar = *it;
         if(getChar != ' '){    // if is char
             if(getChar < 'A'){
                encryptedString.push_back(getChar);
                 file << getChar;
                
            }
             else{
                 char encryptedChar = ((getChar+encryptedBuffer-65)%26+'A');
                 encryptedString.push_back(encryptedChar);
                 file << encryptedChar;
             }
        }
         else{ //if is space
             encryptedString.push_back(' ');
             file << ' ';
        }
    }
    file.close();
    std::cout << "Done " << std::endl;

   
    
    
    std::cout << std::endl;
    std::cout << "-- Decrypting into decrypted.txt --" << std::endl;
    // decryption.
    file.open ("decrypted.txt", std::ios_base::app);
    for(it = encryptedString.begin(); it < encryptedString.end(); it++){
        char charact = *it;
         if(charact != ' '){
             if ((charact < 'A') || (charact > 'Z')){
                 decryptedString.push_back(charact);
                 file << charact;
             }
             else{
                char plainChar = ((charact - encryptedBuffer-65)%26)+'A';
                if(plainChar < 'A')
                    plainChar = 'Z'- ('A'-plainChar-1);
                if(plainChar > 'Z')
                    plainChar = 'Z'-('A'-plainChar-1);
                decryptedString.push_back(plainChar);
                file << plainChar;
             }
        }else{
            decryptedString.push_back(' ');
            file << ' ';
        }
    }
    file.close();
    std::cout << "Done " << std::endl;

}


void encryptCL(std::vector<char> plaintext, cl::Program* program,
               cl::CommandQueue* queue, cl::Context* context,int shiftAmt){
 
    cl::Kernel kernel;                // a single kernel object
    cl::Event kernelEvent, readEvent;

    std::vector<cl_char>encryptVector;
    std::vector<cl_int> nVec(plaintext.size(),shiftAmt);
    
    
    kernel = cl::Kernel(*program, "encryptKernel");

    for (int i = 0; i < plaintext.size(); i++)
    {
        encryptVector.push_back(' ');
    }
    
    cl::Buffer plaintextBuffer = cl::Buffer(*context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_char) * plaintext.size(), &plaintext[0]);
    cl::Buffer NBuffer = cl::Buffer(*context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * plaintext.size(), &nVec[0]);
    cl::Buffer writeBuffer = cl::Buffer(*context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_char) * plaintext.size(), &encryptVector[0]);
    kernel.setArg(0,plaintextBuffer);
    kernel.setArg(1,writeBuffer);
    kernel.setArg(2,NBuffer);

    // enqueue
    cl::NDRange offset(0);
    cl::NDRange globalSize(plaintext.size());    // 4 work-units per kernel
    
    queue->enqueueNDRangeKernel(kernel, offset, globalSize);
    std::cout << "Kernel enqueued." << std::endl;

    std::cout << "---------------------------" << std::endl;
    std::cout << "Encrypting "<< std::endl;
    std::cout << "---------------------------" << std::endl;
    // enqueue command to read from device to host memory
    queue->enqueueReadBuffer(writeBuffer, CL_FALSE, 0, sizeof(cl_char) * plaintext.size(), &encryptVector[0], NULL, &readEvent);
    queue->finish();
    

    for(int i = 0; i < encryptVector.size(); i++){
        std::cout << encryptVector[i];
    }
    std::cout << std::endl;


    // initialize new vector
    std::vector<cl_char>decryptedPlaintext;
     for (int i = 0; i < encryptVector.size(); i++)
    {
        decryptedPlaintext.push_back(' ');
    }
    
    cl::Buffer encryptedBuffer = cl::Buffer(*context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_char) * encryptVector.size(), &encryptVector[0]);
    cl::Buffer decryptBuffer = cl::Buffer(*context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_char) * decryptedPlaintext.size(), &decryptedPlaintext[0]);
    
    
    kernel = cl::Kernel(*program, "decryptKernel");
    kernel.setArg(0,encryptedBuffer);
    kernel.setArg(1,decryptBuffer);
    kernel.setArg(2,NBuffer);

    cl::NDRange globalSizeDec(encryptVector.size());    // 4 work-units per kernel
    
    queue->enqueueNDRangeKernel(kernel, offset, globalSizeDec);
    
    std::cout << "Kernel enqueued." << std::endl;
    
    std::cout << "---------------------------" << std::endl;
    std::cout << "Decrypting "<< std::endl;
    std::cout << "---------------------------" << std::endl;
    // enqueue command to read from device to host memory
    queue->enqueueReadBuffer(decryptBuffer, CL_FALSE, 0, sizeof(cl_char) * encryptVector.size(), &decryptedPlaintext[0], NULL, &readEvent);
    queue->finish();

    for(int i = 0; i < decryptedPlaintext.size(); i++){
        std::cout << decryptedPlaintext[i];
    }
    std::cout << std::endl;
}


void encryptLookup(std::vector<char> loadedString, cl::Program* program, 
        cl::CommandQueue* queue, cl::Context* context){
        cl::Kernel kernel;                // a single kernel object

       
        cl::Event kernelEvent, readEvent;

       
        std::vector<cl_char>encryptVector;
        
        
        kernel = cl::Kernel(*program, "encryptKernelLookup");

        for (int i = 0; i < loadedString.size(); i++)
		{
			encryptVector.push_back('a');
		}
        
        cl::Buffer plaintextBuffer = cl::Buffer(*context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_char) * loadedString.size(), &loadedString[0]);
        cl::Buffer writeBuffer = cl::Buffer(*context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_char) * loadedString.size(), &encryptVector[0]);
        kernel.setArg(0,plaintextBuffer);
        kernel.setArg(1,writeBuffer);

        // enqueue
        cl::NDRange offset(0);
        cl::NDRange globalSize(loadedString.size());    // 4 work-units per kernel
        
        queue->enqueueNDRangeKernel(kernel, offset, globalSize);
        
        
        std::cout << "Kernel enqueued." << std::endl;
        std::cout << "---------------------------" << std::endl;
        
        std::cout << "Decrypting "<< std::endl;
        std::cout << "---------------------------" << std::endl;           // enqueue command to read from device to host memory
        queue->enqueueReadBuffer(writeBuffer, CL_FALSE, 0, sizeof(cl_char) * loadedString.size(), &encryptVector[0], NULL, &readEvent);
        queue->finish();
        

        for(int i = 0; i < encryptVector.size(); i++){
            std::cout << encryptVector[i];
        }
        std::cout << std::endl;


        // pepare the other vectors.
        std::vector<cl_char>decryptedPlaintext;
         for (int i = 0; i < encryptVector.size(); i++)
		{
			decryptedPlaintext.push_back('a');
		}
        
        cl::Buffer encryptedBuffer = cl::Buffer(*context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_char) * encryptVector.size(), &encryptVector[0]);
        cl::Buffer decryptBuffer = cl::Buffer(*context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_char) * decryptedPlaintext.size(), &decryptedPlaintext[0]);
        
        // create a kernel*/
        kernel = cl::Kernel(*program, "decryptKernelLookup");
        kernel.setArg(0,encryptedBuffer);
        kernel.setArg(1,decryptBuffer);

        cl::NDRange globalSizeDec(encryptVector.size());    // 4 work-units per kernel
        
        queue->enqueueNDRangeKernel(kernel, offset, globalSizeDec);
        
        
           
        std::cout << "Kernel enqueued." << std::endl;
        std::cout << "---------------------------" << std::endl;
       std::cout << "Decrypting "<< std::endl;
       std::cout << "---------------------------" << std::endl;
          // enqueue command to read from device to host memory
        queue->enqueueReadBuffer(decryptBuffer, CL_FALSE, 0, sizeof(cl_char) * encryptVector.size(), &decryptedPlaintext[0], NULL, &readEvent);
        queue->finish();

        for(int i = 0; i < decryptedPlaintext.size(); i++){
            std::cout << decryptedPlaintext[i];
        }
        std::cout << std::endl;
}
