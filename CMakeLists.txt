cmake_minimum_required(VERSION 2.8)
project(Rocket.CMS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# sources
set(SRC_LIST
	src/Core.cpp
	src/Log.cpp
	src/ConfigManager.cpp
	src/CacheManager.cpp
	src/TranslationManager.cpp
	src/admin/AdminServer.cpp
	src/admin/PagesHandler.cpp
)

# libs
find_package(Threads REQUIRED)

include_directories(lib/mongoose)
add_library(mongoose
	lib/mongoose/mongoose/mongoose.c
)
target_link_libraries(mongoose ${CMAKE_THREAD_LIBS_INIT})

include_directories(lib/rapidjson/include)
add_library(rapidjson
	lib/rapidjson_dummy.cpp
)

include_directories(lib/boost/predef/include)
add_library(boost_predef
	lib/boost_predef_dummy.cpp
)

include_directories(lib/boost/system/include)
add_library(boost_system
	lib/boost/system/src/error_code.cpp
)

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
	rapidjson
	boost_predef
	boost_system
	boost_filesystem
	boost_algorithm
)

# executable
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LIB_LIST})

# resources
execute_process(
	COMMAND ${CMAKE_COMMAND} -E remove_directory bin/res
	COMMAND ${CMAKE_COMMAND} -E copy_directory res bin/res
)
