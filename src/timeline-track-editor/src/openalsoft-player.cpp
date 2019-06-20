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
	
	//frame size is number of channels * audio bit size divided by 8
	frame_size = sfinfo.channels * (sfinfo.format & 0xFF) / 8;

    /* Set the buffer size, given the desired millisecond length. */
    buffer_size = (uint64_t)sample_rate * BUFFER_TIME_MS/1000 * frame_size;
    
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
int OpenALSoftPlayer::StartPlayer(ALuint* source)
{
    size_t i;

    /* Rewind the source position and clear the buffer queue */
    alSourceRewind(*source);
    alSourcei(*source, AL_BUFFER, 0);
    
    if(buffer_size != 0)
    {
		return 0;
	}
	
    /* Fill the buffer queue */
    for(i = 0;i < NUM_BUFFERS;i++)
    {
         //setup data for buffer
		std::vector<uint16_t> data;
		std::vector<int16_t> read_buf(buffer_size);
		size_t read_size = 0;
		while((read_size = sf_read_short(infile, read_buf.data(), read_buf.size())) != 0)
		{
			data.insert(data.end(), read_buf.begin(), read_buf.begin() + read_size);
		}

		uint32_t slen = data.size() * sizeof(uint16_t); //get size of data in bytes

		std::cout << "Size of data in bytes: " << slen << "\n";
		//if sample buffer is null or size of buffer data is zero, notify of error
		if(slen == 0)
		{
			std::cout << "Failed to read audio from file.\n";
			return 0;
		}

		double seconds = (1.0 * sfinfo.frames) / sfinfo.samplerate ;
		std::cout << "Duration of sound:" << seconds << "s. \n";

		// Buffer the audio data into buffer array
	
		//set buffer data
		//alBufferData(buffer, format, data, slen, frequency);
		alBufferData(buffers[i], format, &data.front(), slen, sfinfo.samplerate);
    }
    
    if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error buffering for playback\n");
        return 0;
    }
    

    /* Now queue and start playback! */
    alSourceQueueBuffers(*source, i, buffers);
    alSourcePlay(*source);
    if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error starting playback\n");
        return 0;
    }

    return 1;
}

int OpenALSoftPlayer::UpdatePlayer(ALuint* source)
{
    ALint processed, state;

    /* Get relevant source info */
    alGetSourcei(*source, AL_SOURCE_STATE, &state);
    alGetSourcei(*source, AL_BUFFERS_PROCESSED, &processed);
    
    if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error checking source state\n");
        return 0;
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
			std::cout << "Failed to read audio from file.\n";
			return 0;
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
            return 0;
        }
    }

    /* Make sure the source hasn't underrun */
    if(state != AL_PLAYING && state != AL_PAUSED)
    {
        ALint queued;

        /* If no buffers are queued, playback is finished */
        alGetSourcei(*source, AL_BUFFERS_QUEUED, &queued);
        if(queued == 0)
            return 0;

        alSourcePlay(*source);
        if(alGetError() != AL_NO_ERROR)
        {
            fprintf(stderr, "Error restarting playback\n");
            return 0;
        }
    }

    return 1;
}

void OpenALSoftPlayer::PlaySource(ALuint* thisSource)
{
	alSourcePlay(*thisSource);
}

void OpenALSoftPlayer::PauseSource(ALuint* thisSource)
{
	alSourcePause(*thisSource);
}

void OpenALSoftPlayer::RewindSource(ALuint* thisSource)
{
	alSourceRewind(*thisSource);
}

void OpenALSoftPlayer::StopSource(ALuint* thisSource)
{
	alSourceStop(*thisSource);
}


