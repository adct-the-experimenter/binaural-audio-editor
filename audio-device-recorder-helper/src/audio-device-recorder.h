#ifndef AUDIO_DEVICE_RECORDER_H
#define AUDIO_DEVICE_RECORDER_H

#include <string>

#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/slider.h>
#include <wx/panel.h>
#include <cstdint>

#include <sndfile.h>

#include <array>

#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#define BUFFER_FRAMES 1200
#define NUM_BUFFERS 4

struct DataArray
{
	std::array <std::int16_t,BUFFER_FRAMES> array_data;
	bool filled = false;
};

class AudioDeviceRecorder : public wxPanel
{
	
public:
	AudioDeviceRecorder();
	~AudioDeviceRecorder();
	
	//function to set an audio device as the audio device to record
	void SetAsAudioDeviceToRecord(std::string devname, int devIndex);
	
	//function to prepare recording of the audio device
	bool PrepareDeviceForRecording();
	
	//function to record audio and putting audio data into a buffer to source
	void RecordAudioFromDevice();
	
	//function to play audio from a buffer
	void PlayAudioRecordedFromDevice();
	
	//function to stop device from recording and clean up some things
	void FreeDeviceFromRecording();
	
//wxwidgets
	void OnSelectedAudioDeviceInComboBox(wxCommandEvent& event);
	void OnRecordButtonPressed(wxCommandEvent& event);
	void OnStopButtonPressed(wxCommandEvent& event);
	
	//function to stop recording on device
	void StopRecording();
	
	void SetReferenceToRecordButton(wxButton* recordButtonPtr);
	
	void InitTrack(wxWindow* parent);
	
	wxComboBox* GetPointerToComboBox();
	wxButton* GetPointerToRecordButton();
	wxButton* GetPointerToStopButton();
	
	
private:
	
	//RtAudio parameters
	RtAudio adc;
	RtAudio::StreamParameters parameters;
	unsigned int sampleRate;
	unsigned int bufferFrames; // 256 sample frames

	//name of audio device to record
	std::string m_deviceName;
	int m_deviceIndex;
	
	//formatting of audio data
	SF_INFO sfinfo;
	
	size_t bit_size;
	size_t frame_size;
	int buffer_time_ms;
	size_t buffer_pack_size;

	
	bool stream_opened;
	
	int buffer_filled = 0;

	DataArray tempArrayOne;
	DataArray tempArrayTwo;
	DataArray tempArrayThree;
	DataArray tempArrayFour;
	
	wxButton* m_record_button_ptr;
	wxButton* m_stop_button_ptr;
	wxComboBox* m_ad_combo_box;
	
	bool m_stream_opened;
    bool m_stream_closed;
    
    bool recording;
	
};

#endif
