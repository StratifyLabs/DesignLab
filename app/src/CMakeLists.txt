
set(SOURCES
	main.cpp
	Application.cpp
	Application.hpp

	logic/AssetManager.cpp
	logic/AssetManager.hpp
	logic/BootstrapIconManager.cpp
	logic/BootstrapIconManager.hpp
	logic/CodePrinter.hpp
	logic/FontAwesomeManager.cpp
	logic/FontAwesomeManager.hpp
	logic/FontManager.cpp
	logic/FontManager.hpp
	logic/Manager.cpp
	logic/Manager.hpp
	logic/StringPrinter.hpp
	logic/ThemeManager.cpp
	logic/ThemeManager.hpp

	worker/ExportWorker.cpp
	worker/ExportWorker.hpp
	worker/WorkerObject.cpp
	worker/WorkerObject.hpp

	model/Model.hpp
	model/Settings.cpp
	model/Settings.hpp

	view/AssetMaker.cpp
	view/AssetMaker.hpp
	view/FontMaker.cpp
	view/FontMaker.hpp
	view/ColorPreview.cpp
	view/ColorPreview.hpp
	view/Editor.cpp
	view/Editor.hpp
	view/Home.cpp
	view/Home.hpp
	view/IconMaker.cpp
	view/IconMaker.hpp
	view/ExportModal.cpp
	view/ExportModal.hpp
	view/Project.cpp
	view/Project.hpp
	view/ThemeMaker.cpp
	view/ThemeMaker.hpp
	view/ThemePreview.cpp
	view/ThemePreview.hpp
	view/ViewObject.hpp
	view/extras/Extras.cpp
	view/extras/Extras.hpp


	designlab/bootstrap.c
	designlab/fontawesome_brands_regular.c
	designlab/fontawesome_regular.c
	designlab/fontawesome_solid.c
	designlab/icon_fonts.h

	PARENT_SCOPE)
