
# binaural-audio-editor
This is an audio application that produces binaural audio from 2D mono audio samples and positional information given through the graphical user interface. Listen to 3D audio through stereo headphones.

## Moving on to a new project that will replace this one. 3d Audio Producer. 
## https://github.com/adct-the-experimenter/3d-audio-producer
## Keeping this here active on Github for anyone who wants to fork this.

Resize the window if unable to move timeline.

[![Open Source Helpers](https://www.codetriage.com/adct-the-experimenter/binaural-audio-editor/badges/users.svg)](https://www.codetriage.com/adct-the-experimenter/binaural-audio-editor)

## Required Libraries
- OpenAL Soft https://github.com/kcat/openal-soft

- openscenegraph https://github.com/openscenegraph/OpenSceneGraph

- libsndfile http://www.mega-nerd.com/libsndfile/

- wxwidgets https://www.wxwidgets.org/

- Boost Math Quaternion headers and Boost ASIO serial https://www.boost.org/users/history/version_1_70_0.html

- PugiXML https://github.com/zeux/pugixml/

## How to Install

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

## Controls

  - Left Click and mouse movement to move orientation of the camera i.e. the direction the camera is facing.
  
  - Right Click and mouse movement to move the camera forward or backward.
  
  - Left Click + Right Click and mouse movement to move the camera up, down, left, or right.

## Instructions

### Control Listener
  
  Light green cube is you the listener. 
  
  Use WASD keys to move listener(you) forward, back, left, right. Q key moves listener down. E key moves listener up.
  
  To use the Listener Track(Group of first 7 tracks with graph) to change position of the listener(you), uncheck Free Roam in EditListener menu. 
  Add points to the graph by left clicking on the graph. Remove points from the graph by right clicking.
  

  First graph controls the x position, second graph is for y position, third graph is for z position.
  
  
  Fourth graph controls w parameter of quaternion rotation, fifth graph controls rotation around x axis, sixth graph controls rotation around y axis, seventh graph controls rotation around z axis.
  
  To use IMU BNO055 sensor to control listener orientation.
  See file External-Orientation-Setup.md
  
### Control SoundProducers
  
  Light blue cubes are the sound producers.
  
  Create a sound producer and name it in Sound Producers -> Create Sound Producers menu.
  
  For the Sound Producer track, the top 2 tracks is for graphing audio and the bottom 3 tracks are for changing position of the sound producer.
  
  Click on dropdown box menu of sound producer track to choose a soundproducer to edit that was created.
  
  Click on browse button near audio track to load sound into the track. Load only 8-bit or 16-bit audio only.
  
  To move a sound producer with keyboard keys, set the free roam option as true when creating it or editing it.
  This option allows the sound producer to keep its position set by keys during playback.
  The select the sound producer in the drop-down menu in the tool bar and use I,J,K,L,U,O keys to move it.

### Coordinate System of Binaural Audio Editor and OpenAL Soft
  Like OpenAL, the application uses a right handed coordinate system to move listener and sound producers, where in a frontal default view X (thumb) points right,  Y points up (index finger), and Z points towards the viewer/camera (middle finger). 
  
  - Up is positive y. Down is negative y.
  
  - Back is positive z. Forward is negative z.
  
  - Right is positive x. Left is negative x.

## Important Note About Multi-channel Audio Input:
Stereo(2 channel) audio does not get 3d spatialization and is instead used to play as background music.

Have audio that you want to be played in 3D be mono(1 channel) audio
and load it into the audio track.

Stereo audio can be changed to mono audio using Audacity or other programs.

sndfile-mix-to-mono is an application that successfully turns multi-channel audio mix into mono-channel audio mix.

  
## Experimental Feature for 5.1,6.1,7.1 channel Surround Sound Output
Run the alsoft-config program that came with the installation of OpenAL Soft to
set the output to 5.1, 6.1, or 7.1 channels.
OpenAL Soft should automatically convert 3d audio information into audio output for surround sound.

For more information, read https://github.com/kcat/openal-soft/blob/master/docs/3D7.1.txt 

## To use with Digital Audio Workstation

See the import-audio-DAW-BAE plugin page about building and installing the plugin.

https://github.com/adct-the-experimenter/import-audio-DAW-BAE

This plugin will export audio from a digital audio workstation to the sound tracks of binaural audio editor so that one can use their DAW to edit audio and spatialize it in Binaural Audio Editor.

## Shortcut Keys

Press b with focus on graphical screen with 3d cubes to attach the last sound producer created to the last sound producer track and browse audio for it.


## Feedback 

Please email questions or comments to this email address bringerofawesomefood @ gmail . com without spaces.
