cmake_minimum_required(VERSION 3.20)

project(helloMaya)

if(NOT MSVC)
  message(FATAL_ERROR "Maya Plugins require MSVC on Windows. Switch to Visual Studio")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

set(MAYA_LOCATION "C:/Program Files/Autodesk/Maya2023")

if (NOT EXISTS "${MAYA_LOCATION}")
  message(FATAL_ERROR "Maya not found at ${MAYA_LOCATION}, please set MAYA_LOCATION to the correct path!!!")
endif()

set(PROJECT_INCLUDE_DIR 
${PROJECT_SOURCE_DIR}/src
)

include_directories(
    ${PROJECT_INCLUDE_DIR}
)
# Set the source files
set(SOURCES
    src/hello_maya.cpp
)

# Set the header files
set(HEADERS
    src/hello_maya.h
)

find_package(Maya REQUIRED)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})