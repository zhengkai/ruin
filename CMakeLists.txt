cmake_minimum_required(VERSION 4.0.0)

project(ruin VERSION 1.2)
set(PROJ ruin)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MAKE_PROGRAM_ARGS "--no-print-directory")
set(VCPKG_VERBOSE OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_BUILD_TYPE STREQUAL "debug")
	target_compile_options(${PROJ} PRIVATE
		-Wall -Wextra -Werror
		-fsanitize=address,undefined
		-fno-omit-frame-pointer
	)
	target_link_options(${PROJ} PRIVATE
		-fsanitize=address,undefined
		-fno-omit-frame-pointer
		-static-libasan
		-static-libubsan
	)
endif()

if (MSVC)
	add_compile_options(/utf-8)
    set(BUILD_SHARED_LIBS ON)
else()
    set(BUILD_SHARED_LIBS OFF)
endif()

include(cmake/build-info.cmake)

find_package(box2d CONFIG REQUIRED QUIET)
find_package(fmt CONFIG REQUIRED QUIET)
find_package(spdlog CONFIG REQUIRED QUIET)
find_package(CLI11 CONFIG REQUIRED QUIET)

find_package(SDL3 CONFIG REQUIRED QUIET)
find_package(SDL3_image CONFIG REQUIRED QUIET)
if (NOT EMSCRIPTEN)
find_package(SDL3_ttf CONFIG REQUIRED QUIET)
endif()

add_executable(${PROJ}
	src/main.cpp
	src/ruin.cpp
	src/sdl.cpp
	src/render/text.cpp
	src/physics.cpp
	src/game.cpp
	src/game.cpp

	src/util/material/cam.cpp
	src/util/material/hct_solver.cpp
	src/util/material/utils.cpp
	src/util/material/viewing_conditions.cpp
)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(THREADS_PREFER_PTHREAD_FLAG ON)

if (EMSCRIPTEN)
	set_target_properties(${PROJ} PROPERTIES SUFFIX ".html")
endif()

target_precompile_headers(${PROJ} PRIVATE
	<box2d/box2d.h>
	<SDL3/SDL.h>
	<SDL3/SDL_events.h>
	<SDL3/SDL_error.h>
	<SDL3_image/SDL_image.h>
	<chrono>
	<cmath>
	<cstdio>
	<cstring>
	<fmt/format.h>
	<random>
	<string>
	<vector>
)

target_link_libraries(${PROJ} PRIVATE
	box2d::box2d
	fmt::fmt
	SDL3::SDL3
	SDL3_image::SDL3_image
	spdlog::spdlog
	CLI11::CLI11
)
if (NOT EMSCRIPTEN)
target_link_libraries(${PROJ} PRIVATE
	SDL3_ttf::SDL3_ttf
)
endif()

target_include_directories(${PROJ} PUBLIC
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_BINARY_DIR}/generated
)
