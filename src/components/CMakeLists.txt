
add_subdirectory ("libxe-core")
add_subdirectory ("libxe-math")
add_subdirectory ("libxe-scene")
add_subdirectory ("libxe-geometry")
add_subdirectory ("libxe-mesh")
add_subdirectory ("libxe-graphics")

if (XE_ENABLE_ENGINE_TOOL_GLTFV)
    add_subdirectory ("libxe-imgui")
endif()

if (XE_ENABLE_ENGINE_PLUGIN_PNG)
    add_subdirectory("libxe-graphics-png")
endif ()

if (XE_ENABLE_ENGINE_PLUGIN_DEVIL)
    add_subdirectory ("libxe-graphics-il")
endif ()

if (XE_ENABLE_ENGINE_PLUGIN_GL_GLFW AND (XE_ENABLE_DEMOS OR XE_ENABLE_ENGINE_TOOL_GLTFV))
    add_subdirectory ("libxe-graphics-gl")
    add_subdirectory ("libxe-graphics-gl3")
endif()

if (XE_ENABLE_TESTING)
    set(CMAKE_FOLDER "Engine/Tests")
    add_subdirectory ("libxe-core-test")
    add_subdirectory ("libxe-geometry-test")
    add_subdirectory ("libxe-graphics-gl-test")
    add_subdirectory ("libxe-math-test")
    add_subdirectory ("libxe-scene-test")
    set(CMAKE_FOLDER "Engine")
endif()
