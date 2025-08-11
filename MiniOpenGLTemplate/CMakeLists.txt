cmake_minimum_required(VERSION 3.8)
project(MiniOpenGLTemplate)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCE_FILES
    MiniOpenGLTemplate.cpp
)

# Add GLAD source
set(GLAD_SRC
    src/glad.c
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${GLAD_SRC})

# Include directories for all dependencies
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/headers
)

# Link GLFW (prebuilt static library)
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/headers"
)

# Find and link OpenGL
find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
)

# Optionally, if you want to link against glad as a library (not needed here since glad.c is compiled in)
# add_library(glad STATIC dependencies/src/glad.c)
# target_link_libraries(${PROJECT_NAME} PRIVATE glad)
