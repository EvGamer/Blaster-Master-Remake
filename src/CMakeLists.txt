cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 11) # Required by Raylib

project(BlasterMasterRemake VERSION 0.0.0)

file(
	GLOB_RECURSE ALL_SRC
	RELATIVE ${PROJECT_SOURCE_DIR}
	*.cpp *.h
)
list(REMOVE_ITEM ALL_SRC "thirdParty/tinyxml2/xmltest.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${ALL_SRC})
add_executable(${PROJECT_NAME} ${ALL_SRC})
set (ASSET_DIR ${PROJECT_BINARY_DIR}/../assets)

## Add asset folder links
add_custom_command(
  TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${ASSET_DIR}/maps $<TARGET_FILE_DIR:${PROJECT_NAME}>/maps
)

add_custom_command(
  TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${ASSET_DIR}/Sprites $<TARGET_FILE_DIR:${PROJECT_NAME}>/Sprites
)

add_custom_command(
  TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${ASSET_DIR}/tilesets $<TARGET_FILE_DIR:${PROJECT_NAME}>/tilesets
)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${ASSET_DIR})

## Raylib
find_package(raylib 3.0 QUIET) # Requires at least version 3.0

if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)

  FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/master.tar.gz
  )

  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)

    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples

    # build raylib
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})

  endif()

endif()

target_link_libraries(${PROJECT_NAME} raylib)

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

## Copy index file if compile for emscripten
if (EMSCRIPTEN) 
  configure_file(
		${PROJECT_SOURCE_DIR}/wasmAppPage.html
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.html
		COPYONLY
	)
  set(COMMON_LINK_FLAGS "-s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s EXPORTED_FUNCTIONS=[\"_main\"] --preload-file ${ASSET_DIR}@/")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${COMMON_LINK_FLAGS})
  set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so taht you can directly open it.

  if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(ENV_EMCC_DEBUG 1)
	  set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${COMMON_LINK_FLAGS} -s NO_DISABLE_EXCEPTION_CATCHING -gsource-map")
  endif (CMAKE_BUILD_TYPE MATCHES Debug)
endif(EMSCRIPTEN)