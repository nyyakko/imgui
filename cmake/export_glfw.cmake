function(export_glfw)
    export(TARGETS   glfw
           FILE      ${EXPORT_DIR}/glfwTargets.cmake
           NAMESPACE glfw::
    )
endfunction()
