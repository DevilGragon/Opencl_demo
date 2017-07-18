//For clarity,error checking has been omitted.
#include <CL/cl.h>
#include "tool.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main(int argc, char* argv[])
{
    cl_int    status;
    /**Step 1: Getting platforms and choose an available one(first).*/
    cl_platform_id platform;
    getPlatform(platform);

    /**Step 2:Query the platform and choose the first GPU device if has one.*/
    cl_device_id *devices=getCl_device_id(platform);

    /**Step 3: Create context.*/
    cl_context context = clCreateContext(NULL,1, devices,NULL,NULL,NULL);

    /**Step 4: Creating command queue associate with the context.*/
    cl_command_queue commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);

    /**Step 5: Create program object */
    const char *filename = "HelloWorld_Kernel.cl";
    string sourceStr;
    status = convertToString(filename, sourceStr);
    const char *source = sourceStr.c_str();
    size_t sourceSize[] = {strlen(source)};
    cl_program program = clCreateProgramWithSource(context, 1, &source, sourceSize, NULL);

    /**Step 6: Build program. */
    status=clBuildProgram(program, 1,devices,NULL,NULL,NULL);

    /*LOG INFO*/
    char* build_log;
    size_t log_size;
    // First call to know the proper size
    clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
    build_log = new char[log_size+1];
    // Second call to get the log
    clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, log_size, build_log, NULL);
    build_log[log_size] = '\0';
    cout << build_log << endl;
    delete[] build_log;

    /**Step 7: Initial input,output for the host and create memory objects for the kernel*/
    const int NUM=25;
    float* input = new float[NUM];
    for(int i=0;i<NUM;i++)
    {
        input[i]=i;
        cout<<input[i];
    }
    cout<<endl;
    float* output = new float[NUM];

    cl_mem inputBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, (NUM) * sizeof(float),(void *) input, NULL);
    cl_mem outputBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY , NUM * sizeof(float), NULL, NULL);

    /**Step 8: Create kernel object */
    cl_kernel kernel = clCreateKernel(program,"helloworld", NULL);

    /**Step 9: Sets Kernel arguments.*/
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&inputBuffer);
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&outputBuffer);
    status = clSetKernelArg(kernel, 2, sizeof(int), &NUM);

    /**Step 10: Running the kernel.*/
    size_t global_work_size[1] = {NUM};
    cl_event enentPoint;
    status = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, &enentPoint);
    clWaitForEvents(1,&enentPoint); ///wait
    clReleaseEvent(enentPoint);

    /**Step 11: Read the cout put back to host memory.*/
    status = clEnqueueReadBuffer(commandQueue, outputBuffer, CL_TRUE, 0, NUM * sizeof(float), output, 0, NULL, NULL);
    for(int i=0;i<NUM;i++)
    {
        cout<<output[i];
    }
    cout<<endl;
    //cout<<output[NUM - 1]<<endl;

    /**Step 12: Clean the resources.*/
    status = clReleaseKernel(kernel);//*Release kernel.
    status = clReleaseProgram(program);    //Release the program object.
    status = clReleaseMemObject(inputBuffer);//Release mem object.
    status = clReleaseMemObject(outputBuffer);
    status = clReleaseCommandQueue(commandQueue);//Release  Command queue.
    status = clReleaseContext(context);//Release context.

    if (output != NULL)
    {
        free(output);
        output = NULL;
    }

    if (devices != NULL)
    {
        free(devices);
        devices = NULL;
    }
    return 0;
}