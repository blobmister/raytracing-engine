#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "color.h"
#include "util.h"
#include "material.h"

#include <chrono>
#include <thread>
#include <atomic>
#include <vector>

class camera {
    public:
        double aspect_ratio = 1.0;
        int image_width = 100;
        int samples_per_pixel = 100;
        double sample_radius = 0.5;
        double diffusion_colour_amount = 0.5;
        int max_recurse_depth = 10;

        double vfov = 90;
        point3 lookfrom = point3(0, 0, 0);
        point3 lookat = point3(0, 0, -1);
        vec3 vup = vec3(0, 1, 0);

        double defocus_angle = 0;
        double focus_dist = 10;

        bool multithread_mode = false;

        void render(const hittable& world) {
            if (multithread_mode) multi_thread_render(world);
            else single_thread_render(world);
        }

        void single_thread_render(const hittable& world) {
            auto start = std::chrono::high_resolution_clock::now();
            initialize();

            int total = image_height * image_width;
            int cur = 0;

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j < image_height; j++) {
                for (int i = 0; i < image_width; i++) {
                    color pixel_color(0,0,0);

                    // Averages the colour around the pixel to avoid 'jagged' edges
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_recurse_depth, world);
                    }

                    write_color(std::cout, pixel_samples_scale * pixel_color);
                    ++cur;
                    generate_loading_bar(cur, total, start);
                }
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start).count();
            long long minutes = duration / 60;
            long long seconds = duration % 60;
            std::clog << "\rTotal time taken: " << minutes << "m " << seconds << "s                                                                                                                                                \n";
        }

        void multi_thread_render(const hittable& world) {
            auto start = std::chrono::high_resolution_clock::now();
            initialize();

            int total  = image_height * image_width;
            std::vector<color> colors(total);
            std::atomic<int> completed = 0;

            int thread_count = std::thread::hardware_concurrency();
            if (thread_count == 0) {
                std::clog << "Unable to determine thread count, defaulting to 2\n";
                thread_count = 2;
            }

            std::vector<std::thread> threads;
            int rows_per_thread = image_height / thread_count;
            for (int i{}; i < thread_count; ++i) {
                int start = i * rows_per_thread;
                int end = (i == thread_count - 1) ? image_height : start + rows_per_thread;
                threads.emplace_back(
                    &camera::render_row,
                    this,
                    std::cref(world),
                    std::ref(colors),
                    std::ref(completed),
                    start,
                    end
                );
            }


            while (completed < total) {
                generate_loading_bar(completed, total, start);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            for (auto& t : threads) {
                t.join();
            }

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
            for (int i{}; i < total; ++i) {
                write_color(std::cout, pixel_samples_scale * colors[i]);
            }

            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start).count();
            long long minutes = duration / 60;
            long long seconds = duration % 60;
            std::clog << "\rTotal time taken: " << minutes << "m " << seconds << "s                                                                                                                               \n";
        }

    private:

        int image_height;
        double pixel_samples_scale;
        point3 center;
        point3 pixel00_loc;
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;
        vec3 u, v, w;
        vec3 defocus_disk_u;
        vec3 defocus_disk_v;

        void generate_loading_bar(const int cur, int total, const std::chrono::time_point<std::chrono::high_resolution_clock>& start) {
            int percent_complete = static_cast<int>((double(cur)/total) * 100.0);
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

        void render_row(const hittable& world, std::vector<color>& colors, std::atomic<int>& completed, int start_row, int end_row) {

            for  (int j = start_row; j < end_row; ++j) {
                for (int i{}; i < image_width; ++i) {
                    color pixel_color(0, 0, 0);
                    for (int sample{}; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_recurse_depth, world);
                    }

                    colors[j * image_width + i] = pixel_color;
                    ++completed;
                }
            }
        }

        void initialize() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = lookfrom;

            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta/2);
            auto viewport_height = 2 * h * focus_dist;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            vec3 viewport_u = viewport_width * u;
            vec3 viewport_v = viewport_height * -v;

            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
	        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

			auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
			defocus_disk_u = u * defocus_radius;
			defocus_disk_v = v * defocus_radius;
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

            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
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

        point3 defocus_disk_sample() const {
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
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
