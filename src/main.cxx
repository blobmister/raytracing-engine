#include <util.h>
#include <hittable.h>
#include <hittable-list.h>
#include <sphere.h>
#include <camera.h>


int main() {
	
	hittable_list world;

	world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
	world.add(make_shared<sphere>(point3(0,-100.5,-2), 100));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1920;
	cam.samples_per_pixel = 10;
	cam.sample_radius = 0.5;
	cam.diffuse = true;
	cam.diffusion_colour_amount = 0.5;
	cam.max_recurse_depth = 50;

	cam.render(world);

}

