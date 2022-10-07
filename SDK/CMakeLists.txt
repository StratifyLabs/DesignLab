cmake_minimum_required (VERSION 3.12)

project(DesignLab-SDK)

set(API_IS_SDK TRUE CACHE INTERNAL "Build SDK projects")
option(IS_PULL "Pull SDK projects" ON)

set(BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
file(MAKE_DIRECTORY ${BASE_DIRECTORY})

message(STATUS "Base directory ${BASE_DIRECTORY}")

macro(sdk_add_subdirectory NAME BRANCH)
	if(IS_PULL OR NOT EXISTS ${BASE_DIRECTORY}/${NAME})
		cmsdk_git_clone_or_pull_branch(${BASE_DIRECTORY} ${NAME} ssh://git@github.com/StratifyLabs/${NAME}.git ${BRANCH})
	endif()
	add_subdirectory(${BASE_DIRECTORY}/${NAME} ${NAME} EXCLUDE_FROM_ALL)
endmacro()

include(CTest)



sdk_add_subdirectory(API main)
include(${BASE_DIRECTORY}/API/libraries/API.cmake)

set(LVGL_API_USE_FREETYPE ON CACHE INTERNAL "Use LVGL Freetype")
set(LVGL_CONFIG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/config CACHE INTERNAL "LVGL config path")
set(LVGL_NO_STRATIFY_OS ON CACHE INTERNAL "Do not include dependency on Stratify OS")
set(MBEDTLS_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/config/mbedtls/config.h CACHE INTERNAL "MBEDTLS config file")

sdk_add_subdirectory(JsonAPI main)
sdk_add_subdirectory(InetAPI main)

sdk_add_subdirectory(WindowAPI main)
sdk_add_subdirectory(LvglAPI main)
sdk_add_subdirectory(DesignAPI main)

sdk_add_subdirectory(CPrinterAPI main)

