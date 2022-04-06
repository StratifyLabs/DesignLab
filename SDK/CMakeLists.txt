cmake_minimum_required (VERSION 3.12)

project(DesignLab-SDK)

set(API_IS_SDK TRUE CACHE INTERNAL "Build SDK projects")
option(IS_PULL "Pull SDK projects" ON)

set(BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
file(MAKE_DIRECTORY ${BASE_DIRECTORY})

message(STATUS "Base directory ${BASE_DIRECTORY}")

# refresh the bootstrap code if needed
execute_process(
	COMMAND git pull
	WORKING_DIRECTORY ${BASE_DIRECTORY}/CMakeSDK
	)

execute_process(
	COMMAND cmake --build . --target install
	WORKING_DIRECTORY ${BASE_DIRECTORY}/CMakeSDK/cmake_link
	)


macro(sdk_add_subdirectory NAME BRANCH)
	if(IS_PULL OR NOT EXISTS ${BASE_DIRECTORY}/${NAME})
		sos_sdk_git_clone_or_pull_branch(${BASE_DIRECTORY} ${NAME} ssh://git@github.com/StratifyLabs/${NAME}.git ${BRANCH})
	endif()
	add_subdirectory(${BASE_DIRECTORY}/${NAME} ${NAME} EXCLUDE_FROM_ALL)
endmacro()

include(CTest)

set(STRATIFYOS_LWIP_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/../InetAPI/lwip/include
	${CMAKE_CURRENT_SOURCE_DIR}/../InetAPI/lwip/lwip-2.1.2/src/include
	CACHE INTERNAL "LWIP INCLUDE PATHS")


sdk_add_subdirectory(API main)
include(${BASE_DIRECTORY}/API/libraries/API.cmake)

set(LVGL_API_IS_QT_SIMULATOR OFF CACHE INTERNAL "Disable QT Simulator")
set(LVGL_CONFIG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/config CACHE INTERNAL "LVGL config path")
set(LVGL_NO_STRATIFY_OS ON CACHE INTERNAL "Do not include dependency on Stratify OS")
set(MBEDTLS_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/config/mbedtls/config.h CACHE INTERNAL "MBEDTLS config file")

add_subdirectory(dependencies/CMakeSDK CMakeSDK)

sdk_add_subdirectory(JsonAPI v1.2)
sdk_add_subdirectory(InetAPI v1.2)

sdk_add_subdirectory(WindowAPI main)
sdk_add_subdirectory(LvglAPI main)
sdk_add_subdirectory(DesignAPI main)
sdk_add_subdirectory(DesignLabThemes main)

sdk_add_subdirectory(CPrinterAPI main)

