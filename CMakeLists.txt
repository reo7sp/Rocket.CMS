cmake_minimum_required(VERSION 3.0.0)
project(Rocket.CMS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/package)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/package)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/package)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

### sources
set(SRC_LIST
    src/cpp/rcms/CoreApp.cpp
    src/cpp/rcms/CacheManager.h
    src/cpp/rcms/tools/FsTools.h
    src/cpp/rcms/tools/StringTools.h
    src/cpp/rcms/tools/NetTools.h
    src/cpp/rcms/web/WebHandlerFactory.cpp
    src/cpp/rcms/web/IndexWebHandler.cpp
    src/cpp/rcms/web/ApiWebHandler.cpp
    src/cpp/rcms/web/api/AbstractApiHandler.h
    src/cpp/rcms/web/api/FsApiHandler.cpp
    src/cpp/rcms/web/api/ConfApiHandler.cpp
    src/cpp/rcms/web/api/WebguiApiHandler.cpp
)

### libs
set(CMAKE_FIND_LIBRARY_PREFIXES lib cyg)
set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .dll .so .a .dll.a .30.dll.a .30.dll)

include_directories("${PROJECT_SOURCE_DIR}/build/libinclude")

# poco
set(POCO_LIBS "")
foreach(LIB PocoFoundation PocoUtil)
	find_library(POCO_${LIB} ${LIB} PATHS "${PROJECT_SOURCE_DIR}/build/lib/Poco" REQUIRED)
	list(APPEND POCO_LIBS ${POCO_${LIB}})
endforeach(LIB)

set(LIB_LIST
	${POCO_LIBS}
)

# executable
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LIB_LIST})

