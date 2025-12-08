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
Allows users to generate images of basic spheres, of differing materials. Will eventually allow users to configure an environment using a simple config.txt file, which the program should read in to create images, allowing for comparisons between single-core CPU, multi-core CPU, and multi-core GPU in the mathematics involved in generating these images.

# Images produced
Below I show the progression of images produced through the development of this engine.

## Sphere gradient based on surface normals
<img width="400" height="225" alt="image" src="https://github.com/user-attachments/assets/464573e3-928d-4bdf-b726-d07dc10925e2" />

A simple sphere rendered by calculating suprphase normals of intersecting rays, and using those normals to create a simple gradient colouring. 

### Anti-Aliasing
To remove the jagged edges of the previous image, a simple anti-aliasing implementation is used, where we sample colours from a small square around each pixel, and uniformly average the colour.
A very large sample radius is used in the below image to make the effect clearer (subsequent images use a more sensible radius of 0.5).


<img width="400" height="225" alt="image" src="https://github.com/user-attachments/assets/cbe46640-1a2e-448b-897d-ef711cfb3c29" />


## Diffusion

<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/016f745c-0cc9-49d2-9195-d24940053a93" />

A basic rejection based diffusion algorithm is used to generate this image. When a ray hits the surphace of the sphere, it bounces off in a random direction, and takes on the colour of whatever it hits after that. 

### Shadow Acne
Due to the recursive method of generating a diffuse image, often dark areas are generated due to floating point errors. This is referred to as shadow acne. Below is a diffuse image with shadow acne correction applied.

<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/6e0efc54-c671-4206-ac68-ee74d29465d5" />

### Currently working on the rest...
