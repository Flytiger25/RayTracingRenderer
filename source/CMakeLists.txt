set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE source CONFIGURE_DEPENDS src/*.cpp)

add_executable(RayTracingRenderer ${source})

target_include_directories(RayTracingRenderer PUBLIC include)

target_link_libraries(RayTracingRenderer glm)