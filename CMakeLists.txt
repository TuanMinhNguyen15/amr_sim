cmake_minimum_required(VERSION 3.10)

project(autonomous_road_vehicle_simulator)

# Set C++ Standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)
find_package(Threads REQUIRED)
find_package(PNG REQUIRED)

# AMR Simulator
add_executable(amr_sim  "src/amr_sim/base.cc"
                        "src/amr_sim/amr_sim.cc"
                        "src/amr_sim/olcPixelGameEngine.cc")
target_link_libraries(amr_sim   ${OpenGL_LIBRARIES} OpenGL::GL
                                X11::X11
                                Threads::Threads
                                PNG::PNG
                                stdc++fs)
target_include_directories(amr_sim PRIVATE "include/")

# Map Creator
add_executable(map_creator  "src/amr_sim/base.cc"
                            "src/amr_sim/map_creator.cc"
                            "src/amr_sim/shapes.cc"
                            "src/amr_sim/olcPixelGameEngine.cc")
target_link_libraries(map_creator   ${OpenGL_LIBRARIES} OpenGL::GL
                                    X11::X11
                                    Threads::Threads
                                    PNG::PNG
                                    stdc++fs)
target_include_directories(map_creator PRIVATE "include/")

include_directories(${OpenGL_INCLUDE_DIRS})
include_directories(${X11_INCLUDE_DIRS})
include_directories(${Threads_INCLUDE_DIRS})
include_directories(${PNG_INCLUDE_DIRS})

add_executable(test "test.cc")