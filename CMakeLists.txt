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
	src/Utils.cpp
	src/ConfigManager.cpp
	src/CacheManager.cpp
	src/TranslationManager.cpp
	src/admin/AdminServer.cpp
	src/admin/PagesHandler.cpp
)

# libs
find_package(Threads REQUIRED)

find_package(Boost COMPONENTS system filesystem graph regex REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(lib/mongoose)
add_library(mongoose
	lib/mongoose/mongoose/mongoose.c
)
target_link_libraries(mongoose ${CMAKE_THREAD_LIBS_INIT})

include_directories(lib/rapidjson/include)
add_library(rapidjson
	lib/rapidjson_dummy.cpp
)

set(LIB_LIST
	mongoose
	rapidjson
	${Boost_LIBRARIES}
)

# executable
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LIB_LIST})

# resources
execute_process(
	COMMAND ${CMAKE_COMMAND} -E remove_directory bin/res
	COMMAND ${CMAKE_COMMAND} -E copy_directory res bin/res
)
