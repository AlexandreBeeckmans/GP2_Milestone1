# Source files
add_custom_target(copy_resources ALL)
add_custom_command(
    TARGET copy_resources PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)

add_custom_target(copy_textures ALL)
add_custom_command(
    TARGET copy_textures PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/textures
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/textures)

add_custom_target(copy_scenes ALL)
add_custom_command(
    TARGET copy_scenes PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/scenes
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/scenes)


set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(SHADER_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${SHADER_SOURCE_DIR}/*.frag"
    "${SHADER_SOURCE_DIR}/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${SHADER_BINARY_DIR}/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL}
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
)




set(SOURCES
    "main.cpp"
    "vulkanbase/VulkanBase.h" 
    "vulkanbase/VulkanUtil.h"
    "vulkanbase/VulkanUtil.cpp"
    # Add other source files here
    "labwork/Week01.cpp"
    "labwork/Week02.cpp" 
    "labwork/Week03.cpp" 
    "labwork/Week04.cpp" 
    "labwork/Week05.cpp"
    "labwork/Week06.cpp"
    "HelperClasses/GP2Shader.h"
    "HelperClasses/GP2Shader.cpp" 
    "HelperClasses/GP2CommandPool.h" 
    "HelperClasses/GP2CommandPool.cpp"
    "HelperClasses/GP2CommandBuffer.h"
    "HelperClasses/GP2CommandBuffer.cpp"
    "HelperClasses/Vertex.h"
    "HelperClasses/GP2Mesh.h"
    "HelperClasses/GP2Mesh.cpp"
    "HelperClasses/GP2Scene.h"
    "HelperClasses/GP2Scene.cpp"
    "HelperClasses/GP2CircleMesh.h"
    "HelperClasses/GP2CircleMesh.cpp"
    "HelperClasses/GP2RectangleMesh.h"
    "HelperClasses/GP2RectangleMesh.cpp"
    "HelperClasses/GP2RoundedRectangleMesh.h"
    "HelperClasses/GP2RoundedRectangleMesh.cpp"
    "HelperClasses/GP2DataBuffer.h"
    "HelperClasses/GP2DataBuffer.cpp"
    "HelperClasses/GP2DescriptorPool.cpp"
    "HelperClasses/GP2DescriptorPool.h"
    "HelperClasses/GP2Pipeline.cpp"
    "HelperClasses/GP2Pipeline.h"
    "HelperClasses/GP2CubeMesh.h"
    "HelperClasses/GP2CubeMesh.cpp"
    "HelperClasses/OBJLoader.h"
    "HelperClasses/GP23DMesh.h"
    "HelperClasses/GP23DMesh.cpp"
    "HelperClasses/GP2Scene2D.cpp" 
    "HelperClasses/GP2Scene2D.h"
    "HelperClasses/GP2Scene3D.h"
    "HelperClasses/GP2Scene3D.cpp"
    "HelperClasses/GP2Camera.cpp"
    "HelperClasses/GP2Camera.h" 
    "HelperClasses/GP2Image.h" 
    "HelperClasses/GP2Image.cpp"
    "HelperClasses/GP2SphereMesh.h"
    "HelperClasses/GP2SphereMesh.cpp"
    "SceneLoader.cpp")

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${GLSL_SOURCE_FILES} )
add_dependencies(${PROJECT_NAME} Shaders)
#add_dependencies(${PROJECT_NAME} resources)

# Link libraries
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} glm)
target_include_directories(${PROJECT_NAME} PRIVATE ${stb_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)



