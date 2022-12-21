cmake_minimum_required(VERSION 3.23.3)

add_compile_options(-Wfatal-errors -w -g)

#find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(OpenGL)
#find_package(OpenGL REQUIRED OPTIONAL_COMPONENTS OpenGl)
#if(TARGET OpenGL::OpenGL)
#    target_link_libraries(${PROJECT_NAME} OpenGL::OpenGL)
#else()
#    target_link_libraries(${PROJECT_NAME} OpenGL::GL)
#endif()

project(App)

set(BOX_2D_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../external/box2d)

add_executable(${PROJECT_NAME} src/main.cpp
    src/App.cpp
    src/Object.cpp
    src/Object3D.cpp
    src/Quad.cpp
    src/Shader.cpp
    src/Mesh.cpp
    src/Model.cpp
    src/stb_image.cpp)

add_subdirectory(external/glfw)
add_subdirectory(external/imgui)
add_subdirectory(external/glm)

find_package(GLEW REQUIRED)

target_include_directories(${PROJECT_NAME}
    PUBLIC external/glfw/include
    PUBLIC external/imgui
    PUBLIC external/glm
    PUBLIC external/box2d/include
    PUBLIC external/assimp/include)

target_link_directories(${PROJECT_NAME}
    PRIVATE external/glfw/src
    PRIVATE external/imgui
    PRIVATE external/glm
    PRIVATE external/box2d/build/bin
    PRIVATE external/assimp/bin)

include_directories(${GLEW_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE glfw ${GLFW_LIBRARIES}
    GLEW
    imgui
    glm::glm
    m
    OpenGL
    box2d
    assimp)
