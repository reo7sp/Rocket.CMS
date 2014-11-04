cmake_minimum_required(VERSION 2.8)
project(Rocket.CMS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# sources
include_directories(src)
set(SRC_LIST
	src/Core.cpp
	src/admin/AdminServer.cpp
)

# libs
find_package(Threads REQUIRED)

include_directories(lib/mongoose)
add_library(mongoose
	lib/mongoose/mongoose/mongoose.c
)
target_link_libraries(mongoose ${CMAKE_THREAD_LIBS_INIT})

include_directories(lib/boost/filesystem/include)
add_library(boost_filesystem
	lib/boost/filesystem/src/path.cpp
	lib/boost/filesystem/src/portability.cpp
	lib/boost/filesystem/src/codecvt_error_category.cpp
	lib/boost/filesystem/src/unique_path.cpp
	lib/boost/filesystem/src/utf8_codecvt_facet.cpp
	lib/boost/filesystem/src/windows_file_codecvt.cpp
	lib/boost/filesystem/src/operations.cpp
	lib/boost/filesystem/src/path_traits.cpp
)

include_directories(lib/boost/algorithm/include)
add_library(boost_algorithm
	lib/boost_algorithm_dummy.cpp
)

set(LIB_LIST
	mongoose
	boost_algorithm
	boost_filesystem
)

# executable
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LIB_LIST})
