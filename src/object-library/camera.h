#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "color.h"
#include "util.h"
#include "material.h"

#include <chrono>

class camera {
    public:
        double aspect_ratio = 1.0;
        int image_width = 100;
        int samples_per_pixel = 100;
        double sample_radius = 0.5;
        double diffusion_colour_amount = 0.5;
        int max_recurse_depth = 10;

        void render(const hittable& world) {
            auto start = std::chrono::high_resolution_clock::now();
            initialize();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j < image_height; j++) {
                generate_loading_bar(j, start);
                for (int i = 0; i < image_width; i++) {
                    color pixel_color(0,0,0);

                    // Averages the colour around the pixel to avoid 'jagged' edges
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_recurse_depth, world);
                    }

                    write_color(std::cout, pixel_samples_scale * pixel_color);
                }
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start).count();
            long long minutes = duration / 60;
            long long seconds = duration % 60;
            std::clog << "\rTotal time taken: " << minutes << "m " << seconds % 60 << "s                                                                                                                                                \n";
        }

    private:

        int image_height;
        double pixel_samples_scale;
        point3 center;
        point3 pixel00_loc;
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;

        void generate_loading_bar(int j, const std::chrono::time_point<std::chrono::high_resolution_clock>& start) {
            int percent_complete = static_cast<int>((double(j)/image_height) * 100.0);
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now-start).count();
            long long minutes = elapsed / 60;
            long long seconds = elapsed % 60;

            std::clog << "[";
            for (int i = 0; i < 100; i++) {
                if (i < percent_complete) std::clog << "=";
                else if (i == percent_complete) std::clog << ">";
                else std::clog << " ";
            }
            std::clog << "] " << percent_complete << "%, Elapsed Time: " << minutes << "m " << seconds << "s \r";
        }

        void initialize() {
            /*
                Image

                A 16/9 aspect ratio is used. Actual pixel numbers are approxiamte. A minimum height of 1
                is set incase image_width is too low to maintain aspect ratio.

            */

            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = point3(0, 0, 0);


            auto focal_length = 1.0;
            auto viewport_height = 2.0;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            /*
                Viewport vectors

                A separate coordinate system from the camera system is required to render the viewport iamge
                This is as (0, 0) on the viewport is assumed to be the top left of the viewport, but it is considered
                to be the center on the camera coordinate system.

                viewport_u represents the horizontal vector from (0, 0) to the right edge of the viewport
                viewport_v represents the horiztonal vector from (0, 0) to the bottom edge of the viewport

            */
            auto viewport_u = vec3(viewport_width, 0, 0);
            auto viewport_v = vec3(0, -viewport_height, 0);

            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
	        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


        }

        /*
            Constructs a camera ray originating from the origin and directed at a randomly
            sampled point around the pixel location (i, j). This is used by the render() function
            as for a simple anti-aliasing implementation.

            User can set the samples_per_pixel attribute in their camera object in order to set the number of samples for
            anti-aliasing.

        */
        ray get_ray(int i, int j) {
            auto offset = sample_square();
            auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = center;
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        /*
            Returns a vector to a random point in the a square with side length 2 * sample-radius

            Used as a helper function to get_ray()
        */
        vec3 sample_square() const {
            auto px = random_double() - 0.5;
            auto py = random_double() - 0.5;

            return vec3(px * sample_radius * 2, py * sample_radius * 2, 0);
        }


        /*
            Set to calculate colours based on direction of normals currently
        */
        color ray_color(const ray& r, int depth, const hittable& world) {
            if (depth <= 0) return color(0, 0, 0);

            hit_record rec;
            if (world.hit(r, interval(0.001, infinity), rec)) {
                ray scattered;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered))
                    return attenuation * ray_color(scattered, depth-1, world);
                return color(0, 0, 0);
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1);
            return (1.0-a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
        }
};

#endif
