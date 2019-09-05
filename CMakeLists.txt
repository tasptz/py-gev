cmake_minimum_required(VERSION 3.13)

project(pygev)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(${PYBIND11_SRC})

pybind11_add_module(pygev MODULE src/api.cpp src/camera.cpp src/camera.h)
target_include_directories(pygev SYSTEM BEFORE PRIVATE "$ENV{GIGEV_DIR}/include")
target_link_directories(pygev PRIVATE "$ENV{GIGEV_DIR}/lib")
target_link_libraries(pygev PRIVATE GevApi)
install(TARGETS pygev DESTINATION gev)
