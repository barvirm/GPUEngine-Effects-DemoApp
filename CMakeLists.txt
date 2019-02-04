CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

#Name of project
PROJECT("DemoApp")

#Setup Qt cmake
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)

#Requirements for cpp
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

#Compile json file for vim and other editors
SET(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

#Dependencies
FIND_PACKAGE(glm REQUIRED)
FIND_PACKAGE(Effects REQUIRED)
FIND_PACKAGE(Qt5 REQUIRED COMPONENTS Quick Widgets)
FIND_PACKAGE(GPUEngine REQUIRED geGL geSG geUtil)
FIND_PACKAGE(AssimpModelLoader REQUIRED)
FIND_PACKAGE(QtImageLoader REQUIRED)

#Path to resources (models, texture, glsl code)
SET(DEFAULT_RESOURCES_PATH "${CMAKE_CURRENT_LIST_DIR}/resources" CACHE PATH "Relative or absolute path to Application resources.")

#Compilation is with sources and header files
ADD_SUBDIRECTORY(src)
