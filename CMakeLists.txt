cmake_minimum_required(VERSION 3.1.1)

set (CMAKE_CXX_STANDARD 11)

set(PROJECT_NAME binaural-audio-editor)
project (${PROJECT_NAME} LANGUAGES C CXX)


#-DCMAKE_BUILD_TYPE=Release
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -DDATADIR=${DATAPATH} -g -Wall -fPIE")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -DDATADIR=${DATAPATH} -Wall -fPIE")

#wxWidget specific stuff
SET(wxWidgets_USE_LIBS)
FIND_PACKAGE(wxWidgets COMPONENTS core base gl REQUIRED)
if(wxWidgets_FOUND)
        include(${wxWidgets_USE_FILE})
endif()

#OpenSceneGraph speicific stuff
FIND_PACKAGE (OpenSceneGraph REQUIRED COMPONENTS osgUtil osgDB osgText osgGA osgFX osgSim osgViewer )

#for serial data stuff
set(BOOST_LIBS date_time system)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
find_package(Threads REQUIRED)

#FreeBSD and linux specific include and libs
#Note: for FreeBSD, use cmake .. -DwxWidgets_CONFIG_EXECUTABLE=/usr/local/bin/wxgtk3u-3.1-config

#RTAudio speicifc stuff

# Check for Jack (any OS)
find_library(JACK_LIB jack)
find_package(PkgConfig)
pkg_check_modules(jack jack)
if(JACK_LIB OR jack_FOUND)
  set(HAVE_JACK TRUE)
endif()

# Check for Pulse (any OS)
pkg_check_modules(pulse libpulse-simple)

# API Options
option(RTAUDIO_API_DS "Build DirectSound API" OFF)
option(RTAUDIO_API_ASIO "Build ASIO API" OFF)
option(RTAUDIO_API_WASAPI "Build WASAPI API" ${WIN32})
option(RTAUDIO_API_OSS "Build OSS4 API" ${xBSD})
option(RTAUDIO_API_ALSA "Build ALSA API" ${LINUX})
option(RTAUDIO_API_PULSE "Build PulseAudio API" ${pulse_FOUND})
option(RTAUDIO_API_JACK "Build JACK audio server API" ${HAVE_JACK})
option(RTAUDIO_API_CORE "Build CoreAudio API" ${APPLE})

# CoreAudio
if(RTAUDIO_API_CORE)
  find_library(COREAUDIO_LIB CoreAudio)
  find_library(COREFOUNDATION_LIB CoreFoundation)
  list(APPEND LINKLIBS ${COREAUDIO_LIB} ${COREFOUNDATION_LIB})
  list(APPEND API_DEFS "-D__MACOSX_CORE__")
  list(APPEND API_LIST "core")
endif()

# ASIO
if(RTAUDIO_API_ASIO)
  set(NEED_WIN32LIBS ON)
  include_directories(include)
  list(APPEND rtaudio_SOURCES
    include/asio.cpp
    include/asiodrivers.cpp
    include/asiolist.cpp
    include/iasiothiscallresolver.cpp)
  list(APPEND API_DEFS "-D__WINDOWS_ASIO__")
  list(APPEND API_LIST "asio")
endif()

# DSound
if(RTAUDIO_API_DS)
  set(NEED_WIN32LIBS ON)
  list(APPEND LINKLIBS dsound)
  list(APPEND API_DEFS "-D__WINDOWS_DS__")
  list(APPEND API_LIST "ds")
endif()

# WASAPI
if(RTAUDIO_API_WASAPI)
  include_directories(include)
  set(NEED_WIN32LIBS ON)
  list(APPEND LINKLIBS ksuser mfplat mfuuid wmcodecdspuuid)
  list(APPEND API_DEFS "-D__WINDOWS_WASAPI__")
  list(APPEND API_LIST "wasapi")
endif()

#For the shared library:
set ( PROJECT_LINK_LIBS ${wxWidgets_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} openal.so sndfile.so ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
#where to find library files .so 
link_directories( /usr/lib /usr/local/lib ) 

set ( PROJECT_INCLUDE_DIR ${wxWidgets_INCLUDE_DIRS} ${OPENSCENEGRAPH_INCLUDE_DIRS} )

#for where to find header files for source and library
include_directories(/usr/include /usr/local/include ./include ./src/timeline-track-editor/include ${PROJECT_INCLUDE_DIR})

set(SOURCES src/osgViewerWX.cpp
			src/lcc-output-dialog.cpp src/RtAudio.cpp
			src/CreateStandardReverbZoneDialog.cpp src/EditMultipleStandardReverbZonesDialog.cpp 
			src/CreateEAXReverbZoneDialog.cpp src/EditMultipleEAXReverbZonesDialog.cpp 
			src/CreateEchoZoneDialog.cpp src/EditMultipleEchoZonesDialog.cpp
			src/echo-zone.cpp
			src/reverb-zone.cpp
			src/effect-zone.cpp
			src/effects-manager.cpp
			src/soundproducer-track-manager.cpp
			src/soundproducer-track.cpp
			src/listener-track.cpp
			src/soundproducer-registry.cpp
			src/CreateSoundProducerDialog.cpp src/EditMultipleSoundProducersDialog.cpp 
			src/Change-HRTF-Dialog.cpp src/HRTF-Test-Dialog.cpp 
			src/EditListenerDialog.cpp
			src/setup-serial-dialog.cpp
			src/listener-external.cpp
			src/openalsoftaudioengine.cpp src/soundproducer.cpp src/listener.cpp
			src/external-orientation-device-serial.cpp

			src/timeline-track-editor/src/timeline-frame.cpp
			src/timeline-track-editor/src/timeline-window.cpp
			src/timeline-track-editor/src/playback-controls.cpp
			src/timeline-track-editor/src/stereo-audio-track.cpp
			src/timeline-track-editor/src/mono-audio-track.cpp
			src/timeline-track-editor/src/audio-track.cpp
			src/timeline-track-editor/src/audio-graph.cpp
			src/timeline-track-editor/src/audio-stream-container.cpp
			src/timeline-track-editor/src/openalsoft-player.cpp
			src/timeline-track-editor/src/double-track.cpp
			src/timeline-track-editor/src/editor-graph.cpp
			src/timeline-track-editor/src/track.cpp
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

#make install ops
install(TARGETS binaural-audio-editor DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

