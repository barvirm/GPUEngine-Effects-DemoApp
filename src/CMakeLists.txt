#List of source files  
# TODO cant be do better way ??
SET(SOURCE_FILES
	glsg/EnumToGL.cpp
	glsg/GLSceneProcessor.cpp
	glsg/TextureFactory.cpp

	core/main.cpp
	core/QuickRendererBase.cpp
	core/GERendererBase.cpp
	core/Renderer.cpp

	graphics/SimpleVT.cpp
	graphics/skybox/SkyboxVT.cpp
	graphics/LaserManager.cpp
   graphics/ShieldManager.cpp
   graphics/testing/TestingVT.cpp

	util/MouseEventHandler.cpp
	util/intersectors/LineSphereIntersector.cpp
	util/collision/LaserShieldCollider.cpp
	util/SceneLoader.cpp

	sg/CubeMapTexture.cpp
	sg/AnimationManager.cpp
	sg/AnimationEventChannel.cpp
)

#List of header files
# TODO cant be do better way ??
SET(HEADER_FILES
   glsg/EnumToGL.h
   glsg/GLAttribute.h
   glsg/GLScene.h
   glsg/GLSceneProcessor.h
   glsg/TextureFactory.h
   
   core/QuickRendererBase.h
   core/GERendererBase.h
   core/Renderer.h
   
   graphics/SimpleVT.h
   graphics/skybox/SkyboxVT.h 
   graphics/LaserManager.h 
   graphics/ShieldManager.h 
   graphics/testing/TestingVT.h
   
   
   util/MouseEventHandler.h
   util/intersectors/LineSphereIntersector.h
   util/collision/LaserShieldCollider.h
   util/Timer.h
   util/Algorithm.h
   util/collision/Collider.h
   util/SceneLoader.h
   
   sg/CubeMapTexture.h
   sg/AnimationManager.h
   sg/AnimationChannel.h
   sg/AnimationEvent.h
   sg/AnimationEventChannel.h
)

#Create executable
ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

#Link dependencies
TARGET_LINK_LIBRARIES(${PROJECT_NAME} Qt5::Quick Qt5::Widgets) # Qt
TARGET_LINK_LIBRARIES(${PROJECT_NAME} Effects) # Effects (My lib)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} geGL geSG geUtil AssimpModelLoader QtImageLoader) # GPUEngine

#GLM define for extensions
#target_compile_definitions(${PROJECT_NAME} GLM_ENABLE_EXPERIMENTAL)

#include directories 
#TODO remove or replace with something meaningfull
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ".")
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC "core")
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC "graphics")
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC "util")
#Create define with path to resources
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "APP_RESOURCES=\"${DEFAULT_RESOURCES_PATH}\"")

include(CreateStandAlone)