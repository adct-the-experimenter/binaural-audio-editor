cmake_minimum_required(VERSION 2.8.9)

set(PROJECT_NAME binaural-audio-editor)
project (${PROJECT_NAME} LANGUAGES C CXX)

#wxWidget specific stuff
SET(wxWidgets_USE_LIBS)
FIND_PACKAGE(wxWidgets COMPONENTS core base gl REQUIRED)
if(wxWidgets_FOUND)
        include(${wxWidgets_USE_FILE})
endif()

#OpenSceneGraph speicific stuff
FIND_PACKAGE (OpenSceneGraph REQUIRED COMPONENTS osgUtil osgDB osgText osgGA osgFX osgSim osgViewer )

#FreeBSD and linux specific include and libs
#Note: for FreeBSD, use cmake .. -DwxWidgets_CONFIG_EXECUTABLE=/usr/local/bin/wxgtk3u-3.1-config

#For the shared library:
set ( PROJECT_LINK_LIBS ${wxWidgets_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} openal.so sndfile.so)
#where to find library files .so 
link_directories( /usr/lib /usr/local/lib ) 

set ( PROJECT_INCLUDE_DIR ${wxWidgets_INCLUDE_DIRS} ${OPENSCENEGRAPH_INCLUDE_DIRS} )

#for where to find header files for source and library
include_directories(/usr/include /usr/local/include ./include ${PROJECT_INCLUDE_DIR})

set(SOURCES src/osgViewerWX.cpp   
			src/EditorManipulator.cpp
			 src/CreateSoundProducerDialog.cpp src/EditMultipleSoundProducersDialog.cpp
			 src/openalsoftaudioengine.cpp src/soundproducer.cpp
			 )

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

#make executable sphere from simple-sphere.cpp 
add_executable(${PROJECT_NAME} ${SOURCES})
#link libraries
target_link_libraries(${PROJECT_NAME} ${PROJECT_LINK_LIBS})
