cmake_minimum_required(VERSION 2.6)

project(CefiedGame)

# Use C++11
set(CMAKE_CXX_STANDARD 11)

# Find 3rdparty
IF(NOT FX11_FOUND)
	FIND_LIBRARY(FX11_LIBRARIES NAMES Effects11 PATHS "${PROJECT_SOURCE_DIR}/3rdparty/FX11/Bin" DOC "FX11 library")
	FIND_LIBRARY(FX11_DEBUG_LIBRARIES NAMES Effects11d PATHS "${PROJECT_SOURCE_DIR}/3rdparty/FX11/Bin" DOC "FX11 library")
	FIND_PATH(FX11_INCLUDE_DIRS d3dx11effect.h "${PROJECT_SOURCE_DIR}/3rdparty/FX11/inc" DOC "FX11 includes")
	include_directories(${FX11_INCLUDE_DIRS})
ENDIF(NOT FX11_FOUND)

include_directories(${PROJECT_SOURCE_DIR})

# Enumerate CefiedGame sources
set(CEFIEDGAME_INTERFACE_FILES 
	Interfaces/IAny.h
	Interfaces/IViewport.h
	Interfaces/IScene.h
	Interfaces/ICamera.h
	Interfaces/IObject.h
	Interfaces/IDrawable.h
	Interfaces/IGeometry.h

	Interfaces/IMaterial.h
	Interfaces/IShaderVariable.h

	Interfaces/TypeSystem.cpp
	Interfaces/TypeSystem.h
)
source_group("Interface Files" FILES ${CEFIEDGAME_INTERFACE_FILES})

set(CEFIEDGAME_SCENE_SOURCE_FILES
	Scene/Camera.cpp
	Scene/Camera.h
	Scene/Transformation.cpp
	Scene/Transformation.h

	Scene/Geometry.cpp
	Scene/Geometry.h
	Scene/Mesh.cpp
	Scene/Mesh.h

	Scene/Material.cpp
	Scene/Material.h
	Scene/ShaderVariable.cpp
	Scene/ShaderVariable.h
)
source_group("Scene Files" FILES ${CEFIEDGAME_SCENE_SOURCE_FILES})

set(CEFIEDGAME_TOOL_SOURCE_FILES
	Tools/GameTimer.cpp
	Tools/GameTimer.h
	Tools/Macros.h
	Tools/ConsoleFPS.cpp
	Tools/ConsoleFPS.h

	Tools/DDSTextureLoader.cpp
	Tools/DDSTextureLoader.h
)
source_group("Tool Files" FILES ${CEFIEDGAME_TOOL_SOURCE_FILES})

set(CEFIEDGAME_MAIN_COMPONENT_SOURCE_FILES
	MainComponents/SceneManager.cpp
	MainComponents/SceneManager.h
	MainComponents/IOHandler.cpp
	MainComponents/IOHandler.h
	MainComponents/Renderer.cpp
	MainComponents/Renderer.h
	MainComponents/Window.cpp
	MainComponents/Window.h
)
source_group("Main component Files" FILES ${CEFIEDGAME_MAIN_COMPONENT_SOURCE_FILES})

# Setup CefiedGame build
add_library(CefiedGame ${CEFIEDGAME_INTERFACE_FILES} ${CEFIEDGAME_SCENE_SOURCE_FILES} ${CEFIEDGAME_TOOL_SOURCE_FILES} ${CEFIEDGAME_MAIN_COMPONENT_SOURCE_FILES})
target_link_libraries(CefiedGame debug ${FX11_DEBUG_LIBRARIES})
target_link_libraries(CefiedGame optimized ${FX11_LIBRARIES})