set(DEPENDENCIES
	DesignAPI
	WindowAPI
	CPrinterAPI
	InetAPI
	JsonAPI
	TestAPI
	SysAPI
	FsAPI
	VarAPI
	ThreadAPI)
set(RELEASE_TARGET DesignLab)
set(ICON_FILE  icon.icns)
set(ICON_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/../designlab/assets/${ICON_FILE})
add_executable(${RELEASE_TARGET} MACOSX_BUNDLE ${ICON_PATH})
cmsdk2_add_sources(
	TARGET ${RELEASE_TARGET}
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
cmsdk2_add_sources(
	TARGET ${RELEASE_TARGET}
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/designlab)
target_compile_definitions(${RELEASE_TARGET}
	PRIVATE
	-DVERSION=\"${CMAKE_PROJECT_VERSION}\"
	__link)
target_include_directories(${RELEASE_TARGET}
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(${RELEASE_TARGET}
	PRIVATE
	-O3)
set_property(TARGET ${RELEASE_TARGET} PROPERTY CXX_STANDARD 17)
foreach(DEPENDENCY ${DEPENDENCIES})
	message(STATUS "SOS SDK Adding dependency ${DEPENDENCY}_release_link to ${RELEASE_TARGET}")
	target_link_libraries(${RELEASE_TARGET}
		PRIVATE
		${DEPENDENCY}_release_link
		)
endforeach()
if(CMSDK_IS_LINUX)
	target_link_libraries(${RELEASE_TARGET} PRIVATE pthread)
endif()
set(BINARY_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build_release)
set_target_properties(${RELEASE_TARGET}
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIR})
set_source_files_properties(${ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
set_target_properties(${RELEASE_TARGET}
	PROPERTIES
	BUNDLE True
	MACOSX_BUNDLE_GUI_IDENTIFIER stratifylabs.dev.style.identifier.${CUR_TARGET}
	MACOSX_BUNDLE_BUNDLE_NAME "Design Lab"
	MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
	MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
	MACOSX_BUNDLE_ICON_FILE ${ICON_FILE}
	MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/MacOSXBundleInfo.plist.in)
message(STATUS "Code signing identify is $ENV{CODESIGN_IDENTITY}")
add_custom_target(sign
	COMMAND codesign -s $ENV{CODESIGN_IDENTITY} ${BINARY_OUTPUT_DIR}/${RELEASE_TARGET}.app
	VERBATIM)
add_custom_target(verify
	COMMAND spctl --assess -vvvv ${BINARY_OUTPUT_DIR}/${RELEASE_TARGET}.app)

