set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(imgui_SourceFiles ${imgui_SourceFiles}
    "${DIR}/imgui_bezier.cpp"
    "${DIR}/imgui_toast.cpp"
    "${DIR}/imgui_text.cpp"

    PARENT_SCOPE
)

