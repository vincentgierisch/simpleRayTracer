#include <iostream>
#include "math.h"

#include <string>
#include <fstream>

#include <vector>
#include <algorithm>

#include <CL/opencl.hpp>
#define N 100000000

int main() {
    // cuda_hello<<<1, 10>>>();
    // cudaDeviceSynchronize();

    float *a, *b, *out;

    a = new float[N];
    b = new float[N];
    out = new float[N];

    std::cout << "initialize array" << std::endl;
    // Initialize array
    for(int i = 0; i < N; i++){
        a[i] = 1.0f; b[i] = 2.0f;
    }
    std::cout << "Array initialized" << std::endl;

    std::cout << "load kernel code" << std::endl;
    std::ifstream fileStream("./ocl/vectorAdd.cl");

    if (!fileStream) {
        std::cerr << "Failed to load kernel" << std::endl;
        exit(1);
    }
    std::string kernelCode((std::istreambuf_iterator<char>(fileStream)),
                 std::istreambuf_iterator<char>());

    // Filter for a 2.0 or newer platform and set it as the default
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::Platform plat;
    for (auto &p : platforms) {
        std::string platver = p.getInfo<CL_PLATFORM_VERSION>();
        std::cout << platver << std::endl;
        if (platver.find("OpenCL 2.") != std::string::npos ||
            platver.find("OpenCL 3.") != std::string::npos) {
            // Note: an OpenCL 3.x platform may not support all required features!
            plat = p;
        }
    }
    if (plat() == 0) {
        std::cout << "No OpenCL 2.0 or newer platform found.\n";
        return -1;
    }
 
    cl::Platform newP = cl::Platform::setDefault(plat);
    if (newP != plat) {
        std::cout << "Error setting default platform.\n";
        return -1;
    }

    // create context
    cl_context_properties cps[3] = {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)(newP)(),
        0
    };

    std::cout << "created context" << std::endl;

    cl::Context context(CL_DEVICE_TYPE_GPU, cps);

    // Get a list of devices on this platform
    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

    // create a command queue and use the first device
    cl::CommandQueue queue = cl::CommandQueue(context, devices[0]);

    cl::vector<cl::string> sourceStrings(1, kernelCode.c_str());
    cl::Program::Sources source(sourceStrings);
    cl::Program program = cl::Program(context, source);
    std::cout << "loaded program" << std::endl;
    program.build(devices);

    std::cout << "loaded program" << std::endl;

    cl::Kernel kernel(program, "vectorAdd");

    cl::Buffer bufferA = cl::Buffer(context, CL_MEM_READ_ONLY, N * sizeof(float));
    cl::Buffer bufferB = cl::Buffer(context, CL_MEM_READ_ONLY, N * sizeof(float));
    cl::Buffer bufferOut = cl::Buffer(context, CL_MEM_WRITE_ONLY, N * sizeof(float));

    queue.enqueueWriteBuffer(bufferA, CL_TRUE, 0, N * sizeof(float), a);
    queue.enqueueWriteBuffer(bufferB, CL_TRUE, 0, N * sizeof(float), b);

    // Set arguments to kernel
    kernel.setArg(0, bufferA);
    kernel.setArg(1, bufferB);
    kernel.setArg(2, bufferOut);

    // Run the kernel
    cl::NDRange global(N);
    cl::NDRange local(1);
    std::cout << "Run Kernel" << std::endl;
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);
    std::cout << "Kernel finished" << std::endl;
    queue.enqueueReadBuffer(bufferOut, CL_TRUE, 0, N * sizeof(float), out);

    float maxError = 0.0f;
    std::cout << out[0] << std::endl;
    for(int i = 0; i < N; i++) {
        maxError = fmax(maxError, fabs(out[i]-3.0f));
    }
    std::cout << "Max error: " << maxError << std::endl;

    delete[] a;
    delete[] b;
    delete[] out;

    return 0;
}

