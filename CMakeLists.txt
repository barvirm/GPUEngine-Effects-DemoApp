cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME "DemoApp")
set(APP_NAME "DemoApp")

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(${PROJECT_NAME})

find_package(glm REQUIRED)
find_package(Effects REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Quick Widgets)
find_package(GPUEngine REQUIRED COMPONENTS geGL geSG geUtil)
find_package(AssimpModelLoader REQUIRED HINTS /home/makku/Programs/GPUEngine/geAd/AssimpModelLoader/cmake)
find_package(QtImageLoader REQUIRED HINTS /home/makku/Programs/GPUEngine/geAd/QtImageLoader/cmake)

set(DEFAULT_RESOURCES_PATH "${CMAKE_CURRENT_LIST_DIR}/resources")
set(${APP_NAME}_RESOURCES "${DEFAULT_RESOURCES_PATH}" CACHE PATH "Relative or absolute path to Application resources.")


set(GLSG_FILES
   src/glsg/EnumToGL.h src/glsg/EnumToGL.cpp
   src/glsg/GLAttribute.h
   src/glsg/GLScene.h
   src/glsg/GLSceneProcessor.h src/glsg/GLSceneProcessor.cpp
   src/glsg/TextureFactory.h src/glsg/TextureFactory.cpp
)

set(CORE_FILES
    src/core/main.cpp
    src/core/QuickRendererBase.cpp src/core/QuickRendererBase.h
    src/core/Renderer.cpp src/core/Renderer.h
    src/core/SceneLoader.cpp src/core/SceneLoader.h
)

set(GRAPHICS_FILES
    src/graphics/VisualizationTechnique.h
    src/graphics/SimpleVT.cpp src/graphics/SimpleVT.h
    src/graphics/skybox/SkyboxVT.h src/graphics/skybox/SkyboxVT.cpp
)

set(UTIL_FILES
    src/util/MouseEventHandler.cpp src/util/MouseEventHandler.h
)
set(SG_FILES
    src/sg/CubeMapTexture.cpp src/sg/CubeMapTexture.h
    src/sg/AnimationManager.cpp src/sg/AnimationManager.h
    src/sg/AnimationChannel.h
)

source_group(glsg FILES ${GLSG_FILES})
source_group(util FILES ${UTIL_FILES})
source_group(core FILES ${CORE_FILES})
source_group(sg FILES ${SG_FILES})
source_group(graphics FILES ${GRAPHICS_FILES})


add_executable(${APP_NAME} ${CORE_FILES} ${GLSG_FILES} ${GRAPHICS_FILES} ${UTIL_FILES} ${SG_FILES})
target_link_libraries(${APP_NAME} Qt5::Quick Qt5::Widgets geGL geSG geUtil AssimpModelLoader QtImageLoader)

target_include_directories(${APP_NAME} PUBLIC "src/")
target_include_directories(${APP_NAME} PUBLIC "src/core")
target_include_directories(${APP_NAME} PUBLIC "src/graphics")
target_include_directories(${APP_NAME} PUBLIC "src/util")

set_target_properties(${APP_NAME} PROPERTIES COMPILE_DEFINITIONS "APP_RESOURCES=\"${${APP_NAME}_RESOURCES}\"")
set_property(TARGET ${APP_NAME} PROPERTY FOLDER "${examples_folder_name}")
