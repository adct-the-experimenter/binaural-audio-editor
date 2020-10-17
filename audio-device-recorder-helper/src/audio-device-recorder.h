#ifndef AUDIO_DEVICE_RECORDER_H
#define AUDIO_DEVICE_RECORDER_H

#include <string>

#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/slider.h>
#include <wx/panel.h>
#include <cstdint>
#include <wx/checkbox.h>

#include <sndfile.h>

#include <array>

#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#define BUFFER_FRAMES 2400
#define NUM_BUFFERS 2

struct DataArray
{
	std::array <std::int16_t,BUFFER_FRAMES> array_data;
	bool filled = false;
	std::string filename_end = "";
};

class AudioDataArray
{
public:
	
	AudioDataArray();
	~AudioDataArray();
	
	//function to write data in main array to file
	void WriteArrayDataToFile(std::string filename);
	
	//function to copy data from an array to the main array
	void CopyDataToMainArray(std::array <std::int16_t,BUFFER_FRAMES> &this_array);
	
	void SetMainArrayFullBool(bool state);
	bool IsMainArrayFull();
	
private:

	//assuming sample rate 48 kHz, 1 second of audio
	
	//main array that has data written to it
	std::array <std::int16_t,48000> m_main_array;
	
	//current read position of m_main array
	size_t m_main_current_pos;
	
	//backup array used to save data from recording when data is being written to file
	std::array <std::int16_t,12000> m_backup_write_array;
	
	//backup array used to copy data when main array is full
	std::array <std::int16_t,12000> m_backup_copy_array;
	
	//bool to indicate if main array is full
	bool m_full;
	
	//bool to indicate if there is data written to backup copy array
	bool m_data_in_backup_copy;
	
	//formatting of audio data
	SF_INFO sfinfo;
	
};


#include <functional>   // std::function, std::negate


//class to use for calling record repeatedly
class RecorderTimer : public wxTimer
{
public:
    RecorderTimer();
    
    void Notify(); //action to take periodically after certain amount of time defined
    void start();
	void AddFunctionToTimerLoop( std::function < void() > thisFunction);
	void stop();
	
private:
	//function to call everytime Notify is called
	std::function < void() > m_function;
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
	
	enum class HelperProgramBufferState : std::int8_t { NONE=0, BUFFER_1_READY_READ, BUFFER_2_READY_READ};
	
private:
	
	//RtAudio parameters
	RtAudio adc;
	RtAudio::StreamParameters parameters;
	unsigned int sampleRate;
	unsigned int bufferFrames; // 256 sample frames

	//name of audio device to record
	std::string m_deviceName;
	int m_deviceIndex;
	
	size_t bit_size;
	size_t frame_size;
	int buffer_time_ms;
	size_t buffer_pack_size;
	
	bool stream_opened;
	
	int buffer_filled = 0;
	
	AudioDataArray m_main_audio_array;

	DataArray tempArrayOne;
	DataArray tempArrayTwo;
	//DataArray tempArrayThree;
	//DataArray tempArrayFour;
	
	wxButton* m_record_button_ptr;
	wxButton* m_stop_button_ptr;
	wxComboBox* m_ad_combo_box;
	
	bool m_stream_opened;
    bool m_stream_closed;
    
    bool recording;
	
	std::string data_dir_fp;
	
	RecorderTimer m_rec_timer;
	
	//state
	
	AudioDeviceRecorder::HelperProgramBufferState m_state;
	void SetState(AudioDeviceRecorder::HelperProgramBufferState);
	AudioDeviceRecorder::HelperProgramBufferState GetState();
	void WriteStateToFile();
	
	std::string fp_state_file;
	
};

#endif
