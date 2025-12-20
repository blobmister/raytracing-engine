#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"
#include <iostream>


// Convenient typedef in order to differentiate
// between color vectors and geometry vectors
using color = vec3;

// Writes out color data to specified outstream from one vector
//
// Expects vector components to be in the range [0, 1]. Converts them
// to the [0, 255] range, and then outputs them in the ppm format.
inline void write_color(std::ostream& out, const color& pixel_color) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// Translate the [0, 1] value into the appropriate [0, 255] byte range
	static const interval intensity(0.000, 0.999);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(255.999 * intensity.clamp(g));
	int bbyte = int(255.999 * intensity.clamp(b));

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
