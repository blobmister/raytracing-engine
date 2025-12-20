#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include "util.h"


// Simple 3-Dimensional vector class
//
// Some Notes: Many implementations opt for 4-Dimensional vectors in order to store an extra data-point
// (examples include alpha value for RGB), however this is not necessary for my simple ray tracing
// engine. This class is used for both coordinate geometry and colour. Whilst separating these
// into two different classes may aid readability, and prevent bugs related mixing the two up,
//  mathematically they are identical, and so for the sake of less writing, this class will be shared for
//  both use-cases.

class vec3 {
	public:
		double e[3];
		vec3() : e{0, 0, 0} {}
		vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

		double x() const { return e[0]; }
		double y() const { return e[1]; }
		double z() const { return e[2]; }

		vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
		double operator[](int i) const { return e[i]; }
		double& operator[](int i) { return e[i]; }

		vec3& operator+=(const vec3& v) {
			e[0] += v.e[0];
			e[1] += v.e[1];
			e[2] += v.e[2];

			return *this;
		}

		vec3& operator*=(double t) {
			e[0] *= t;
			e[1] *= t;
			e[2] *= t;
			return *this;
		}

		vec3& operator/=(double t) {
			return *this *= 1/t;
		}

		double length() const {
			return std::sqrt(length_squared());
		}

		double length_squared() const {
			return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
		}

		static vec3 random() {
			return vec3(random_double(), random_double(), random_double());
		}

		static vec3 random(double min, double max) {
			return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
		}


};

using point3 = vec3; // point3 acting as an alias for vec3 for readability when talking about points

// Utilility functions
//
// The following define vector addition, subtraction
// component-wise multiplication, scalar multiplication (and division),
// dot product, cross product, and normalization. These are all relatively simple calculations
// and have been defined here for readability

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}


inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
	return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
	return v * t;
}

inline vec3 operator/(const vec3& v, double t) {
	return (1/t) * v;
}


inline double dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(
			u.e[1] * v.e[2] - u.e[2] * v.e[1],
			u.e[2] * v.e[0] - u.e[0] * v.e[2],
			u.e[0] * v.e[1] - u.e[1] * v.e[0]
		);
}

inline vec3 unit_vector(const vec3& u) {
	return u / u.length();
}

/*
	Generates a unit random unit vector using the rejection algorithm as follows:

	Generate a random vector within the unit square. If this vector falls within the unit circle enscribed in this unit
	square, normalise it and return it, otherwise reject and generate a new one. This solution is easier to implement than other
	analytical based solutions for the purposes of generating diffuse materials.

	While not strictly necessary to reject all vectors outside of the unit circle, it ensures that the distribution
	of random vectors is uniformly distributed. Using some basic math, it is found that each attempt has roughly 52% chance of
	success, which is adequate for this purpose, and often faster than the analytical methods due to the simple mathematics required

*/
inline vec3 random_unit_vector() {
	while (true) {
		auto p = vec3::random(-1, 1);
		auto lensq = p.length_squared();

		// Reject values that are too small to avoid floating point issues
		if (1e-160 < lensq && lensq <= 1) {
			return p / sqrt(lensq);
		}
	}
}

/*
	Generates a random unit vector on the hemisphere of a circle. Guarantees that that the vector is pointing outwards from
	the surface.
*/
inline vec3 random_on_hemisphere(const vec3& normal) {
	vec3 on_unit_hemisphere = random_unit_vector();
	if (dot(on_unit_hemisphere, normal) > 0.0) {
		return on_unit_hemisphere;
	} else {
		return -on_unit_hemisphere;
	}
}

#endif
