
find_package(DevIL REQUIRED)

set (target xe-graphics-il)
set (sources 
    "src/xe/graphics/ImageLoaderIL.h"
    "src/xe/graphics/ImageLoaderIL.cpp")

add_library(${target} ${sources})
add_library(xe::graphics-il ALIAS ${target})

target_include_directories(${target} PUBLIC src)
target_link_libraries(${target} PUBLIC xe::core PUBLIC xe::graphics PRIVATE xenoide::interface)
target_link_libraries(${target} DevIL::DevIL)
