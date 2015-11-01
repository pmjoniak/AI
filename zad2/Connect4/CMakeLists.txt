cmake_minimum_required(VERSION 2.8)
project(Connect4 C CXX)

#-----------------------------------------------------------------------------
# CMake
#-----------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(AUX_CXX_FLAGS "-std=c++14 -Wall -Wextra -Wno-switch -Wno-reorder -pthread")
	set(CMAKE_CXX_FLAGS "${AUX_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s")
	message("\nCXX_FLAGS = ${CMAKE_CXX_FLAGS}\n")

	set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/deploy")
endif()

# set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_BUILD_TYPE "MinSizeRel")
 set(CMAKE_BUILD_TYPE "Release")
# set(CMAKE_BUILD_TYPE "RelWithDebInfo")


#-----------------------------------------------------------------------------
# find_package()
#-----------------------------------------------------------------------------
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})


#-----------------------------------------------------------------------------
# include_directories()
#-----------------------------------------------------------------------------
include_directories("include")
include_directories("src")


#-----------------------------------------------------------------------------
# ${SRC}
#-----------------------------------------------------------------------------
aux_source_directory("src" SRC)


#-----------------------------------------------------------------------------
# ${LIBS}
#-----------------------------------------------------------------------------
set(LIBS ${LIBS} "GL")
set(LIBS ${LIBS} "SM")

set(LIBS ${LIBS} "${PROJECT_SOURCE_DIR}/lib/libglfw3.a")

set(LIBS ${LIBS} "X11")
set(LIBS ${LIBS} "Xcursor")
set(LIBS ${LIBS} "Xext")
set(LIBS ${LIBS} "Xi")
set(LIBS ${LIBS} "Xinerama")
set(LIBS ${LIBS} "Xrandr")
set(LIBS ${LIBS} "Xxf86vm")
set(LIBS ${LIBS} "dl")


#set(LIBS ${LIBS} "${PROJECT_SOURCE_DIR}/lib64/libglfw3.a")


#-----------------------------------------------------------------------------
# add_executable()
#-----------------------------------------------------------------------------
add_executable(Connect4 ${SRC})
target_link_libraries(Connect4 ${LIBS})



message("\nSRC = ${SRC}\n\nLIBS = ${LIBS}\n")

