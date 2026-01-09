cmake_minimum_required(VERSION 4.0.0)

project(ruin VERSION 1.2)
set(PROJ ruin)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MAKE_PROGRAM_ARGS "--no-print-directory")
set(VCPKG_VERBOSE OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
	add_compile_options(/utf-8)
    set(BUILD_SHARED_LIBS ON)
else()
    set(BUILD_SHARED_LIBS OFF)
endif()

include(cmake/build-info.cmake)

find_package(fmt CONFIG REQUIRED QUIET)
find_package(spdlog CONFIG REQUIRED QUIET)
find_package(CLI11 CONFIG REQUIRED QUIET)
find_package(Protobuf CONFIG REQUIRED QUIET)
find_package(SDL3 CONFIG REQUIRED QUIET)
find_package(SDL3_image CONFIG REQUIRED QUIET)
if (NOT EMSCRIPTEN)
find_package(SDL3_ttf CONFIG REQUIRED QUIET)
endif()

set(PROTO_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/pb)
file(MAKE_DIRECTORY ${PROTO_OUT_DIR})

add_library(pb OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/pose.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/asset.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/map.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/manifest.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/monster.proto
)

target_include_directories(pb PUBLIC
    ${PROTO_OUT_DIR}
    ${Protobuf_INCLUDE_DIRS}
)

protobuf_generate(
    TARGET pb
    PROTOS ${CMAKE_CURRENT_SOURCE_DIR}/proto/manifest.proto
    IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/proto
    PROTOC_OUT_DIR ${PROTO_OUT_DIR}
    LANGUAGE cpp
    APPEND_PATH
)

add_executable(${PROJ}
	src/main.cpp
	src/ruin.cpp
	src/sdl.cpp

	src/util/material/cam.cpp
	src/util/material/hct_solver.cpp
	src/util/material/utils.cpp
	src/util/material/viewing_conditions.cpp

    $<TARGET_OBJECTS:pb>
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_options(${PROJ} PRIVATE
		-Wall -Wextra -Werror
		-fsanitize=undefined
		-fno-omit-frame-pointer
	)
	target_link_options(${PROJ} PRIVATE
		-fsanitize=undefined
		-fno-omit-frame-pointer
		-static-libasan
	)
endif()

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(THREADS_PREFER_PTHREAD_FLAG ON)

if (EMSCRIPTEN)
	set_target_properties(${PROJ} PROPERTIES SUFFIX ".html")
endif()

target_link_libraries(${PROJ} PRIVATE
	fmt::fmt
	SDL3::SDL3
	SDL3_image::SDL3_image
	spdlog::spdlog
	CLI11::CLI11
	protobuf::libprotobuf
)
if (NOT EMSCRIPTEN)
target_link_libraries(${PROJ} PRIVATE
	SDL3_ttf::SDL3_ttf
)
endif()

target_include_directories(${PROJ} PUBLIC
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_BINARY_DIR}/generated
	${PROTO_OUT_DIR}
    ${Protobuf_INCLUDE_DIRS}
)
