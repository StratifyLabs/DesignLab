


find_package(QT
	NAMES Qt6
	COMPONENTS Core REQUIRED)

find_package(Qt${QT_VERSION_MAJOR}
	COMPONENTS
	Quick
	Widgets
	REQUIRED
	)

set(LOCAL_NAME ${PROJECT_NAME})

qt6_add_resources(QT_RESOURCES src/qml.qrc)

sos_sdk_add_subdirectory(PRIVATE_SOURCELIST ${CMAKE_CURRENT_SOURCE_DIR}/src)
sos_sdk_add_subdirectory(FONT_SOURCELIST ${CMAKE_CURRENT_SOURCE_DIR}/src/fonts)
sos_sdk_app_target(RELEASE ${LOCAL_NAME} "" release ${SOS_ARCH})

set(RELEASE_TARGET_NAME DesignLab)
add_executable(${RELEASE_TARGET_NAME} MACOSX_BUNDLE)
target_sources(${RELEASE_TARGET_NAME}
	PRIVATE
	${PRIVATE_SOURCELIST}
	${FONT_SOURCELIST}
	${QT_RESOURCES}
	)

target_compile_definitions(${RELEASE_TARGET_NAME}
	PRIVATE
	-DVERSION=\"${CMAKE_PROJECT_VERSION}\"
	)

target_include_directories(${RELEASE_TARGET_NAME}
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
	)

target_compile_options(${RELEASE_TARGET_NAME}
	PRIVATE
	-O3
	)

set_target_properties(${RELEASE_TARGET_NAME}
	PROPERTIES
		BUNDLE TRUE
		MACOSX_BUNDLE_GUI_IDENTIFIER stratifylabs.dev.style.identifier.${CUR_TARGET}
		MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
		MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
		MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
		MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/MacOSXBundleInfo.plist.in
)

set_property(TARGET ${RELEASE_TARGET_NAME} PROPERTY CXX_STANDARD 17)


target_link_libraries(${RELEASE_TARGET_NAME} PUBLIC
	lvgl_qt
	lvgl_release_link
	JsonAPI_release_link
	)

set(MAC_DEPLOY_QT ${CMAKE_PREFIX_PATH}/bin/macdeployqt)

get_target_property(BUNDLE_DIRECTORY ${RELEASE_TARGET_NAME} BINARY_DIR)

add_custom_target(deploy
	COMMAND ${MAC_DEPLOY_QT} ${BUNDLE_DIRECTORY}/${RELEASE_TARGET_NAME}.app -codesign=4ZXQ5PR398)


