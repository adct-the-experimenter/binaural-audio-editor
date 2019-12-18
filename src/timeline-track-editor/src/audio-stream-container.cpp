#include "audio-stream-container.h"

#include <fstream>
#include <stdio.h>


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
    sfinfo.channels = channels; m_channels = channels;
    sfinfo.samplerate = sample_rate; m_sample_rate = sample_rate;
    sfinfo.format = format; m_format = format;
    
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
	
	sf_seek(outFile, 0, SEEK_SET);
	write_count = sf_write_double(outFile, stream_audio_data.front(), readSize);
	
	sf_close(outFile);
}

int AudioStreamContainer::GetFormat(){return m_format;}
int AudioStreamContainer::GetChannels(){return m_channels;}
int AudioStreamContainer::GetSampleRate(){return m_sample_rate;}

void AudioStreamContainer::ClearStreamDataStored()
{
	//clear audio data stored
	stream_audio_data.clear();
}

void AudioStreamContainer::ClearDataInStreamFile(std::string filename)
{
	//clear audio data stored in file
	if( remove( filename.c_str() ) != 0 )
	{
		std::cout << "\nError deleting file\n";
	}
    else
    {
		std::cout << "\nFile successfully deleted.\n";
	}
    
}
