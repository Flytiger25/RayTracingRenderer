set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE source CONFIGURE_DEPENDS src/*.cpp)

add_executable(RayTracingRenderer ${source})

target_include_directories(RayTracingRenderer PUBLIC include)
target_compile_definitions(RayTracingRenderer PUBLIC GLM_FORCE_DEPTH_ZERO_TO_ONE GLM_FORCE_RADIANS GLM_FORCE_LEFT_HAND)

target_link_libraries(RayTracingRenderer glm)