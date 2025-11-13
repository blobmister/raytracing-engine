#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>


// Convenient typedef in order to differentiate
// between color vectors and geometry vectors
using color = vec3;



// Writes out color data to specified outstream from one vector
//
// Expects vector components to be in the range [0, 1]. Converts them 
// to the [0, 255] range, and then outputs them in the ppm format.
void write_color(std::ostream& out, const color& pixel_color) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// Translate the [0, 1] value into the appropriate [0, 255] byte range
	int rbyte = int(255.999 * r);
	int gbyte = int(255.999 * g);
	int bbyte = int(255.999 * b);

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif

