#include "openalsoft-player.h"

/*
 * Code Adapted from OpenAL Audio Stream Example
 * and OpenAL Source Play Example and HRTF example
 *
 * Copyright (c) 2017 by Chris Robinson <chris.kcat@gmail.com>
 * Copyright (c) 2011 by Chris Robinson <chris.kcat@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

OpenALSoftPlayer::OpenALSoftPlayer()
{
    infile = nullptr;
    bit_size = 0;
    buffer_size = 0;
    buffer_index = 0;
}

OpenALSoftPlayer::~OpenALSoftPlayer()
{
	ClosePlayerFile();

    alDeleteBuffers(NUM_BUFFERS, buffers);
    if(alGetError() != AL_NO_ERROR)
    {
		fprintf(stderr, "Failed to delete object buffer IDs\n");
	}
	
	
	buffer_size = 0;
	infile = nullptr;
}

void OpenALSoftPlayer::InitBuffersForStreaming()
{
	// Generate the buffers
    alGenBuffers(NUM_BUFFERS, buffers);
    assert(alGetError() == AL_NO_ERROR && "Could not create buffers");
}

bool OpenALSoftPlayer::InitOpenALSoft(ALCdevice* thisAudioDevice, ALCcontext* thisAudioContext)
{
	//use default audio device
	thisAudioDevice = alcOpenDevice(NULL);
	if(thisAudioDevice == NULL)
	{
		std::cout << "OpenAL Soft was unable to initialize! \n";
		std::cout << "Unable to initialize default audio device! \n";
		return false;
	}

	//create context
	thisAudioContext = alcCreateContext(thisAudioDevice, NULL);
	if(thisAudioContext == NULL)
	{
		std::cout << "OpenAL Soft was unable to initialize! \n";
		std::cout << "Unable to initialize OpenAL context! \n";
		return false;
	}
	else{alcMakeContextCurrent(thisAudioContext);}

	/* Check if an error occured, and clean up if so. */
	ALenum err; //variable for error flag
	err = alGetError();
	if(err != AL_NO_ERROR)
	{
		//fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
		std::cout << "OpenAL Soft was unable to initialize! \n";
		std::cout << "OpenAL Error in initializing OpenAL Soft: ";
		std::cout << alGetString(err);
		std::cout << "\n";
		return false;
	}
	
	std::cout << " Open AL Soft Initialized! \n";
	return true;
}

void OpenALSoftPlayer::SetReferenceToAudioDevice(ALCdevice* thisAudioDevice){audioDevicePtr = thisAudioDevice;}
void OpenALSoftPlayer::SetReferenceToAudioContext(ALCcontext* thisContext){alContextPtr = thisContext;}

void OpenALSoftPlayer::InitSource(ALuint* source)
{
	alGenSources(1, source);
    assert(alGetError() == AL_NO_ERROR && "Could not create source");

    /* Set parameters so mono sources play out the front-center speaker and
     * won't distance attenuate. */
    alSource3i(*source, AL_POSITION, 0, 0, -1);
    alSourcei(*source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(*source, AL_ROLLOFF_FACTOR, 0);
	assert(alGetError() == AL_NO_ERROR && "Could not set source parameters");
}

void OpenALSoftPlayer::CloseOpenALSoft(ALCdevice* thisAudioDevice, ALCcontext* thisAudioContext)
{
	std::cout << "Close OpenAL Soft called!\n";
	alcDestroyContext(thisAudioContext);	//delete context
	alcCloseDevice(thisAudioDevice);	//close device
}

int OpenALSoftPlayer::OpenPlayerFile(const char *filename)
{
    uint32_t frame_size;
    
    OpenALSoftPlayer::ClosePlayerFile();

	/* The SF_INFO struct must be initialized before using it.*/
	memset (&sfinfo, 0, sizeof (sfinfo)) ;

    /* Open the file and get the first stream from it */
	if (! (infile = sf_open (filename, SFM_READ, &sfinfo)))
	{
		std::cout << "Unable to open file" << filename << "\n";
		std::string error;
		error.append(sf_strerror(NULL));
		std::cout << error << std::endl;
		return 0;
	 }
	 
	 std::cout << "File opened successfully for streaming.\n";

    /* Get the stream format, and figure out the OpenAL format */
    
    if (sfinfo.channels > MAX_CHANNELS)
	{	std::cout << "Not able to process more than "; 
		std::cout << std::to_string(MAX_CHANNELS); 
		std::cout << "%d channels.\n";
		return 0;
	}

	/* Get the sound format, and figure out the OpenAL format */

	//check for number of channels
	//if audio sample has only 1 channel
	if(sfinfo.channels == 1)
	{
		//if audio sample format is 8-bit, set ALenum format variable to mono 8-bit
		if( (sfinfo.format & SF_FORMAT_SUBMASK) == SF_FORMAT_PCM_S8 ||
				(sfinfo.format & SF_FORMAT_SUBMASK) == SF_FORMAT_PCM_U8){format = AL_FORMAT_MONO8;}
		//if audio sample format is 16-bit, set ALenum format variable to mono 8-bit
		else if( (sfinfo.format & SF_FORMAT_SUBMASK) == SF_FORMAT_PCM_16){format = AL_FORMAT_MONO16;}
		//else notify that sample format is unsupported
		else
		{
			std::cout << "Unsupported sample format for mono. Must be 8-bit or 16-bit";
			return 0;
		}
	}
	//else if audio sample has 2 channels
	else if(sfinfo.channels == 2)
	{
		//if audio sample format is 8-bit, set ALenum format variable to stereo 8-bit
		if( (sfinfo.format & SF_FORMAT_SUBMASK) == SF_FORMAT_PCM_S8 ||
			(sfinfo.format & SF_FORMAT_SUBMASK) == SF_FORMAT_PCM_U8){format = AL_FORMAT_STEREO8; }
		//if audio sample format is 16-bit, set ALenum format variable to stereo 8-bit
		else if( (sfinfo.format & SF_FORMAT_SUBMASK) == SF_FORMAT_PCM_16){format = AL_FORMAT_STEREO16;}
		//else notify that sample format is unsupported
		else
		{
			std::cout << "Unsupported sample format for stereo. Must be 8-bit or 16-bit.";
			return 0;
		}
	}
	//else notify that channel count is unsupported
	else
	{
		std::cout << "Unsupported channel count. Must be 1 or 2 channels.\n";
		return 0;
	}
	
    sample_rate = sfinfo.samplerate;
	
	//bit size is the size of a double because that is format for stream.wav
	std::cout << "bit type: " << (sfinfo.format & SF_FORMAT_SUBMASK) << std::endl;
	
	//get audio bit size 
	switch(sfinfo.format & SF_FORMAT_SUBMASK)
	{
		case SF_FORMAT_PCM_S8:{bit_size = sizeof(int8_t); break;}
		case SF_FORMAT_PCM_U8 :{bit_size = sizeof(uint8_t);break;}
		case SF_FORMAT_PCM_16:{bit_size = sizeof(int16_t); break;}
		case SF_FORMAT_PCM_32:{bit_size = sizeof(int32_t); break;}
		case SF_FORMAT_FLOAT:{bit_size = sizeof(float); break;}
		case SF_FORMAT_DOUBLE:{bit_size = sizeof(double); break;}
	}
	
	frame_size = sfinfo.channels * bit_size;

    /* Set the buffer size, given the desired millisecond length. */
    buffer_size = (uint64_t)sample_rate * (double(BUFFER_TIME_MS))/1000 * frame_size;
    std::cout << "buffer size:" << buffer_size << std::endl;
    
    return 1;
}

void OpenALSoftPlayer::ClosePlayerFile()
{
	if(infile != nullptr)
	{
		sf_close (infile);
	}
	
}

/* Prebuffers some audio from the file, and starts playing the source */
int OpenALSoftPlayer::StartPlayerBuffering(ALuint* source, double& current_time)
{
	//std::cout << "In start player!\n";
	
    buffer_index = 0; //reset buffer index

    /* Rewind the source position and clear the buffer queue */
    alSourceRewind(*source);
    alSourcei(*source, AL_BUFFER, 0);
    
    //prevent program from going further if buffer size is zero
    if(buffer_size == 0){return 0;}
    
    //convert time into sample number
    sf_count_t current_sample_number = current_time * sfinfo.samplerate;
    
    //move file to current sample number
    sf_seek(infile, current_sample_number, SEEK_SET);
	
    /* Fill the buffer queue */
    for(buffer_index = 0; buffer_index < NUM_BUFFERS;buffer_index++)
    {
		std::cout << "buffer index:" << buffer_index << std::endl;
		
		//read data differently depending on bit size set in OpenPlayerFile
		
		//if data is 8-bit
		if(bit_size == 1)
		{
			std::vector<int> data;
			std::vector<int> read_buf(buffer_size);
			size_t read_size = 0;
			while((read_size = sf_read_int(infile, read_buf.data(), read_buf.size())) != 0)
			{
				data.insert(data.end(), read_buf.begin(), read_buf.begin() + read_size);
			}

			int slen = data.size() * sizeof(int); //get size of data in bytes

			std::cout << "Number of samples in data buffer : " << slen << "\n";
			//if sample buffer is null or size of buffer data is zero, notify of error
			if(slen == 0)
			{
				std::cout << "Failed to read anymore audio from file. Sample length is 0! \n";
				break;
			}

			// Buffer the audio data into buffer array
		
			//set buffer data
			alBufferData(buffers[buffer_index], format, &data.front(), slen, sfinfo.samplerate);
		}
		
		//if data is 16-bit
		else if(bit_size == 2)
		{
			std::vector<uint16_t> data;
			std::vector<int16_t> read_buf(buffer_size);
			size_t read_size = 0;
			while((read_size = sf_read_short(infile, read_buf.data(), read_buf.size())) != 0)
			{
				data.insert(data.end(), read_buf.begin(), read_buf.begin() + read_size);
			}

			int slen = data.size() * sizeof(uint16_t); //get size of data in bytes

			std::cout << "Number of samples in data buffer : " << slen << "\n";
			//if sample buffer is null or size of buffer data is zero, notify of error
			if(slen == 0)
			{
				std::cout << "Failed to read anymore audio from file. Sample length is 0! \n";
				break;
			}

			// Buffer the audio data into buffer array
		
			//set buffer data
			alBufferData(buffers[buffer_index], format, &data.front(), slen, sfinfo.samplerate);
		}
		//else if data is 32-bit float
		else if(bit_size == 4)
		{
			std::vector<float> data;
			std::vector<float> read_buf(buffer_size);
			size_t read_size = 0;
			while((read_size = sf_read_float(infile, read_buf.data(), read_buf.size())) != 0)
			{
				data.insert(data.end(), read_buf.begin(), read_buf.begin() + read_size);
			}

			int slen = data.size() * sizeof(float); //get size of data in bytes

			std::cout << "Number of samples in data buffer : " << slen << "\n";
			//if sample buffer is null or size of buffer data is zero, notify of error
			if(slen == 0)
			{
				std::cout << "Failed to read anymore audio from file. Sample length is 0! \n";
				break;
			}

			// Buffer the audio data into buffer array
		
			//set buffer data
			alBufferData(buffers[buffer_index], format, &data.front(), slen, sfinfo.samplerate);
		}
		//else if data is 64-bit double
		else if(bit_size == 8)
		{
			std::vector<double> data;
			std::vector<double> read_buf(buffer_size);
			size_t read_size = 0;
			while((read_size = sf_read_double(infile, read_buf.data(), read_buf.size())) != 0)
			{
				data.insert(data.end(), read_buf.begin(), read_buf.begin() + read_size);
			}

			int slen = data.size() * sizeof(double); //get size of data in bytes

			std::cout << "Number of samples in data buffer : " << slen << "\n";
			//if sample buffer is null or size of buffer data is zero, notify of error
			if(slen == 0)
			{
				std::cout << "Failed to read anymore audio from file. Sample length is 0! \n";
				break;
			}

			// Buffer the audio data into buffer array
		
			//set buffer data
			alBufferData(buffers[buffer_index], format, &data.front(), slen, sfinfo.samplerate);
		}
        
    }
    
    if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error buffering for playback\n");
        return PlayerStatus::ERROR_BUFFERING_DATA;
    }
    
    /* Now queue buffer to source */
    alSourceQueueBuffers(*source, buffer_index, buffers);
    
    return PlayerStatus::GOOD_START_BUFFERING;
}

int OpenALSoftPlayer::StartPlayingBuffer(ALuint* source)
{
	//Start playback assume buffer queued
    alSourcePlay(*source);
    if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error starting playback\n");
        return PlayerStatus::ERROR_STARTING_PLAYBACK;
    }
    
    return PlayerStatus::GOOD_PLAYING_STATUS;
}

int OpenALSoftPlayer::UpdatePlayerBuffer(ALuint* source,double& current_time)
{
	 //convert time into sample number
	sf_count_t current_sample_number = current_time * sfinfo.samplerate;
	
	//move file to current sample number
	sf_seek(infile, current_sample_number, SEEK_SET);
	
	//std::cout << "In update player! \n";
    ALint processed, state;

    /* Get relevant source info */
    alGetSourcei(*source, AL_SOURCE_STATE, &state);
    alGetSourcei(*source, AL_BUFFERS_PROCESSED, &processed);
    
    if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error checking source state.\n");
        return PlayerStatus::ERROR_CHECKING_SOURCE_STATE;
    }

    /* Unqueue and handle each processed buffer */
    while(processed > 0)
    {
        ALuint bufid;
        uint32_t slen;

        alSourceUnqueueBuffers(*source, 1, &bufid);
        processed--;

        /* Read the next chunk of data, refill the buffer, and queue it
         * back on the source */
         
         //read data differently depending on the bit size set in OpenPlayerFile
         switch(bit_size)
         {
			 //if 8-bit
			 case 1:
			 {
				  //setup data for buffer
				std::vector<int> data;
				std::vector<int> read_buf(buffer_size);
				size_t read_size = 0;
				while((read_size = sf_read_int(infile, read_buf.data(), read_buf.size())) != 0)
				{
					data.insert(data.end(), read_buf.begin(), read_buf.begin() + read_size);
				}

				slen = data.size() * sizeof(int); //get size of data in bytes
				std::cout << "Size of data in bytes: " << slen << "\n";
				//if sample buffer is null or size of buffer data is zero, notify of error
				if(slen == 0)
				{
					std::cout << "Failed to read anymore audio from file.\n";
					return PlayerStatus::FAILED_TO_READ_ANYMORE_AUDIO_FROM_FILE;
				}

				double seconds = (1.0 * sfinfo.frames) / sfinfo.samplerate ;
				std::cout << "Duration of sound:" << seconds << "s. \n";

				if(slen > 0)
				{
					alBufferData(bufid, format, &data.front(), slen, sfinfo.samplerate);
					alSourceQueueBuffers(*source, 1, &bufid);
				}
				if(alGetError() != AL_NO_ERROR)
				{
					fprintf(stderr, "Error buffering data\n");
					return PlayerStatus::ERROR_BUFFERING_DATA;
				}
				break;
			 }
			 
			 //if 16-bit
			 case 2:
			 {
				 //setup data for buffer
				std::vector<uint16_t> data;
				std::vector<int16_t> read_buf(buffer_size);
				size_t read_size = 0;
				while((read_size = sf_read_short(infile, read_buf.data(), read_buf.size())) != 0)
				{
					data.insert(data.end(), read_buf.begin(), read_buf.begin() + read_size);
				}

				slen = data.size() * sizeof(uint16_t); //get size of data in bytes
				std::cout << "Size of data in bytes: " << slen << "\n";
				//if sample buffer is null or size of buffer data is zero, notify of error
				if(slen == 0)
				{
					std::cout << "Failed to read anymore audio from file.\n";
					return PlayerStatus::FAILED_TO_READ_ANYMORE_AUDIO_FROM_FILE;
				}

				double seconds = (1.0 * sfinfo.frames) / sfinfo.samplerate ;
				std::cout << "Duration of sound:" << seconds << "s. \n";

				if(slen > 0)
				{
					alBufferData(bufid, format, &data.front(), slen, sfinfo.samplerate);
					alSourceQueueBuffers(*source, 1, &bufid);
				}
				if(alGetError() != AL_NO_ERROR)
				{
					fprintf(stderr, "Error buffering data\n");
					return PlayerStatus::ERROR_BUFFERING_DATA;
				}
				break;
			 }
			 
			 //if 32-bit float
			 case 4:
			 {
				  //setup data for buffer
				std::vector<float> data;
				std::vector<float> read_buf(buffer_size);
				size_t read_size = 0;
				while((read_size = sf_read_float(infile, read_buf.data(), read_buf.size())) != 0)
				{
					data.insert(data.end(), read_buf.begin(), read_buf.begin() + read_size);
				}

				slen = data.size() * sizeof(float); //get size of data in bytes
				std::cout << "Size of data in bytes: " << slen << "\n";
				//if sample buffer is null or size of buffer data is zero, notify of error
				if(slen == 0)
				{
					std::cout << "Failed to read audio from file.\n";
					return PlayerStatus::FAILED_TO_READ_ANYMORE_AUDIO_FROM_FILE;
				}

				double seconds = (1.0 * sfinfo.frames) / sfinfo.samplerate ;
				std::cout << "Duration of sound:" << seconds << "s. \n";

				if(slen > 0)
				{
					alBufferData(bufid, format, &data.front(), slen, sfinfo.samplerate);
					alSourceQueueBuffers(*source, 1, &bufid);
				}
				if(alGetError() != AL_NO_ERROR)
				{
					fprintf(stderr, "Error buffering data\n");
					return PlayerStatus::ERROR_BUFFERING_DATA;
				}
				break;
			 }
			 
			 //if 64-bit double
			 case 8:
			 {
				   //setup data for buffer
				std::vector<double> data;
				std::vector<double> read_buf(buffer_size);
				size_t read_size = 0;
				while((read_size = sf_read_double(infile, read_buf.data(), read_buf.size())) != 0)
				{
					data.insert(data.end(), read_buf.begin(), read_buf.begin() + read_size);
				}

				slen = data.size() * sizeof(double); //get size of data in bytes
				std::cout << "Size of data in bytes: " << slen << "\n";
				//if sample buffer is null or size of buffer data is zero, notify of error
				if(slen == 0)
				{
					std::cout << "Failed to read anymore audio from file.\n";
					return PlayerStatus::FAILED_TO_READ_ANYMORE_AUDIO_FROM_FILE;
				}

				double seconds = (1.0 * sfinfo.frames) / sfinfo.samplerate ;
				std::cout << "Duration of sound:" << seconds << "s. \n";

				if(slen > 0)
				{
					alBufferData(bufid, format, &data.front(), slen, sfinfo.samplerate);
					alSourceQueueBuffers(*source, 1, &bufid);
				}
				if(alGetError() != AL_NO_ERROR)
				{
					fprintf(stderr, "Error buffering data\n");
					return PlayerStatus::ERROR_BUFFERING_DATA;
				}
				break;
			 }
		 }
    }
    
    return PlayerStatus::GOOD_UPDATE_BUFFER_STATUS;
}

int OpenALSoftPlayer::PlayUpdatedPlayerBuffer(ALuint* source)
{
	ALint state;
	
	alGetSourcei(*source, AL_SOURCE_STATE, &state);
	
	/* Make sure the source hasn't underrun */
    if(state != AL_PLAYING && state != AL_PAUSED)
    {
        ALint queued;

        /* If no buffers are queued, playback is finished */
        alGetSourcei(*source, AL_BUFFERS_QUEUED, &queued);
        if(queued == 0)
        {
			return PlayerStatus::PLAYBACK_FINISHED;
		}

        alSourcePlay(*source);
        if(alGetError() != AL_NO_ERROR)
        {
            fprintf(stderr, "Error restarting playback\n");
            return PlayerStatus::ERROR_RESTARTING_PLAYBACK;
        }
    }
    
    return GOOD_PLAYING_STATUS;
}

int OpenALSoftPlayer::PlayMultipleUpdatedPlayerBuffers(std::vector <ALuint*> *sources_vec)
{	
	//vector containing queued status of each source
	std::vector <ALint> queued_source_vector;
	queued_source_vector.resize(sources_vec->size());
	
	//vector containing sources that are eligible to be played i.e. no underrun 
	std::vector <ALuint> sources_to_play_vec;
	
	for(size_t i = 0; i < sources_vec->size(); i++)
	{
		ALint state;
		
		//get state of source
		alGetSourcei(*(sources_vec->at(i)), AL_SOURCE_STATE, &state);
		
		/* Make sure the source hasn't underrun */
		if(state != AL_PLAYING && state != AL_PAUSED)
		{
			ALint queued;
			
			alGetSourcei(*(sources_vec->at(i)), AL_BUFFERS_QUEUED, &queued);
			queued_source_vector[i] = queued;
		}
		
	}
	
	for(size_t i = 0; i < queued_source_vector.size(); i++)
	{
		//if source not underrun 
		if(queued_source_vector[i] != 0)
		{
			//add it to vector of eligible sources to be played
			//if source is valid
			if(alIsSource( *(sources_vec->at(i)) ) == AL_TRUE )
			{
				sources_to_play_vec.push_back( *(sources_vec->at(i)) );
			}
			else
			{
				std::cout << "Invalid source name:" << sources_vec->at(i) << std::endl;
			}
		}
	}
	
	//if there are no elegible sources to play
	if(sources_to_play_vec.size() == 0)
	{
		/* If no buffers are queued for all sources, playback is finished */
		return PlayerStatus::PLAYBACK_FINISHED;
	}
	//else if there are eligible sources to play
    else
    {
		 //play sources
		ALsizei n = sources_to_play_vec.size();
		
		const ALuint *sNames = &sources_to_play_vec.at(0);
		alSourcePlayv(n, sNames);
		
		ALenum err = alGetError();
		if(err != AL_NO_ERROR)
		{
			fprintf(stderr, "Error restarting playback in PlayMultipleUpdatedPlayerBuffers.\n");
			fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
			return PlayerStatus::ERROR_RESTARTING_PLAYBACK;
		}
	}
   
    return GOOD_PLAYING_STATUS;
}


void OpenALSoftPlayer::PlaySource(ALuint* thisSource)
{
	alSourcePlay(*thisSource);
	if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error starting playback\n");
    }
}

void OpenALSoftPlayer::PlayMultipleSources(std::vector <ALuint*> *sources_vec)
{
	ALsizei n = sources_vec->size();
	const ALuint *sNames = sources_vec->at(0);
	alSourcePlayv(n,sNames);
	if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error restarting playback in PlayMultipleSources.\n");
    }
}

void OpenALSoftPlayer::PauseSource(ALuint* thisSource)
{
	alSourcePause(*thisSource);
}

void OpenALSoftPlayer::PauseMultipleSources(std::vector <ALuint*> *sources_vec)
{
	ALsizei n = sources_vec->size();
	const ALuint *sNames = sources_vec->at(0);
	alSourcePausev (n,sNames);
}


void OpenALSoftPlayer::RewindSource(ALuint* thisSource)
{
	alSourceRewind(*thisSource);
}

void OpenALSoftPlayer::RewindMultipleSources(std::vector <ALuint*> *sources_vec)
{
	ALsizei n = sources_vec->size();
	const ALuint *sNames = sources_vec->at(0);
	alSourceRewindv (n, sNames);
}
	

void OpenALSoftPlayer::StopSource(ALuint* thisSource)
{
	alSourceStop(*thisSource);
}

void OpenALSoftPlayer::StopMultipleSources(std::vector <ALuint*> *sources_vec)
{
	ALsizei n = sources_vec->size();
	const ALuint *sNames = sources_vec->at(0);
	alSourceStopv(n,sNames);
}

void OpenALSoftPlayer::ClearQueue(ALuint* thisSource)
{
	alSourcei(*thisSource, AL_BUFFER, 0);
}
