cmake_minimum_required(VERSION 3.25)
project(project1)

set(CMAKE_CXX_STANDARD 17)

link_libraries(fltk)
link_libraries(fltk_images)


add_executable(project1
        main.cpp
        global.hpp
        map_window.hpp
        polygondrawer.hpp
        polygondrawer.cpp
        map_window.cpp
        )
