#ifndef AUDIO_DEVICE_RECORDER_H
#define AUDIO_DEVICE_RECORDER_H

#include <string>

#include <sndfile.h>

#include <array>

#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/slider.h>
#include <wx/panel.h>

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
	
	//function to record audio and putting audio data into a buffer to source
	void RecordAudioFromDevice();
		
	//function to stop device from recording and clean up some things
	void FreeDeviceFromRecording();
	
	void SetReferenceToRecordButton(wxButton* recordButtonPtr);
	
	void InitTrack(wxWindow* parent);
	
	wxComboBox* GetPointerToComboBox();
	wxButton* GetPointerToRecordButton();
	
private:
	
	//name of audio device to record
	std::string m_deviceName;
	int m_device_index;
	
	//formatting of audio data
	SF_INFO sfinfo;
	
	size_t bit_size;
	size_t frame_size;
	int buffer_time_ms;
	size_t buffer_pack_size;
	
	
	size_t buffer_index; //keeps track of which buffer player is on

	wxButton* m_record_button_ptr;
	wxComboBox* m_ad_combo_box;
};

#endif
