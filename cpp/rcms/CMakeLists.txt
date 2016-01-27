### sources
include_directories(include)
set(RCMS_SRC_LIST
    src/Launcher.cpp
    src/managers/PluginManager.cpp
    src/managers/TranslationManager.cpp
    src/web/WebHandlerFactory.cpp
    src/web/AbstractWebHandler.cpp
    src/web/IndexWebHandler.cpp
    src/web/WebguiWebHandler.cpp
    src/web/CmsConfWebHandler.cpp
    src/web/CmsTranslationsWebHandler.cpp
    src/web/SiteFsWebHandler.cpp
    src/tools/FsTools.cpp
    src/tools/StringTools.cpp
)

### libs

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