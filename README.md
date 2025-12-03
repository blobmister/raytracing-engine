# Basic Ray Tracing Engine
A simple raytracing engine made by following [this](https://raytracing.github.io/books/RayTracingInOneWeekend.html) guide. 

WIP: I extend on this further by implementing multi-threading + CUDA optimisations, and then use a micro-benchmarking tool to compare performance between single thread CPU, multi thread CPU, and multi thread GPU for basic graphics tasks.

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
The repo consists of an object interfact that can be used to generate images. This interface has header only implementations of a basic 3-Dimension vector (which is used for storing geometric and colour data), and header only implementations of spheres, with logic to calculate ray intersctions.

# Images produced
Below I show the progression of images produced through the development of this engine.

## Sphere gradient based on surface normals
<img width="400" height="225" alt="image" src="https://github.com/user-attachments/assets/464573e3-928d-4bdf-b726-d07dc10925e2" />

A simple sphere rendered by calculating suprphase normals of intersecting rays, and using those normals to create a simple gradient colouring. 

### Anti-Aliasing
To remove the jagged edges of the previous image, a simple anti-aliasing implementation is used, where we sample colours from a small square around each pixel, and uniformly average the colour.
A very large sample radius is used in the below image to make the effect clearer (subsequent images use a more sensible radius of 0.5).


<img width="400" height="225" alt="image" src="https://github.com/user-attachments/assets/cbe46640-1a2e-448b-897d-ef711cfb3c29" />


### Currently working on the rest...
