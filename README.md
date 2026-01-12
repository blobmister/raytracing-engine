# Basic Ray Tracing Engine
A simple raytracing engine made by following [this](https://raytracing.github.io/books/RayTracingInOneWeekend.html) guide. 

WIP: I extend on this further by implementing multi-threading + CUDA optimisations, and then use a micro-benchmarking tool to compare performance between single thread CPU, multi thread CPU, and multi thread GPU for basic graphics tasks.


For more details about my current progress, how I have optimized the render times compared to the original guide, and other implmenetation features of the project, please consult the [wiki](https://github.com/blobmister/raytracing-engine/wiki).

# Usage
To compile, run the following commands in the root directory:

```
cmake -B build
cmake --build build
```

Then pipe the resultant main object file in the build directory to an image file:
```
cd build
./main > image.ppm
```
# Features
Allows users to generate images of basic spheres, of differing materials. Will eventually allow users to configure an environment using a simple config.txt file, which the program should read in to create images, allowing for comparisons between single-core CPU, multi-core CPU, and multi-core GPU in the mathematics involved in generating these images.

