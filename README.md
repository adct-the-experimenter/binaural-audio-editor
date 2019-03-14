# binaural-audio-editor
This is an audio application that produces binaural audio from 2D mono/stereo audio samples. Listen to 3D audio through stereo headphones.

# Required Libraries
OpenAL Soft https://github.com/kcat/openal-soft

openscenegraph https://github.com/openscenegraph/OpenSceneGraph

libsndfile http://www.mega-nerd.com/libsndfile/

wxwidgets https://www.wxwidgets.org/


# How to Install

1. Install required libraries.
2. Clone this repository
3. cd binaural-audio-editor
4. mkdir build
5. cd build
6. cmake .. -DwxWidgets_CONFIG_EXECUTABLE=path-to-wxconfig-executable
    
    FreeBSD and Linux: /usr/local/bin/wxgtk3u-3.1-config
7. make
8. ./binaural-audio-editor

# Controls
  *Left Click and mouse movement to move orientation of camera i.e. the direction the camera is facing.
  *Right Click and mouse movement to move position of camera.
