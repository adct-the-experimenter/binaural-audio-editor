#ifndef AUDIO_DEVICE_RECORDER_H
#define AUDIO_DEVICE_RECORDER_H

#include <string>

#include <sndfile.h>

#include <array>

#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/slider.h>
#include <wx/panel.h>
#include <cstdint>


#include "portaudio.h"

#define SAMPLE_RATE         48000
#define FRAMES_PER_BUFFER   64

class AudioDeviceRecorder : public wxPanel
{
	
public:
	AudioDeviceRecorder();
	~AudioDeviceRecorder();
	
	//function to set an audio device as the audio device to record
	void SetAsAudioDeviceToRecord(std::string devname);
	
	//function to prepare recording of the audio device
	bool PrepareDeviceForRecording();
	
	void OnSelectedAudioDeviceInComboBox(wxCommandEvent& event);
	void OnRecordButtonPressed(wxCommandEvent& event);
	void OnStopButtonPressed(wxCommandEvent& event);
	
	//function to record audio and putting audio data into a buffer to source
	void RecordAudioFromDevice();
		
	//function to stop device from recording and clean up some things
	void FreeDeviceFromRecording();
	
	//function to stop recording on device
	void StopRecording();
	
	void SetReferenceToRecordButton(wxButton* recordButtonPtr);
	
	void InitTrack(wxWindow* parent);
	
	wxComboBox* GetPointerToComboBox();
	wxButton* GetPointerToRecordButton();
	wxButton* GetPointerToStopButton();
	
private:
	
	//name of audio device to record
	std::string m_deviceName;
	int m_device_index;
	
	//formatting of audio data
	SF_INFO sfinfo;
	
	wxButton* m_record_button_ptr;
	wxButton* m_stop_button_ptr;
	wxComboBox* m_ad_combo_box;
	
	//array to contain audio data to write to file
	std::array <std::int16_t,FRAMES_PER_BUFFER> m_audio_data_saved;
	
	//portaudio stream 
	PaStream *m_stream_src_ptr;
    PaError err;
    PaStreamParameters inputParameters, outputParameters;
    
    bool m_stream_opened;
    bool m_stream_closed;
    
    bool recording;
};

#endif
