#include "audio-track.h"

/*
 * Code adapted from sfprocess.c on libsndfile github
** Copyright (C) 2001-2013 Erik de Castro Lopo <erikd@mega-nerd.com>
**
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**
**	 * Redistributions of source code must retain the above copyright
**	   notice, this list of conditions and the following disclaimer.
**	 * Redistributions in binary form must reproduce the above copyright
**	   notice, this list of conditions and the following disclaimer in
**	   the documentation and/or other materials provided with the
**	   distribution.
**	 * Neither the author nor the names of any contributors may be used
**	   to endorse or promote products derived from this software without
**	   specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
** TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
** PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
** CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
** EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
** OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
** WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
** OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
** ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

AudioTrack::AudioTrack(const wxString& title) : Track(title)
{
	track_state = PLAYER_NULL;
	sourceToManipulatePtr = nullptr;
	audioPlayerPtr = nullptr;
	
	m_audio_graph = nullptr;
	
	
	playbackControlsPtr = nullptr;
	
}

//Audio related functions

void AudioTrack::SetReferenceToSourceToManipulate(ALuint* thisSource){sourceToManipulatePtr = thisSource;}

void AudioTrack::SetReferenceToAudioPlayer(OpenALSoftPlayer* thisPlayer){audioPlayerPtr = thisPlayer;}


int AudioTrack::GetNumberOfChannelsInAudioFile(std::string filepath,SF_INFO& input_sfinfo)
{
	int channels = 0;
	
	//Read data from file
	if (! (inputFile = sf_open (filepath.c_str(), SFM_READ, &input_sfinfo)))
	{	
		// Open failed, so print an error message.
		std::cout << "Not able to open input file " <<  filepath << std::endl;
		/* Print the error message from libsndfile. */
		puts (sf_strerror (NULL)) ;
		return 0;
	} 
	
	channels = input_sfinfo.channels;
	
	/* Close input file. */
	sf_close(inputFile);
	
	return channels;
}

void AudioTrack::ReadAndCopyDataFromInputFile(std::vector<double> *audio_data_input_copy_ptr,std::string inputSoundFilePath,SF_INFO& input_sfinfo)
{
	//Read data from file
	if (! (inputFile = sf_open (inputSoundFilePath.c_str(), SFM_READ, &input_sfinfo)))
	{	
		// Open failed, so print an error message.
		std::cout << "Not able to open input file " <<  inputSoundFilePath << std::endl;
		/* Print the error message from libsndfile. */
		puts (sf_strerror (NULL)) ;
		return;
	} 
		
	if (input_sfinfo.channels > MAX_CHANNELS)
	{
		std::cout << "Not able to process more than" <<  MAX_CHANNELS << "channels.\n";
		return;
	}
	std::cout << "Successfully loaded " << inputSoundFilePath << " saving data..." << std::endl;
	
	//read input data
	std::vector<double> read_buf(BUFFER_LEN);
	size_t read_size = 0;
	
	
	while( (read_size = sf_read_double(inputFile, read_buf.data(), read_buf.size()) ) != 0)
	{
		audio_data_input_copy_ptr->insert(audio_data_input_copy_ptr->end(), read_buf.begin(), read_buf.begin() + read_size);
	}
	
	/* Close input and stream files. */
	sf_close(inputFile);
	
	//std::string messageString;
	//messageString.append("Successfully loaded and saved a copy of audio data of");
	//messageString.append(inputSoundFilePath);
	//wxMessageBox( messageString );
}

void AudioTrack::CopyInputDataIntoAudioDataStream(std::vector<double> *audio_data_input_copy_ptr, AudioStreamContainer* audio_data_stream_ptr,std::string streamSoundFilePath,SF_INFO& input_sfinfo)
{
	//copy input audio data references to audio data stream
	audio_data_stream_ptr->ResizeAudioStream(audio_data_input_copy_ptr->size());
	for(size_t i=0; i < audio_data_stream_ptr->GetSize(); i++)
	{
		double* ref_at_i = &(audio_data_input_copy_ptr->at(i));
		audio_data_stream_ptr->SetPointerToDataAtThisSampleIndex(ref_at_i,i);
	}
	
	double slen;
	slen = audio_data_stream_ptr->GetSize() * sizeof(uint16_t);
	
	std::cout << "Size of data in bytes: " << slen << "\n";
	//if sample buffer is null or size of buffer data is zero, notify of error
	if(slen == 0)
	{
		std::string messageString;
		messageString.append("Failed to read audio from file.\n");
		wxMessageBox( messageString );
		return;
	}
	
	double seconds = (1.0 * input_sfinfo.frames) / input_sfinfo.samplerate ;
	std::cout << "Duration of sound:" << seconds << "s. \n";
	
	audio_data_stream_ptr->WriteStreamContentsToFile(streamSoundFilePath, input_sfinfo.format, input_sfinfo.channels, input_sfinfo.samplerate,int(BUFFER_LEN));
}

void AudioTrack::PlotOneChannelStreamAudioDataToGraph(AudioStreamContainer* audio_data_stream_ptr,SF_INFO& input_sfinfo)
{
	m_audio_graph->PlotOneChannelStreamAudioDataToGraph(audio_data_stream_ptr,input_sfinfo.samplerate,
										verticalStart, verticalEnd, verticalResolution);
	Refresh();
}

void AudioTrack::PlotLeftChannelStreamAudioDataToGraph(AudioStreamContainer* audio_data_stream_ptr,SF_INFO& input_sfinfo)
{
	m_audio_graph->PlotLeftChannelStreamAudioDataToGraph(audio_data_stream_ptr,input_sfinfo.samplerate,
										verticalStart, verticalEnd, verticalResolution);
	Refresh();
}

void AudioTrack::PlotRightChannelStreamAudioDataToGraph(AudioStreamContainer* audio_data_stream_ptr,SF_INFO& input_sfinfo)
{
	m_audio_graph->PlotRightChannelStreamAudioDataToGraph(audio_data_stream_ptr,input_sfinfo.samplerate,
										verticalStart, verticalEnd, verticalResolution);
	Refresh();
}

//Track related functions

void AudioTrack::al_nssleep(unsigned long nsec)
{
    struct timespec ts, rem;
    ts.tv_sec = nsec / 1000000000ul;
    ts.tv_nsec = nsec % 1000000000ul;
    while(nanosleep(&ts, &rem) == -1 && errno == EINTR){ts = rem;}
}

void AudioTrack::FunctionToCallInPlayState()
{

}

void AudioTrack::FunctionToCallInPauseState()
{
	if(AudioTrack::GetAudioTrackState() != PLAYER_PAUSED)
	{
		if(sourceToManipulatePtr != nullptr && audioPlayerPtr != nullptr)
		{
			audioPlayerPtr->PauseSource(sourceToManipulatePtr);
			AudioTrack::SetAudioTrackState(PLAYER_PAUSED);
		}	
	}
}

void AudioTrack::FunctionToCallInRewindState()
{
	if(sourceToManipulatePtr != nullptr && audioPlayerPtr != nullptr)
	{
		alSourceRewind(*sourceToManipulatePtr);
		AudioTrack::SetAudioTrackState(PLAYER_REWINDING);
	}
}

void AudioTrack::FunctionToCallInFastForwardState()
{
	
}

void AudioTrack::FunctionToCallInNullState()
{
	
}

void AudioTrack::SetFunctionToCallAfterVariableChange(std::function < void() > thisFunction){func_after_var_change = thisFunction;} 

void AudioTrack::render(wxDC& dc)
{
	m_audio_graph->render(dc,&m_vertical_var_num);
}

void AudioTrack::logic_left_click()
{
	
}

void AudioTrack::logic_right_click()
{
	
}

void AudioTrack::InitTrack(wxWindow* parent, std::vector <int> *timeTickVector)
{
	
	this->Create(parent, wxID_ANY, wxPoint(wxDefaultPosition.x,wxDefaultPosition.y + 50), wxSize(TRACK_WIDTH, TRACK_HEIGHT),wxTAB_TRAVERSAL,AudioTrack::GetTitle());
	this->SetBackgroundColour( *wxLIGHT_GREY );
	AudioTrack::SetReferenceToTimeTickVector(timeTickVector);
	
	//initialize audio graph for left channel
	m_audio_graph = new AudioGraph(this);
	m_audio_graph->SetReferenceToTimeTickVector(timeTickVector);
	
}

void AudioTrack::SetupAxisForVariable(double& start, double& end, double& resolution, int& numTick)
{
	verticalStart = start;
	verticalEnd = end;
	verticalNumTicks = numTick;
	verticalResolution = resolution;
	
	//setup tick marks
	AudioTrack::InitVerticalAxis();
}

void AudioTrack::InitVerticalAxis()
{
	//get linearly spaced vector of doubles
	std::vector <double> thisVector = AudioTrack::LinearSpacedArray(verticalStart,verticalEnd,(size_t)verticalNumTicks);
	
	for(size_t i=0; i < thisVector.size(); ++i)
	{
		m_vertical_var_num.push_back(thisVector[i]);
	}
}

std::vector<double> AudioTrack::LinearSpacedArray(double a, double b, std::size_t N)
{
	// Linear interpolation following MATLAB linspace
	double h = (b - a) / static_cast<double>(N-1);
	std::vector<double> xs(N);
	std::vector<double>::iterator x;
	double val;
	for (x = xs.begin(), val = a; x != xs.end(); ++x, val += h) {
		*x = val;
	}
	return xs;
}

void AudioTrack::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	AudioTrack::render(dc);
}

void AudioTrack::OnScroll(wxScrollEvent& event)
{
	Refresh(); //for wxDc onPaint stuff 
	
	FitInside(); //for scroll and sizer
}

void AudioTrack::OnSize(wxSizeEvent& event)
{
	Refresh();

	FitInside();
}

void AudioTrack::OnLeftMouseClick(wxMouseEvent& event)
{
	
}

void AudioTrack::OnRightMouseClick(wxCommandEvent& event)
{
	
}

void AudioTrack::SetReferenceToCurrentTimeVariable(double* thisTimeVariable){Track::SetReferenceToCurrentTimeVariable(thisTimeVariable);}

void AudioTrack::SetReferenceToTimeTickVector(std::vector <int> *thisVector){ Track::SetReferenceToTimeTickVector(thisVector);}

std::vector <int> *AudioTrack::GetReferenceToTimeTickVector(){return Track::GetReferenceToTimeTickVector();}

void AudioTrack::SetTitle(wxString thisTitle){Track::SetTitle(thisTitle);}

wxString AudioTrack::GetTitle(){return Track::GetTitle();}

double AudioTrack::GetCurrentTime(){return Track::GetCurrentTime();}

void AudioTrack::SetAudioTrackState(int thisState){track_state = thisState;}
int AudioTrack::GetAudioTrackState(){return track_state;}

void AudioTrack::SetReferenceToPlaybackControls(PlaybackControls* controls){playbackControlsPtr = controls;}
PlaybackControls* AudioTrack::GetReferenceToPlaybackControls(){return playbackControlsPtr;}

void AudioTrack::ClearGraph()
{
	m_audio_graph->ClearGraph();
	Refresh();
}


