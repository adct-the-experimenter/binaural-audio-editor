#ifndef AUDIO_STREAM_CONTAINER
#define AUDIO_STREAM_CONTAINER

#include <sndfile.h>
#include <vector>
#include <string>
#include <iostream>

class AudioStreamContainer
{
public:
	AudioStreamContainer();

	void SetReferenceToInputAudioData(std::vector <double> *input_audio_data);
	
	void ResizeAudioStream(size_t thisSize);
	size_t GetSize();

	void SetPointerToDataAtThisSampleIndex(double* thisRef,size_t i);
	double* GetPointerToDataAtThisSampleIndex(size_t i);
	
	void CopyInputAudioDataToStream();
	
	void WriteStreamContentsToFile(std::string filename, int format, int channels, int samplerate,int buffer_length);
	
private:
	std::vector <double> *input_audio_data_ptr;
	
	std::vector <double*> stream_audio_data;
	
};

#endif
