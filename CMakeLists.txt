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

#For the shared library:
set ( PROJECT_LINK_LIBS ${wxWidgets_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} openal sndfile ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
#where to find library files .so 
link_directories( /usr/lib /usr/local/lib ) 

if (WIN32)
    #add -lwsock32 to library dependencies for windows
    set (PROJECT_LINK_LIBS ${PROJECT_LINK_LIBS} -lwsock32)
endif (WIN32)

set ( PROJECT_INCLUDE_DIR ${wxWidgets_INCLUDE_DIRS} ${OPENSCENEGRAPH_INCLUDE_DIRS} )

#for where to find header files for source and library
include_directories(/usr/include /usr/local/include ./include ./src/timeline-track-editor/include ${PROJECT_INCLUDE_DIR})



##############################

#RtAudio CMake specific


# Check for Jack (any OS)
find_library(JACK_LIB jack)
find_package(PkgConfig)
pkg_check_modules(jack jack)
if(JACK_LIB OR jack_FOUND)
  set(HAVE_JACK TRUE)
endif()

# Check for Pulse (any OS)
pkg_check_modules(pulse libpulse-simple)

# Check for known non-Linux unix-likes
if (CMAKE_SYSTEM_NAME MATCHES "kNetBSD.*|NetBSD.*")
  message(STATUS "NetBSD detected, using OSS")
  set(xBSD ON)
elseif(UNIX AND NOT APPLE)
  set(LINUX ON)
endif()

# Necessary for Windows
if(MINGW)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Standard CMake options

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")
endif()
if(WIN32)
  set(CMAKE_DEBUG_POSTFIX d CACHE STRING "Postfix for debug version of library")
endif()

# Build Options
option(RTAUDIO_BUILD_PYTHON "Build PyRtAudio python bindings" OFF)
set(RTAUDIO_TARGETNAME_UNINSTALL "uninstall" CACHE STRING "Name of 'uninstall' build target")

# API Options
option(RTAUDIO_API_DS "Build DirectSound API" OFF)
option(RTAUDIO_API_ASIO "Build ASIO API" OFF)
option(RTAUDIO_API_WASAPI "Build WASAPI API" ${WIN32})
option(RTAUDIO_API_OSS "Build OSS4 API" ${xBSD})
option(RTAUDIO_API_ALSA "Build ALSA API" ${LINUX})
option(RTAUDIO_API_PULSE "Build PulseAudio API" ${pulse_FOUND})
option(RTAUDIO_API_JACK "Build JACK audio server API" ${HAVE_JACK})
option(RTAUDIO_API_CORE "Build CoreAudio API" ${APPLE})

# Check for functions
include(CheckFunctionExists)
check_function_exists(gettimeofday HAVE_GETTIMEOFDAY)
if (HAVE_GETTIMEOFDAY)
    add_definitions(-DHAVE_GETTIMEOFDAY)
endif ()

# Init variables
set(rtaudio_SOURCES RtAudio.cpp RtAudio.h rtaudio_c.cpp rtaudio_c.h)
set(LINKLIBS)
set(PKGCONFIG_REQUIRES)
set(API_DEFS)
set(API_LIST)

# Tweak API-specific configuration.

# Jack
if (RTAUDIO_API_JACK AND jack_FOUND)
  set(NEED_PTHREAD ON)
  list(APPEND PKGCONFIG_REQUIRES "jack")
  list(APPEND API_DEFS "-D__UNIX_JACK__")
  list(APPEND API_LIST "jack")
  if(jack_FOUND)
    list(APPEND LINKLIBS ${jack_LIBRARIES})
    list(APPEND INCDIRS ${jack_INCLUDEDIR})
  else()
    list(APPEND LINKLIBS ${JACK_LIB})
  endif()
endif()

# ALSA
if (RTAUDIO_API_ALSA)
  set(NEED_PTHREAD ON)
  find_package(ALSA)
  if (NOT ALSA_FOUND)
    message(FATAL_ERROR "ALSA API requested but no ALSA dev libraries found")
  endif()
  list(APPEND INCDIRS ${ALSA_INCLUDE_DIR})
  list(APPEND LINKLIBS ${ALSA_LIBRARY})
  list(APPEND PKGCONFIG_REQUIRES "alsa")
  list(APPEND API_DEFS "-D__LINUX_ALSA__")
  list(APPEND API_LIST "alsa")
endif()

# OSS
if (RTAUDIO_OSS)
  set(NEED_PTHREAD ON)
  find_library(OSSAUDIO_LIB ossaudio)
  if (OSSAUDIO_LIB)
    list(APPEND LINKLIBS ossaudio)
    # Note: not an error on some systems
  endif()
  list(APPEND API_DEFS "-D__LINUX_OSS__")
  list(APPEND API_LIST "oss")
endif()

# Pulse
if (RTAUDIO_API_PULSE)
  set(NEED_PTHREAD ON)
  find_library(PULSE_LIB pulse)
  find_library(PULSESIMPLE_LIB pulse-simple)
  list(APPEND LINKLIBS ${PULSE_LIB} ${PULSESIMPLE_LIB})
  list(APPEND PKGCONFIG_REQUIRES "libpulse-simple")
  list(APPEND API_DEFS "-D__LINUX_PULSE__")
  list(APPEND API_LIST "pulse")
endif()

# CoreAudio
if (RTAUDIO_API_CORE)
  find_library(COREAUDIO_LIB CoreAudio)
  find_library(COREFOUNDATION_LIB CoreFoundation)
  list(APPEND LINKLIBS ${COREAUDIO_LIB} ${COREFOUNDATION_LIB})
  list(APPEND API_DEFS "-D__MACOSX_CORE__")
  list(APPEND API_LIST "core")
endif()

# ASIO
if (RTAUDIO_API_ASIO)
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
if (RTAUDIO_API_DS)
  set(NEED_WIN32LIBS ON)
  list(APPEND LINKLIBS dsound)
  list(APPEND API_DEFS "-D__WINDOWS_DS__")
  list(APPEND API_LIST "ds")
endif()

# WASAPI
if (RTAUDIO_API_WASAPI)
  include_directories(include)
  set(NEED_WIN32LIBS ON)
  list(APPEND LINKLIBS ksuser mfplat mfuuid wmcodecdspuuid)
  list(APPEND API_DEFS "-D__WINDOWS_WASAPI__")
  list(APPEND API_LIST "wasapi")
endif()

# Windows libs
if (NEED_WIN32LIBS)
  list(APPEND LINKLIBS winmm ole32)
endif()


# pthread
if (NEED_PTHREAD)
  find_package(Threads REQUIRED)
  list(APPEND LINKLIBS Threads::Threads)
endif()

# Set standard installation directories.
include(GNUInstallDirs)


##############################


set(SOURCES src/osgViewerWX.cpp
			src/audio-device-registry.cpp src/RtAudio.cpp
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

# Set compile-time definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE ${API_DEFS})
target_compile_definitions(${PROJECT_NAME} PRIVATE RTAUDIO_EXPORT)

#link libraries
target_link_libraries(${PROJECT_NAME} ${PROJECT_LINK_LIBS} ${LINKLIBS})




#make install ops
install(TARGETS binaural-audio-editor DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

