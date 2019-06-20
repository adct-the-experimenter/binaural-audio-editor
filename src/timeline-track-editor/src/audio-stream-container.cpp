#include "audio-stream-container.h"

AudioStreamContainer::AudioStreamContainer()
{
	input_audio_data_ptr = nullptr;
}

void AudioStreamContainer::SetReferenceToInputAudioData(std::vector <double> *input_audio_data){input_audio_data_ptr = input_audio_data;}


void AudioStreamContainer::ResizeAudioStream(size_t thisSize){stream_audio_data.resize(thisSize);}
size_t AudioStreamContainer::GetSize(){return stream_audio_data.size();}

void AudioStreamContainer::SetPointerToDataAtThisSampleIndex(double* thisRef,size_t i)
{
	stream_audio_data.at(i) = thisRef;
}

double* AudioStreamContainer::GetPointerToDataAtThisSampleIndex(size_t i)
{
	return stream_audio_data.at(i);
}

void AudioStreamContainer::CopyInputAudioDataToStream()
{
	if(input_audio_data_ptr != nullptr)
	{
		for(size_t i=0; i < input_audio_data_ptr->size(); i++)
		{
			double* thisRef = &(input_audio_data_ptr->at(i));
			stream_audio_data[i] = thisRef;
		}
	}
}

void AudioStreamContainer::WriteStreamContentsToFile(std::string filename, int format, int channels, int sample_rate,int buffer_length)
{
	SF_INFO sfinfo;
    sfinfo.channels = channels;
    sfinfo.samplerate = sample_rate;
    sfinfo.format = format;
    
    SNDFILE * outFile;
    
	// Open the stream file
	if (! ( outFile = sf_open (filename.c_str(), SFM_WRITE, &sfinfo)))
	{	
		std::cout << "Not able to open stream file for writing" << outFile << std::endl;
		puts (sf_strerror (NULL)) ;
		return;
	} 
	
	//write data
	size_t readSize = stream_audio_data.size();
	sf_count_t write_count = 0; 
	size_t count_buffer = 0;
	
	write_count = sf_write_double(outFile, stream_audio_data.front(), readSize);
	
	sf_close(outFile);
}
