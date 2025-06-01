add_subdirectory(extensions)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(imgui_HeaderFiles ${imgui_HeaderFiles}
    "${DIR}/imstb_truetype.hpp"
    "${DIR}/imstb_textedit.hpp"
    "${DIR}/imstb_rectpack.hpp"
    "${DIR}/imgui_internal.hpp"
    "${DIR}/imgui_impl_opengl3_loader.hpp"
    "${DIR}/imgui_impl_opengl3.hpp"
    "${DIR}/imgui_impl_glfw.hpp"
    "${DIR}/imgui.hpp"
    "${DIR}/imconfig.hpp"

    PARENT_SCOPE
)

