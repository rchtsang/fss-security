##################################################
# project: unicorn-tutorial
# root-level cmakelists.txt
##################################################

cmake_minimum_required(VERSION 3.16)

project(unicorn-tutorial VERSION 0.1.0 LANGUAGES CXX C)

# declare include and link directories
include_directories(/usr/include 
	${PROJECT_SOURCE_DIR}/inc
	${PROJECT_SOURCE_DIR}/src)
link_directories(/usr/lib /usr/lib64 /lib/x86_64-linux-gnu/)

# declare file lists
file(GLOB src_files
	"${PROJECT_SOURCE_DIR}/src/*")
file(GLOB inc_files
	"${PROJECT_SOURCE_DIR}/inc/*")

# add libraries
add_library(systemc INTERFACE)
target_sources(systemc INTERFACE /usr/include/systemc.h)
add_library(unicorn INTERFACE)
target_sources(unicorn INTERFACE /usr/include/unicorn/unicorn.h)

add_executable(main main.cpp ${src_files} ${inc_files})

# link libraries
target_link_libraries(main 
	INTERFACE systemc PUBLIC /usr/lib/libsystemc.a 
	INTERFACE unicorn PUBLIC /usr/lib64/libunicorn.a
	pthread)

set(CMAKE_CXX_COMPILER "clang++")