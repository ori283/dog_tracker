cmake_minimum_required(VERSION 3.10)
project(dog_tracker)

# Set compiler
set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always -g")

# Set OpenCV paths
set(OpenCV_DIR "C:/opencvSrc/opencv-4.11.0/build/install")
set(OpenCV_INCLUDE_DIRS "${OpenCV_DIR}/include")
set(OpenCV_LIBRARIES 
    "${OpenCV_DIR}/x64/mingw/lib/libopencv_world4110.dll.a"
    "${OpenCV_DIR}/x64/mingw/lib/libopencv_tracking4110.dll.a"
)

# Set MinGW paths
set(MINGW_DIR "C:/msys64/mingw64")
set(MINGW_INCLUDE_DIRS "${MINGW_DIR}/include")
set(MINGW_LIBRARIES "${MINGW_DIR}/lib")

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
    ${MINGW_INCLUDE_DIRS}
)

# Link directories
link_directories(
    ${OpenCV_DIR}/x64/mingw/lib
    ${MINGW_LIBRARIES}
)

# Source files
set(SOURCES
    source/main.cpp
    source/cameraController.cpp
    source/circularBuffer.cpp
    source/producerConsumer.cpp
    source/configLoader.cpp
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    opencv_world4110
    opencv_tracking4110
    pthread
)

# Set output directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../bin)

# Copy OpenCV DLLs to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OpenCV_DIR}/x64/mingw/bin/libopencv_world4110.dll"
        "${OpenCV_DIR}/x64/mingw/bin/libopencv_tracking4110.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
) 