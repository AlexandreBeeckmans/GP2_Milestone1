cmake_minimum_required(VERSION 3.10)

# Project Name
project(VulkanLab01)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8 # Specify the version you need
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        bf71a834948186f4097caa076cd2663c69a10e1e
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
	stb
	GIT_REPOSITORY https://github.com/nothings/stb.git
	GIT_TAG master
)
FetchContent_MakeAvailable(stb)

FetchContent_Declare(
	json
	GIT_REPOSITORY https://github.com/nlohmann/json.git
	GIT_TAG v3.11.3
	GIT_SHALLOW 1
	GIT_PROGRESS 1
)
# Make the fetched content available
FetchContent_MakeAvailable(json)


# Find the required packages
find_package(Vulkan REQUIRED)

# Include Directories
include_directories(${Vulkan_INCLUDE_DIRS})

add_subdirectory(Project)


# If using validation layers, copy the required JSON files (optional)
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${CMAKE_SOURCE_DIR}/path_to_json/VK_LAYER_KHRONOS_validation.json"
#        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
