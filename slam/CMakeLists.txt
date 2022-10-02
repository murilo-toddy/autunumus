cmake_minimum_required(VERSION 3.23)
project(slam)

set(CMAKE_CXX_STANDARD 17)

add_executable(slam main.cpp FastSLAM.cpp FastSLAM.h Particle.cpp Particle.h file_handler.cpp file_handler.h cylinder_handler.cpp cylinder_handler.h Robot.cpp Robot.h)
