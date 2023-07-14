cmake_minimum_required(VERSION 3.25)
project(ZeroBot)

if(!MSVC)
    message(FATAL_ERROR "Use MSVC to build this project.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /utf-8")

add_compile_definitions(HV_STATICLIB)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MT>)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src")

include_directories(include header)
link_directories(lib)

file(GLOB SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cc")

add_executable(${PROJECT_NAME} main/main.cc ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} hv_static libssl-1_1 libcrypto-1_1)