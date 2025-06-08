cmake_minimum_required(VERSION 3.25)

project(imgui LANGUAGES CXX)

# include(cmake/static_analyzers.cmake)
include(cmake/get_cpm.cmake)
include(GNUInstallDirs)

# set(imgui_CompilerOptions ${imgui_CompilerOptions})
# set(imgui_LinkerOptions ${imgui_LinkerOptions})

# Downloads source from remote directly
# CPMAddPackage("gh:repo/package1#REF")

# Tries to find package locally first, then downloads if it couldn't be found.
# CPMFindPackage(
#     NAME package2
#     GITHUB_REPOSITORY repo/package2
#     GIT_TAG REF
# )

# set(imgui_ExternalLibraries package1 package2 ...)

add_subdirectory(imgui)

