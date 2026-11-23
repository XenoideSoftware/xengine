set(target "xe-core-test")

set(sources
    "DataType.Test.cpp"
)

add_executable(${target} ${sources})
target_link_libraries(${target} PRIVATE
    xe::core
    xenoide::interface
    Catch2::Catch2WithMain
)

xenoide_add_catch2_tests (${target})
