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
message("ASSIMP LIBARARY DIR: " "${ASSIMP_LIBRARY_DIRS}")
#Create define with path to resources
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "APP_RESOURCES=\"${DEFAULT_RESOURCES_PATH}\"")


macro(COPY_RUNTIME_LIB_POST_BUILD TARGET_APP LINK_TARGET)
    # find the any *.dll for this link target
	get_target_property(RUNTIME_LIB ${LINK_TARGET} LOCATION)
	# find the release *.dll file
    get_target_property(RUNTIME_LIB_RELEASE ${LINK_TARGET} IMPORTED_LOCATION_RELEASE)
    # find the debug *d.dll file
    get_target_property(RUNTIME_LIB_DEBUG ${LINK_TARGET} IMPORTED_LOCATION_DEBUG)
	if (EXISTS ${RUNTIME_LIB_RELEASE})
		add_custom_command(TARGET ${TARGET_APP} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${RUNTIME_LIB_RELEASE} ${STANDALONE_APP_PATH}/lib)
	endif()
	if (EXISTS ${RUNTIME_LIB_DEBUG})
		add_custom_command(TARGET ${TARGET_APP} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${RUNTIME_LIB_DEBUG} ${STANDALONE_APP_PATH}/lib)
	endif()
	if (NOT EXISTS ${RUNTIME_LIB_DEBUG} AND 
	    NOT EXISTS ${RUNTIME_LIB_RELEASE} AND 
		EXISTS ${RUNTIME_LIB})
		add_custom_command(TARGET ${TARGET_APP} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${RUNTIME_LIB} ${STANDALONE_APP_PATH}/lib)
	endif()
endmacro()


# Create Standalone directories
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${RUNTIME_LIB_RELEASE} ${STANDALONE_APP_PATH})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${RUNTIME_LIB_RELEASE} ${STANDALONE_APP_PATH}/lib)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${RUNTIME_LIB_RELEASE} ${STANDALONE_APP_PATH}/bin)

# Copy executable
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> ${STANDALONE_APP_PATH}/bin)

# Copy .so
COPY_RUNTIME_LIB_POST_BUILD(${PROJECT_NAME} geCore)
COPY_RUNTIME_LIB_POST_BUILD(${PROJECT_NAME} geSG)
COPY_RUNTIME_LIB_POST_BUILD(${PROJECT_NAME} Effects)
COPY_RUNTIME_LIB_POST_BUILD(${PROJECT_NAME} geGL)
COPY_RUNTIME_LIB_POST_BUILD(${PROJECT_NAME} geUtil)
COPY_RUNTIME_LIB_POST_BUILD(${PROJECT_NAME} assimp::assimp)

IF(UNIX) 
    FIND_PROGRAM(BASH bash)
    FILE(WRITE  ${STANDALONE_APP_PATH}/run.sh "#!${BASH}\n")
    FILE(APPEND ${STANDALONE_APP_PATH}/run.sh "export LD_LIBRARY_PATH=\"$LD_LIBRARY_PATH:${STANDALONE_APP_PATH}/lib\"\n" )
    FILE(APPEND ${STANDALONE_APP_PATH}/run.sh "cd bin\n" )
    FILE(APPEND ${STANDALONE_APP_PATH}/run.sh "./${PROJECT_NAME}\n" )
ENDIF()

IF(WIN32)
	get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
	get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
	set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.exe)
ENDIF()
