cmake_minimum_required(
    VERSION 3.20.0
)

project(graph2d)

set(CMAKE_CXX_STANDARD 20)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")
set(SRCS "main.cpp" "lexer.cpp" "eval.cpp" "parser.cpp" "renderer.cpp" "graph.cpp")
list(TRANSFORM SRCS PREPEND ${SRC_DIR})

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})

# GLFW 3
set(GLFW3_PATH "D:/DevTools/glfw3")

target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW3_PATH}/include")
target_link_directories(${PROJECT_NAME} PRIVATE "${GLFW3_PATH}/lib-vc2019")
target_link_libraries(${PROJECT_NAME} PRIVATE "glfw3.lib" "opengl32.lib")

# GLAD
set(GLAD_PATH "D:/DevTools/glad")

target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_PATH}/include")
target_sources(${PROJECT_NAME} PRIVATE "${GLAD_PATH}/src/glad.c")

