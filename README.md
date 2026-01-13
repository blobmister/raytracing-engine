# Ray Tracing Engine
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
Currently has support to render images using multi-core and single-core CPU. Eventually will add support for a simple script to generate images without recompilation of the program using a basic config file style syntax. Currently working on using the GPU to reduce render times.

# Sample Image
<img width="400" height="225" alt="image" src="https://github.com/user-attachments/assets/72386795-98cb-426e-b774-9bb8c34daa93" />


