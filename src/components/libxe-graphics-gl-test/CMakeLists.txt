set (target "libxe-graphics-test")

set (sources
    "src/XEGraphicsGLUnitTest.cpp"
)

add_executable(${target} ${sources})
target_link_libraries(${target} PRIVATE Catch2::Catch2WithMain xe::graphics xenoide::interface)

xenoide_add_catch2_tests (${target})
