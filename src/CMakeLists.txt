cmake_minimum_required(VERSION 3.16)

project(BlasterMasterRemake VERSION 0.0.0)

add_executable(BlasterMasterRemake main.cpp)

FILE(GLOB SUB_SOURCES *.cpp)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SUB_SOURCES)

##################################################

find_package(OpenGL REQUIRED)
if(NOT OpenGL_OpenGL_FOUND)
	message(SEND_ERROR "Failed to find OpenGL")
	return()
elseif(NOT OPENGL_GLU_FOUND)
    message(SEND_ERROR "Failed to find GLU")
    return()
else()
	include_directories(${OPENGL_INCLUDE_DIR})
endif()

###################################################

find_library(GDIP gdiplus)


include_directories(${PROJECT_SOURCE_DIR}/../libs)