
set(SOURCES
	main.cpp
	Application.cpp
	Application.hpp

	model/Model.hpp
	model/Settings.cpp
	model/Settings.hpp

	view/AssetMaker.cpp
	view/AssetMaker.hpp
	view/FontMaker.cpp
	view/FontMaker.hpp
	view/Editor.cpp
	view/Editor.hpp
	view/Home.cpp
	view/Home.hpp
	view/IconMaker.cpp
	view/IconMaker.hpp
	view/Project.cpp
	view/Project.hpp
	view/ThemeMaker.cpp
	view/ThemeMaker.hpp
	view/ThemePreview.cpp
	view/ThemePreview.hpp
	view/ViewObject.hpp
	view/extras/Extras.cpp
	view/extras/Extras.hpp

	controller/AssetManager.cpp
	controller/AssetManager.hpp
	controller/BootstrapIconManager.cpp
	controller/BootstrapIconManager.hpp
	controller/CodePrinter.hpp
	controller/FontAwesomeManager.cpp
	controller/FontAwesomeManager.hpp
	controller/FontManager.cpp
	controller/FontManager.hpp
	controller/Manager.cpp
	controller/Manager.hpp
	controller/StringPrinter.hpp
	controller/ThemeManager.cpp
	controller/ThemeManager.hpp
  controller/ExportWorker.cpp
  controller/ExportWorker.hpp

	PARENT_SCOPE)
