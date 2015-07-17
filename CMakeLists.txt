cmake_minimum_required(VERSION 3.0.0)
project(Rocket.CMS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g -fpermissive")

### sources
include_directories(src/cpp)
set(SRC_LIST
    src/cpp/rcms/TranslationManager.cpp
    src/cpp/rcms/CoreApp.cpp
    src/cpp/rcms/CacheManager.cpp
    src/cpp/rcms/PluginManager.cpp
    src/cpp/rcms/FsEvent.cpp
    src/cpp/rcms/api/ApiManager.cpp
    src/cpp/rcms/api/FsApiHandler.cpp
    src/cpp/rcms/api/ConfApiHandler.cpp
    src/cpp/rcms/api/WebguiApiHandler.cpp
    src/cpp/rcms/web/WebHandlerFactory.cpp
    src/cpp/rcms/web/IndexWebHandler.cpp
    src/cpp/rcms/web/ApiWebHandler.cpp
    src/cpp/rcms/pluginapi/rcms_Core.cpp
    src/cpp/rcms/pluginapi/rcms_FsEvent.cpp
    src/cpp/rcms/tools/FsTools.cpp
    src/cpp/rcms/tools/StringTools.cpp
    src/cpp/rcms/tools/NetTools.cpp
)

### libs
include_directories(build/libinclude)

set(CMAKE_FIND_LIBRARY_PREFIXES lib cyg)
set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .dll .so .a .dll.a .30.dll.a .30.dll)

# poco
set(POCO_LIBS "")
foreach(LIB PocoFoundation PocoUtil PocoNet PocoJSON)
    find_library(POCO_${LIB} ${LIB} PATHS build/lib/Poco REQUIRED)
    list(APPEND POCO_LIBS ${POCO_${LIB}})
endforeach(LIB)

set(LIB_LIST
    ${POCO_LIBS}
)

# executable
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LIB_LIST})
