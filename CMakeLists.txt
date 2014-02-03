project(cellsim)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
if (APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()


set(cellsim_sources
	src/atom.cpp
	src/bin.cpp
	src/simulation.cpp
)

set(cellsim_headers
	src/atom.h
	src/bin.h
	src/defs.h
	src/simulation.h
	src/util/weakset.h
)

include_directories(src)
add_executable(${PROJECT_NAME} src/main.cpp ${cellsim_sources} ${cellsim_headers})


# OpenGL dependency
#find_package(OpenGL REQUIRED)
#include_directories( ${OPENGL_INCLUDE_DIRS} )
#target_link_libraries(cellsim ${OPENGL_LIBRARIES} )

# GLEW dependency
#find_package(GLEW REQUIRED)
#include_directories( ${GLEW_INCLUDE_DIRS} )
#target_link_libraries(cellsim ${GLEW_LIBRARIES} )

# GLFW dependency
#add_subdirectory(glfw)
#include_directories(glfw/include)
#target_link_libraries(cellsim glfw ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES})

