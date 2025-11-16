#include <iostream>
#include <vec3.h>
#include <color.h>
#include <ray.h>


// Simple utility class to check if a sphere with a given centre and radius
// is hit by a given ray for any value of t. It returns the minimal value of t
// such that the ray intersects the surface of the sphere at that value.
double hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = center - r.origin();
	auto a = dot(r.direction(), r.direction());
	auto h = dot(r.direction(), oc);
	auto c = dot(oc, oc) - radius*radius;
	auto discriminant = h*h - a*c;
	
	if (discriminant < 0) {
		return -1.0;
	} else {
		return (h - std::sqrt(discriminant) ) / (a);
	}
}

color ray_color(const ray& r) {
	// Render red circle
	auto t = hit_sphere(point3(0,0,-1), 0.5, r);
	
	// Render the surface Normals of the sphere
	if (t > 0.0) {
		vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
		return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
	}

	// Simple blue gradient colouring
	vec3 unit_direction = unit_vector(r.direction());

	auto a  = 0.5 * (unit_direction.y() + 1.0);
	return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}


int main() {
	// Image
	//
	// A 16/9 aspect ratio is used. Actual pixel numbers are approximate
	// A minimum height of 1 is set incase image_width is too low to maintain
	// aspect ratio.

	double aspect_ratio = 16.0 / 9.0;
	int image_width = 400;

	int image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	// Camera
	//
	// Camera centre is set to 0,0,0 for simplicity, and a focal length of 1 is also assumed
	
	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (double(image_width)/image_height);
	auto camera_center = point3(0,0,0);

	// Viewport vector def
	//
	// A separate coordinate system from the camera system is required to render the viewport image
	// This is as (0, 0) on the viewport is assumed to be the top left of the viewport, but it is
	// considered to be the centre on the camera coordinate system. 
	//
	// viewport_u represents the horiztonal vector from (0, 0), to the right edge of the veiwport and 
	// viewport_v represents the vertical vertical from (0, 0), to the bottom edge of the veiwport.
	
	auto viewport_u = vec3(viewport_width, 0, 0);
	auto viewport_v = vec3(0, -viewport_height, 0);

	// Pixel-to-pixel gaps
	//
	// Pixels are taken to be 'dots' in the center of a grid. Since the grid requires
	// boxes with width and height, pixels have gaps in between them. This represents that
	// gap amount.

	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	// Location of upper left pixel
	
	auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
	auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


	// Render
	//
	// Below is rendering logic for desired image in ppm format.
	

	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int j = 0; j < image_height; j++) {
		std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
		for (int i = 0; i < image_width; i++) {
			auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
			auto ray_direction = pixel_center - camera_center;
			ray r(camera_center, ray_direction);

			color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);
		}
	}

	std::clog << "\rDone.					\n";

}

