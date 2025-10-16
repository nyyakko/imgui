cmake_minimum_required(VERSION 3.25)

project(imgui LANGUAGES CXX)

include(cmake/get_cpm.cmake)
include(GNUInstallDirs)

# Downloads source from remote directly
# CPMAddPackage("gh:repo/package1#REF")

CPMFindPackage(
    NAME glfw
    GITHUB_REPOSITORY glfw/glfw
    GIT_TAG 3.4
)

# include(cmake/static_analyzers.cmake)

# set(imgui_CompilerOptions ${imgui_CompilerOptions})
# set(imgui_LinkerOptions ${imgui_LinkerOptions})

set(imgui_ExternalLibraries
    glfw
    m
)

add_subdirectory(imgui)

