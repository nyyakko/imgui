cmake_minimum_required(VERSION 3.25)

project(imgui LANGUAGES CXX)

# include(cmake/static_analyzers.cmake)
include(cmake/get_cpm.cmake)
include(GNUInstallDirs)

# set(imgui_CompilerOptions ${imgui_CompilerOptions})
# set(imgui_LinkerOptions ${imgui_LinkerOptions})

# Downloads source from remote directly
# CPMAddPackage("gh:repo/package1#REF")

CPMAddPackage(
    NAME glfw_
    GITHUB_REPOSITORY glfw/glfw
    GIT_TAG 3.4
)

set(imgui_ExternalLibraries
    glfw_
)

add_subdirectory(imgui)

