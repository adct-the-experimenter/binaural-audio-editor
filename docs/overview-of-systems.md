# Overview of Systems in Binaural Audio Editor

These are the main three systems that binaural audio editor interfaces with.
- Main Graphical User Interface, GUI
- OpenAL Soft Audio Engine
- Timeline Track Editor

## Main Graphical User Interface, GUI
This is what puts together everything seen in binaural audio editor.


### Files to look at:
- `osgViewerWX.h`
- `osgViewerWX.cpp`
- `SoundProducerTrackManager.h`
- `SoundProducerTrackManager.cpp`
- Files with Dialog in the filename in include and src folder.

#### In`osgViewerWX.cpp` and `osgViewerWX.h`

**OsgCanvas** is a class to show the OpenSceneGraph window and 3D objects.

**GraphicsWindowWX** is a class that contains OsgCanvas and contains the rendering output in a window.

**MainFrame** is the window that contains the menu bar above and its items, contains the timeline editor, and opens dialogs/windows when a menu item is clicked on. 

Since it is the main part that user interacts with, MainFrame also contains important classes such as TimelineFrame, reference to OpenALSoftAudioEngine, reference to Listener, SoundProducerTrackManager, vector of SoundProducerTrack, ListenerTrack.

**wxOsgApp** is the main of the wxWidgets appplication, much like the main of a basic C++ program. It contains MainFrame and initializes it.

#### In`SoundProducerTrackManager.cpp` and `SoundProducerTrackManager.h`

**SoundProducerTrackManager** is a class that handles multiple sound producer tracks, buffers the audio, and plays the audio of sound producer tracks in sync.

## OpenALSoftAudioEngine 
This is a class to abstract 3D audio operations involving OpenAL Soft.

It is mostly just used to initialize the OpenAL system and query and test HRTF support.

### Files to look at
- `openalsoftaudioengine.h` 
- `openalsoftaudioengine.cpp`

## Timeline Track Editor
This is a very big important module that is used by binaural audio editor. 

It is imported from Timeline Track Editor github https://github.com/adct-the-experimenter/timeline-track-editor.

This module is contained in its own folder in src/timeline-track-editor.

The timeline track editor module contains many important classes such as Track, DoubleTrack, StereoAudioTrack, and TimelineFrame. This is the module to edit to improve the tracks and timeline and see how audio gets buffered.

**Without the Timeline Track Editor module, there would not be a listener track, soundproducer track, or even a timeline for playing audio!**

The reasons this is developed into its own separate project: 
- It is very big.
- It is functional apart from binaural audio editor.
- It is much easier and cleaner to edit it as its own project rather than inside binaural audio editor.

### Files to look at
- `src/timeline-track-editor/include/timeline-frame.h`
- `src/timeline-track-editor/src/timeline-frame.cpp`
- `src/timeline-track-editor/include/openalsoft-player.h`
- `src/timeline-track-editor/src/openalsoft-player.cpp`
- `src/timeline-track-editor/include/double-track.h`
- `src/timeline-track-editor/src/double-track.cpp`
- `src/timeline-track-editor/include/stereo-audio-track.h`
- `src/timeline-track-editor/src/stereo-audio-track.cpp`
