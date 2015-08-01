### sources
include_directories(include)
set(RCMS_SRC_LIST
    src/CoreApp.cpp
    src/CacheManager.cpp
    src/PluginManager.cpp
    src/FsEvent.cpp
    src/TranslationManager.cpp
    src/api/ApiManager.cpp
    src/api/FsApiHandler.cpp
    src/api/ConfApiHandler.cpp
    src/api/WebguiApiHandler.cpp
    src/web/WebHandlerFactory.cpp
    src/web/IndexWebHandler.cpp
    src/web/ApiWebHandler.cpp
    src/pluginapi/rcms_Core.cpp
    src/pluginapi/rcms_FsEvent.cpp
    src/tools/FsTools.cpp
    src/tools/StringTools.cpp
    src/tools/NetTools.cpp
)

### libs
include_directories(${PROJECT_SOURCE_DIR}/build/libinclude)

# poco
set(RCMS_POCO_LIBS "")
foreach(LIB Foundation Util Net JSON)
	if(LIB STREQUAL "Foundation")
		find_path(RCMS_POCO_${LIB}_INCLUDE_DIR Poco/${LIB}.h)
	else()
		find_path(RCMS_POCO_${LIB}_INCLUDE_DIR Poco/${LIB}/${LIB}.h)
	endif()
    find_library(RCMS_POCO_${LIB}_LIB Poco${LIB} REQUIRED)
    message(STATUS "Poco${LIB} include dir is ${RCMS_POCO_${LIB}_INCLUDE_DIR}")
    message(STATUS "Poco${LIB} lib is ${RCMS_POCO_${LIB}_LIB}")
	include_directories(${RCMS_POCO_${LIB}_INCLUDE_DIR})
    list(APPEND RCMS_POCO_LIBS ${RCMS_POCO_${LIB}_LIB})
endforeach(LIB)

set(RCMS_LIB_LIST
    ${RCMS_POCO_LIBS}
)

### result
add_library(rcms ${RCMS_SRC_LIST})
target_link_libraries(rcms ${RCMS_LIB_LIST})

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} rcms)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(TARGETS rcms DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/)