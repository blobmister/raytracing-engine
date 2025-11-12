cmake_minimum_required(VERSION 3.23)

project(Raytracing)

add_executable(ppm-image)
target_sources(ppm-image
	PRIVATE
		ppm-image/main.cxx
)
