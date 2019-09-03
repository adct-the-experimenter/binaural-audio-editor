#include "openalsoftaudioengine.h"


/*
 * Code adapted from OpenAL Source Play Example and HRTF example
 *
 * Copyright (c) 2017 by Chris Robinson <chris.kcat@gmail.com>
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

OpenAlSoftAudioEngine::OpenAlSoftAudioEngine()
{
	
	//Initialize sound setup variables to point to nullptr
	gAudioDevice = nullptr;
	alContext = nullptr;

    //clear testHRTF Results string
    testHRTF_Results.clear();
    loadSound_Results.clear();
}

OpenAlSoftAudioEngine::~OpenAlSoftAudioEngine()
{
    OpenAlSoftAudioEngine::close_openALSoft();
}


bool OpenAlSoftAudioEngine::initOpenALSoft()
{
	
	//use default audio device
	gAudioDevice = alcOpenDevice(NULL);
	if(gAudioDevice == NULL)
	{
		std::cout << "OpenAL Soft was unable to initialize! \n";
		std::cout << "Unable to initialize default audio device! \n";
		std::cout << alGetString(alGetError()) << std::endl;
		return false;
	}

	//create context
	alContext = alcCreateContext(gAudioDevice, NULL);
	if(alContext == NULL)
	{
		std::cout << "OpenAL Soft was unable to initialize! \n";
		std::cout << "Unable to initialize OpenAL context! \n";
		std::cout << alGetString(alGetError()) << std::endl;
		return false;
	}
	else{alcMakeContextCurrent(alContext);}

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



void OpenAlSoftAudioEngine::close_openALSoft()
{		
	alcDestroyContext(alContext);	//delete context
	alcCloseDevice(gAudioDevice);	//close device
}

ALCdevice* OpenAlSoftAudioEngine::GetReferenceToAudioDevice(){return gAudioDevice;}
ALCcontext* OpenAlSoftAudioEngine::GetReferenceToAudioContext(){return alContext;}


//HRTF
//define functions for hrtf
static LPALCGETSTRINGISOFT alcGetStringiSOFT;
static LPALCRESETDEVICESOFT alcResetDeviceSOFT;


void OpenAlSoftAudioEngine::TestHRTF()
{

    //call after initOpenALSoft

    ALCint hrtf_state; //state of hrtf, enabled or disabled
    ALCint num_hrtf; //variable for number of hrtf avaiable

    /* Check for HRTF support. */
	
		
	if(!alcIsExtensionPresent(gAudioDevice, "ALC_SOFT_HRTF"))
	{
		//fprintf(stderr, "Error: ALC_SOFT_HRTF not supported\n");
		testHRTF_Results.append("Error: ALC_SOFT_HRTF not supported! \n");
	}
	else
	{
		testHRTF_Results.append(" ALC_SOFT_HRTF is supported! \n");

		/* Define a macro to help load the function pointers. */
		#define LOAD_PROC_alcGetStringiSOFT(d, x)  ((x) = (LPALCGETSTRINGISOFT)alcGetProcAddress((d), #x))
			LOAD_PROC_alcGetStringiSOFT(gAudioDevice, alcGetStringiSOFT);
		#undef LOAD_PROC__alcGetStringiSOFT

		#define LOAD_PROC_alcResetDeviceSOFT(d, x)  ((x) = (LPALCRESETDEVICESOFT)alcGetProcAddress((d), #x))
			LOAD_PROC_alcResetDeviceSOFT(gAudioDevice, alcResetDeviceSOFT);
		#undef LOAD_PROC_alcResetDeviceSOFT

		/* Check for the AL_EXT_STEREO_ANGLES extension to be able to also rotate
		 * stereo sources.
		 */
		ALboolean has_angle_ext = alIsExtensionPresent("AL_EXT_STEREO_ANGLES");

		//printf("AL_EXT_STEREO_ANGLES%s found\n", has_angle_ext?"":" not");
		if(has_angle_ext )
		{
			testHRTF_Results.append("AL_EXT_STEREO_ANGLES support found. Stereo sources can be rotated. \n");
		}
		else
		{
			testHRTF_Results.append("AL_EXT_STEREO_ANGLES support not found. Stereo sources cannot be rotated. \n");
		}


		/* Enumerate available HRTFs, and reset the device using one. */
		alcGetIntegerv(gAudioDevice, ALC_NUM_HRTF_SPECIFIERS_SOFT, 1, &num_hrtf);
		if(!num_hrtf)
		{
			testHRTF_Results.append("No HRTFs found. \n");
		}
		else
		{
			ALCint i; //iterator

			testHRTF_Results.append("Available HRTFs:\n");
			for(i = 0;i < num_hrtf;i++)
			{
				const ALCchar *name = alcGetStringiSOFT(gAudioDevice, ALC_HRTF_SPECIFIER_SOFT, i);

				testHRTF_Results.append(name);
				testHRTF_Results.append(" ");
				testHRTF_Results.append(std::to_string(i));
				testHRTF_Results.append(" \n");
			}
		}

		/* Check if HRTF is enabled, and show which is being used. */
		alcGetIntegerv(gAudioDevice, ALC_HRTF_SOFT, 1, &hrtf_state);
		if(!hrtf_state)
		{
			testHRTF_Results.append("HRTF not enabled!\n");
		}
		else
		{
			const ALchar *name = alcGetString(gAudioDevice, ALC_HRTF_SPECIFIER_SOFT);
			
			testHRTF_Results.append("HRTF enabled, using ");
			testHRTF_Results.append(name);
			testHRTF_Results.append("\n");
		}
	}
	
}

std::string OpenAlSoftAudioEngine::getHRTFTestResult(){return testHRTF_Results;}

void OpenAlSoftAudioEngine::clear_testHRTFResults(){testHRTF_Results.clear();}

void OpenAlSoftAudioEngine::GetAvailableHRTFNames(std::vector <std::string> *names_vector)
{
	
	/* Define a macro to help load the function pointers. */
		#define LOAD_PROC_alcGetStringiSOFT(d, x)  ((x) = (LPALCGETSTRINGISOFT)alcGetProcAddress((d), #x))
			LOAD_PROC_alcGetStringiSOFT(gAudioDevice, alcGetStringiSOFT);
		#undef LOAD_PROC__alcGetStringiSOFT
		
	 ALCint num_hrtf; //variable for number of hrtf avaiable
	
	/* Enumerate available HRTFs, and reset the device using one. */
	alcGetIntegerv(gAudioDevice, ALC_NUM_HRTF_SPECIFIERS_SOFT, 1, &num_hrtf);
	if(!num_hrtf)
	{
		return;
	}
	else
	{
		ALCint i; //iterator

		for(i = 0;i < num_hrtf;i++)
		{
			const ALCchar *name = alcGetStringiSOFT(gAudioDevice, ALC_HRTF_SPECIFIER_SOFT, i);
			
			std::string name_string(name);
			names_vector->push_back(name_string);
		}
	}
}

std::string OpenAlSoftAudioEngine::GetCurrentHRTFSelected()
{
	/* Check if HRTF is enabled, and show which is being used. */
	const ALchar *name = alcGetString(gAudioDevice, ALC_HRTF_SPECIFIER_SOFT);
	
	std::string name_string(name);
	
	return name_string;
}

void OpenAlSoftAudioEngine::SelectThisHRTFByIndex(int& index,std::string& message)
{
	/* Define a macro to help load the function pointers. */
		#define LOAD_PROC_alcGetStringiSOFT(d, x)  ((x) = (LPALCGETSTRINGISOFT)alcGetProcAddress((d), #x))
			LOAD_PROC_alcGetStringiSOFT(gAudioDevice, alcGetStringiSOFT);
		#undef LOAD_PROC__alcGetStringiSOFT

		#define LOAD_PROC_alcResetDeviceSOFT(d, x)  ((x) = (LPALCRESETDEVICESOFT)alcGetProcAddress((d), #x))
			LOAD_PROC_alcResetDeviceSOFT(gAudioDevice, alcResetDeviceSOFT);
		#undef LOAD_PROC_alcResetDeviceSOFT
		
	ALCint i;
	ALCint attr[5];
	
	i = 0;
	attr[i++] = ALC_HRTF_SOFT;
	attr[i++] = ALC_TRUE;
	attr[i++] = ALC_HRTF_ID_SOFT;
    attr[i++] = index;
    attr[i] = 0;
    
    if(!alcResetDeviceSOFT(gAudioDevice, attr))
    {
		message = "Failed to reset device: \n" + std::string( alcGetString(gAudioDevice, alcGetError(gAudioDevice)) );}
    else{message = "Successfully changed HRTF and reset device.";}
}

void OpenAlSoftAudioEngine::loadSound(ALuint* buffer, const std::string& filename)
{
	
	std::cout << "Load Sound called! \n";
	/*Code modified from Chris Robinson's OpenAL-Soft example for loading sound file.*/
	
	ALuint tempBuffer;
	ALenum err; //variable for error flag
	ALenum format; //ALenum variable for format of sound sample e.g. 8-bit 16-bit
	ALsizei slen; //Size in bytes of the buffer data.
	ALsizei frequency; //Sample rate of the buffer data

	
	//libsndfile code below adapted from sfprocess.c of libsndfile example on github.
	
	//libsndfile file handle for input file
	SNDFILE	*infile;

	/* A pointer to an SF_INFO struct is passed to sf_open.
	** On read, the library fills this struct with information about the file.
	*/
	SF_INFO	sfinfo;
	const char	*infilename = filename.c_str();

	/* The SF_INFO struct must be initialized before using it.
	*/
	memset (&sfinfo, 0, sizeof (sfinfo)) ;

	/* Here's where we open the input file. We pass sf_open the file name and
	** a pointer to an SF_INFO struct.
	** On successful open, sf_open returns a SNDFILE* pointer which is used
	** for all subsequent operations on that file.
	** If an error occurs during sf_open, the function returns a NULL pointer.
	**
	*/

	if (! (infile = sf_open (infilename, SFM_READ, &sfinfo)))
	{
		/* Open failed so print an error message. */
		loadSound_Results.append("Not able to open input file "); loadSound_Results.append(infilename); loadSound_Results.append(".\n");
		/* Print the error message from libsndfile. */
		loadSound_Results.append(sf_strerror (NULL)) ;
		return;
	 }

	if (sfinfo.channels > MAX_CHANNELS)
	{	loadSound_Results.append("Not able to process more than "); 
		loadSound_Results.append(std::to_string(MAX_CHANNELS)); 
		loadSound_Results.append("%d channels.\n");
		return;
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
			loadSound_Results.append("Unsupported sample format for mono. Must be 8-bit or 16-bit");
			return;
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
			loadSound_Results.append("Unsupported sample format for stereo. Must be 8-bit or 16-bit.");
			return;
		}

	}
	//else notify that channel count is unsupported
	else
	{
		loadSound_Results.append("Unsupported channel count. Must be 1 or 2 channels.\n");
		return;
	}

	/* Decode the whole audio stream to a buffer. */

	frequency = sfinfo.samplerate;
	std::cout << "Sample Rate: " << frequency << "\n";

	//setup data for buffer
	std::vector<uint16_t> data;
	std::vector<int16_t> read_buf(BUFFER_LEN);
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
		loadSound_Results.append("Failed to read audio from file.\n");
		return;
	}

	double seconds = (1.0 * sfinfo.frames) / sfinfo.samplerate ;
	std::cout << "Duration of sound:" << seconds << "s. \n";

	/* Buffer the audio data into a new buffer object, then free the data and
	 * close the file. */
	tempBuffer = 0; //initialize temp buffer
			
	alGenBuffers(1, &tempBuffer);//request 1 buffer
	
	//set buffer data
	//alBufferData(buffer, format, data, slen, frequency);
	alBufferData(tempBuffer, format,&data.front(), data.size() * sizeof(uint16_t), sfinfo.samplerate);
	
	

	/* Check if an error occured, and clean up if so. */
	err = alGetError();
	
	
	if(err != AL_NO_ERROR)
	{
		//fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
		loadSound_Results.append("OpenAL Error in loading sound: ");
		loadSound_Results.append(alGetString(err));
		loadSound_Results.append("\n");
		if(tempBuffer && alIsBuffer(tempBuffer)){alDeleteBuffers(1, &tempBuffer);}
		return;
	}
	loadSound_Results.append("Loaded "); loadSound_Results.append(filename); loadSound_Results.append(" successfully! \n");
	*buffer = tempBuffer; //assign temp buffer to buffer

	/* Close input files. */
	sf_close (infile);
}


std::string OpenAlSoftAudioEngine::getLoadSoundResult(){return loadSound_Results;}

void OpenAlSoftAudioEngine::clear_LoadSoundResults(){loadSound_Results.clear();}


void OpenAlSoftAudioEngine::createSource(ALuint* buffer,ALuint* source)
{
	/* Create the source to play the sound with. */
	
	//if buffer is defined
	if(*buffer != 0)
	{
		alGenSources(1, source);
		alSourcei(*source, AL_SOURCE_RELATIVE, AL_TRUE);
		alSource3f(*source, AL_POSITION, 0.0f, 0.0f, -1.0f);
		alSourcei(*source, AL_BUFFER, *buffer);
		assert(alGetError()==AL_NO_ERROR && "Failed to setup sound source.");
	}
	
}

void al_nssleep(unsigned long nsec)
{
    struct timespec ts, rem;
    ts.tv_sec = nsec / 1000000000ul;
    ts.tv_nsec = nsec % 1000000000ul;
    while(nanosleep(&ts, &rem) == -1 && errno == EINTR)
        ts = rem;
}

void OpenAlSoftAudioEngine::playSound(ALuint* source)
{
	ALenum state;
	
	/* Play the sound until it finishes. */
	
	alGetSourcei(*source, AL_SOURCE_STATE, &state);
	if (state == AL_STOPPED || state == AL_INITIAL)
	{ 
		alSourcePlay(*source);
	}
	
	
	//alSourcePlay(*source);
	//do {
	//	al_nssleep(10000000);

	//	alGetSourcei(*source, AL_SOURCE_STATE, &state);
	//} while(alGetError() == AL_NO_ERROR && state == AL_PLAYING);

}

void OpenAlSoftAudioEngine::error_check(std::string location_str)
{
	/* Check if an error occured, and clean up if so. */
	test_error_flag = alGetError();
	if(test_error_flag != AL_NO_ERROR)
	{
		std::cout << "Error found in " << location_str << ": " << alGetString(test_error_flag) << "\n";
		return;
	}	
}

