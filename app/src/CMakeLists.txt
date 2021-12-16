
set(SOURCES
	main.cpp
	qml.qrc

	logic/Manager.cpp
	logic/Manager.hpp
	logic/AssetManager.hpp
	logic/AssetManager.cpp
	logic/ThemeManager.hpp
	logic/ThemeManager.cpp
	logic/FontAwesomeManager.cpp
	logic/FontAwesomeManager.hpp
	logic/BootstrapIconManager.cpp
	logic/BootstrapIconManager.hpp
	logic/CodePrinter.hpp
	logic/StringPrinter.hpp
	logic/Settings.cpp
	logic/Settings.hpp

	screens/AssetMaker.cpp
	screens/AssetMaker.hpp
	screens/FontMaker.cpp
	screens/FontMaker.hpp
	screens/Home.cpp
	screens/Home.hpp
	screens/IconMaker.cpp
	screens/IconMaker.hpp
	screens/Project.cpp
	screens/Project.hpp
	screens/ThemeMaker.cpp
	screens/ThemeMaker.hpp
	screens/ThemePreview.cpp
	screens/ThemePreview.hpp

	extras/Extras.cpp
	extras/Extras.hpp

	Application.cpp
	Application.hpp

	screens/ScreenObject.hpp

	Tree.hpp

	Canvas.hpp
	Tools.hpp
	Properties.hpp

	FileBrowser.cpp
	FileBrowser.hpp

	PARENT_SCOPE)
