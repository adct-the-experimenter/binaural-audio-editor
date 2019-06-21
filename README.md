# binaural-audio-editor
This is an audio application that produces binaural audio from 2D mono/stereo audio samples and positional information given through the graphical user interface. Listen to 3D audio through stereo headphones.

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
    
    FreeBSD: path-to-wxconfig-executable = /usr/local/bin/wxgtk3u-3.1-config
    
    Linux: path-to-wxconfig-executable = /usr/bin/wx-config-gtk3
7. make
8. ./binaural-audio-editor

# Controls

  *Left Click and mouse movement to move orientation of the camera i.e. the direction the camera is facing.
  
  *Right Click and mouse movement to move the camera forward or backward.
  
  *Left Click + Right Click and mouse movement to move the camera up, down, left, or right.

# Instructions
  
  Dark blue cube is you the listener. 
  
  Light blue cubes are the sound producers.

  Use the Listener Track(Group of first 3 tracks with graph) to change position of the listener(you). 
  Add points to the graph by left clicking on the graph. Remove points from the graph by right clicking.
  First graph controls the x position, second graph is for y position, third graph is for z position.
  
  The bottom 3 tracks are for changing position of the sound producer.
  
  Click on the Add Sound Producer Track button to add a new track to control another sound producer.
  
  Click on the Remove Sound Producer Track button to remove a sound producer track.
  
  The application uses standard Cartesian coordinate system to move sound producers and listener.
  
  Up is positive z. Down is negative z.
  
  Back is positive x. Forward is negative x.
  
  Right is positive y. Left is negative y.

# Important Note:
Stereo(2 channel) audio does not get 3d spatialization and is instead used to play as background music.

Have audio that you want to be played in 3D be mono(1 channel) audio
and load it into the audio track.

Stereo audio can be changed to mono audio using Audacity or other programs.
  
# Experimental Feature for 5.1,6.1,7.1 channel Surround Sound Output
Run the alsoft-config program that came with the installation of OpenAL Soft to
set the output to 5.1, 6.1, or 7.1 channels.
OpenAL Soft should automatically convert 3d audio information into audio output for surround sound.

For more information, read https://github.com/kcat/openal-soft/blob/master/docs/3D7.1.txt 
