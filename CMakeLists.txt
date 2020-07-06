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

#RTAudio specific stuff

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
if(CMAKE_SYSTEM_NAME MATCHES "kNetBSD.*|NetBSD.*")
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
option(BUILD_SHARED_LIBS "Build as shared library" OFF)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")
endif()
if(WIN32)
  set(CMAKE_DEBUG_POSTFIX d CACHE STRING "Postfix for debug version of library")
endif()

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
if(HAVE_GETTIMEOFDAY)
  add_definitions(-DHAVE_GETTIMEOFDAY)
endif()

# Add -Wall if possible
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif(CMAKE_COMPILER_IS_GNUCXX)

# Add debug flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-D__RTAUDIO_DEBUG__)
  if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
  endif(CMAKE_COMPILER_IS_GNUCXX)
endif()

# Init variables
set(rtaudio_SOURCES src/rtaudio/RtAudio.cpp src/rtaudio/RtAudio.h)
set(LINKLIBS)
set(PKGCONFIG_REQUIRES)
set(API_DEFS)
set(API_LIST)

# Tweak API-specific configuration.

# Jack
if(RTAUDIO_API_JACK AND jack_FOUND)
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
if(RTAUDIO_API_ALSA)
  set(NEED_PTHREAD ON)
  find_package(ALSA)
  if(NOT ALSA_FOUND)
    message(FATAL_ERROR "ALSA API requested but no ALSA dev libraries found")
  endif()
  list(APPEND INCDIRS ${ALSA_INCLUDE_DIR})
  list(APPEND LINKLIBS ${ALSA_LIBRARY})
  list(APPEND PKGCONFIG_REQUIRES "alsa")
  list(APPEND API_DEFS "-D__LINUX_ALSA__")
  list(APPEND API_LIST "alsa")
endif()

# OSS
if(RTAUDIO_OSS)
  set(NEED_PTHREAD ON)
  find_library(OSSAUDIO_LIB ossaudio)
  if(OSSAUDIO_LIB)
    list(APPEND LINKLIBS ossaudio)
    # Note: not an error on some systems
  endif()
  list(APPEND API_DEFS "-D__LINUX_OSS__")
  list(APPEND API_LIST "oss")
endif()

# Pulse
if(RTAUDIO_API_PULSE)
  set(NEED_PTHREAD ON)
  find_library(PULSE_LIB pulse)
  find_library(PULSESIMPLE_LIB pulse-simple)
  list(APPEND LINKLIBS ${PULSE_LIB} ${PULSESIMPLE_LIB})
  list(APPEND PKGCONFIG_REQUIRES "libpulse-simple")
  list(APPEND API_DEFS "-D__LINUX_PULSE__")
  list(APPEND API_LIST "pulse")
endif()

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

# Windows libs
if(NEED_WIN32LIBS)
  list(APPEND LINKLIBS winmm ole32)
endif()

# pthread
if(NEED_PTHREAD)
  find_package(Threads REQUIRED
    CMAKE_THREAD_PREFER_PTHREAD
    THREADS_PREFER_PTHREAD_FLAG)
  list(APPEND LINKLIBS Threads::Threads)
endif()

# Create library targets.
set(LIB_TARGETS)

# Use RTAUDIO_BUILD_SHARED_LIBS / RTAUDIO_BUILD_STATIC_LIBS if they
# are defined, otherwise default to standard BUILD_SHARED_LIBS.
if(DEFINED RTAUDIO_BUILD_SHARED_LIBS)
  if(RTAUDIO_BUILD_SHARED_LIBS)
    add_library(rtaudio SHARED ${rtaudio_SOURCES})
  else()
    add_library(rtaudio STATIC ${rtaudio_SOURCES})
    set(RTAUDIO_IS_STATIC TRUE)
  endif()
elseif(DEFINED RTAUDIO_BUILD_STATIC_LIBS)
  if(RTAUDIO_BUILD_STATIC_LIBS)
    add_library(rtaudio STATIC ${rtaudio_SOURCES})
    set(RTAUDIO_IS_STATIC TRUE)
  else()
    add_library(rtaudio SHARED ${rtaudio_SOURCES})
  endif()
else()
  add_library(rtaudio ${rtaudio_SOURCES})
  if(NOT BUILD_SHARED_LIBS)
    set(RTAUDIO_IS_STATIC TRUE)
  endif()
endif()
list(APPEND LIB_TARGETS rtaudio)

if(NOT DEFINED RTAUDIO_STATIC_MSVCRT)
  set(RTAUDIO_STATIC_MSVCRT ${RTAUDIO_IS_STATIC})
endif()

# In MSVC, set MD/MT appropriately for a static library
# (From https://github.com/protocolbuffers/protobuf/blob/master/cmake/CMakeLists.txt)
if(MSVC AND RTAUDIO_STATIC_MSVCRT)
  foreach(flag_var
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
  endforeach(flag_var)
endif()

# Add headers destination for install rule.
set_property(TARGET rtaudio PROPERTY PUBLIC_HEADER RtAudio.h)
set_target_properties(rtaudio PROPERTIES
  SOVERSION ${SO_VER}
  VERSION ${FULL_VER})

# Set standard installation directories.
include(GNUInstallDirs)

# Set include paths, populate target interface.
target_include_directories(rtaudio
  INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/rtaudio>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    ${INCDIRS}
)

# Set compile-time definitions
target_compile_definitions(rtaudio PRIVATE ${API_DEFS})
target_compile_definitions(rtaudio PRIVATE RTAUDIO_EXPORT)
target_link_libraries(rtaudio ${LINKLIBS})

# Subdirs
include(CTest)

if(NOT DEFINED RTAUDIO_BUILD_TESTING OR RTAUDIO_BUILD_TESTING STREQUAL "")
  set(RTAUDIO_BUILD_TESTING ${BUILD_TESTING})
endif()

# Message
string(REPLACE ";" " " apilist "${API_LIST}")
message(STATUS "Compiling with support for: ${apilist}")

# PkgConfig file
string(REPLACE ";" " " req "${PKGCONFIG_REQUIRES}")
string(REPLACE ";" " " api "${API_DEFS}")
set(prefix ${CMAKE_INSTALL_PREFIX})

# Store the package in the user registry.
export(PACKAGE RtAudio)

# Create CMake configuration export file.
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/RtAudioConfig.cmake.in "@PACKAGE_INIT@\n")

if(NEED_PTHREAD)
  file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/RtAudioConfig.cmake.in "find_package(Threads REQUIRED)\n")
endif()

file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/RtAudioConfig.cmake.in "include(\${CMAKE_CURRENT_LIST_DIR}/RtAudioTargets.cmake)")

# Create uninstall target.                                                                                              
#configure_file(
#  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
#  "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake" IMMEDIATE @ONLY)
  
#add_custom_target(
#  uninstall "${CMAKE_COMMAND}" -P
#            "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake")

include_directories(..)
if (WIN32)
    include_directories(../include)
endif (WIN32)

list(GET LIB_TARGETS 0 LIBRTAUDIO)

#End RTAudio specific stuff

#For the shared library:
set ( PROJECT_LINK_LIBS ${wxWidgets_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} openal.so sndfile.so ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${LIBRTAUDIO} ${LINKLIBS})
#where to find library files .so 
link_directories( /usr/lib /usr/local/lib ) 

set ( PROJECT_INCLUDE_DIR ${wxWidgets_INCLUDE_DIRS} ${OPENSCENEGRAPH_INCLUDE_DIRS} src/rtaudio )

#for where to find header files for source and library
include_directories(/usr/include /usr/local/include ./include ./src/timeline-track-editor/include ${PROJECT_INCLUDE_DIR})

set(SOURCES src/osgViewerWX.cpp
			src/lcc-output-dialog.cpp src/rtaudio/RtAudio.cpp
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

# Add headers destination for install rule.
set_property(TARGET ${PROJECT_NAME} PROPERTY PUBLIC_HEADER RtAudio.h)
set_target_properties(${PROJECT_NAME} PROPERTIES
  SOVERSION ${SO_VER}
  VERSION ${FULL_VER})

# Set standard installation directories.
include(GNUInstallDirs)

# Set include paths, populate target interface.
target_include_directories(${PROJECT_NAME}
  INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/rtaudio>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    ${INCDIRS}
)
