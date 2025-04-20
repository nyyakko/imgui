# A cmake wrapper for Dear ImGui

This is a simple cmake wrapper for [ImGui](https://github.com/ocornut/imgui) I use myself. Note that this uses OpenGL + GLFW as its backend.

# installation

you may copy the files under imgui/include into your project, install it with [CPM](https://github.com/cpm-cmake/CPM.cmake) or install directly into your system with the following:

* ``py install.py``

and then include it with cmake into your project

```cmake
cmake_minimum_required_version(VERSION 3.25)

project(CoolProject LANGUAGES CXX)

find_package(imgui CONFIG REQUIRED)
find_package(liberror CONFIG REQUIRED)
add_executable(CoolProject source.cpp)
target_link_libraries(CoolProject PRIVATE imgui::imgui)
```
