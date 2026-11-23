
set (target "xe-scene-test")
set (sources
    "src/Projection.Test.cpp"
    "src/Trackball.Test.cpp"
    "src/VirtualSphere.Test.cpp"
)

add_executable(${target} ${sources})
target_link_libraries(${target} PRIVATE Catch2::Catch2WithMain xe::scene xenoide::interface)

xenoide_add_catch2_tests (${target})
