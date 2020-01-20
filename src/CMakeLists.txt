cmake_minimum_required(VERSION 3.16)

project(BlasterMasterRemake VERSION 0.0.0)

file(
	GLOB_RECURSE ALL_SRC
	RELATIVE ${PROJECT_SOURCE_DIR}
	*.cpp *.h
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${ALL_SRC})
add_executable(BlasterMasterRemake WIN32 ${ALL_SRC})
set_property(TARGET BlasterMasterRemake PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/assets")

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

include_directories("${PROJECT_SOURCE_DIR}/../libs")