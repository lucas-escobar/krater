cmake_minimum_required(VERSION 3.5)

project(krater-app LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

set(GLFW_LIBRARY ${PROJECT_SOURCE_DIR}/lib/glfw/glfw-3.3.8.bin.WIN64/lib-vc2019/glfw3.lib CACHE FILEPATH "GLFW Library")
set(GLFW_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib/glfw/glfw-3.3.8.bin.WIN64/include CACHE PATH "GLFW Include Directory")
set(glm_DIR ${PROJECT_SOURCE_DIR}/lib/glm/cmake/glm CACHE PATH "glm Directory")
set(VK_LAYER_ENABLES VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT CACHE STRING "VK_LAYER_ENABLES Option")

find_package(Vulkan REQUIRED)
find_package(GLFW REQUIRED)
find_package(glm REQUIRED)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(krater-app ${SOURCES})

target_include_directories(krater-app PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(krater-app PRIVATE ${PROJECT_SOURCE_DIR}/lib/stb/include)
target_include_directories(krater-app PRIVATE ${PROJECT_SOURCE_DIR}/lib/tinyobjloader/include)

target_link_libraries(krater-app PRIVATE
Vulkan::Vulkan
GLFW::GLFW
glm::glm
)