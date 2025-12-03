cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Raytracing)

add_executable(main)
target_sources(main
	PRIVATE
		src/main.cxx
)

add_library(vec INTERFACE)
target_sources(vec
	INTERFACE
		FILE_SET HEADERS
		BASE_DIRS
			src/object-library
		FILES
			src/object-library/vec3.h
			src/object-library/ray.h
			src/object-library/color.h
			src/object-library/hittable.h
			src/object-library/sphere.h
			src/object-library/hittable-list.h
			src/object-library/util.h
			src/object-library/interval.h
			src/object-library/camera.h
)

target_include_directories(vec
	INTERFACE
		src/object-library
)

target_link_libraries(main
	PRIVATE
		vec
)
