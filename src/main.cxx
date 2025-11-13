#include <iostream>
#include <vec3.h>
#include <color.h>

int main() {
	double apsect_ratio = 16.0 / 9.0;
	int image_width = 400;

	int image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	double viewport_height = 2.0;
	double viewport_width = viewport_height * (double(image_width/image_height));
}

