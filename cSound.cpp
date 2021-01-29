#include "cSound.h"
/*
==========================================================================
cSound.cpp
==========================================================================
*/
//#pragma warning ( disable : 4996 )
#include "cSound.h"
using namespace std;
cSound::cSound()
{
	this->theSoundType = soundType::sfx;
}
cSound::cSound(soundType sndType)
{
	this->theSoundType = sndType;
}
cSound::~cSound()
{
}
bool cSound::load(const string filename)
{
	switch (this->theSoundType)
	{
	case soundType::sfx:
	{
		this->SoundFX = Mix_LoadWAV(filename.c_str());
		if (this->SoundFX == NULL)
		{
			cout << "Sound FX '" << filename << "' could not be loaded. " << SDL_GetError() << endl;
			return false;
		}
		cout << "Sound FX '" << filename << "' loaded successfully!. " << endl;
		return true;
	}
	break;
	case soundType::music:
	{
		this->sMusic = Mix_LoadMUS(filename.c_str());
		if (this->sMusic == NULL)
		{
			cout << "Music '" << filename << "' could not be loaded. " << SDL_GetError() << endl;
			return false;
		}
		cout << "Music '" << filename << "' loaded successfully!. " << endl;
		return true;
	}
	break;
	default:
		return false;
	}
}
void cSound::play(int loop)
{
	switch (this->theSoundType)
	{
	case soundType::sfx:
	{
		Mix_PlayChannel(-1, this->SoundFX, loop);
	}
	break;
	case soundType::music:
	{
		Mix_PlayMusic(this->sMusic, loop);
	}
	break;
	default:
		Log::LOG_DEBUG("File of this type not found!!!");
		break;
	}
}

void cSound::pause()
{
	Mix_PauseMusic(); // pause music
}

void cSound::resume()
{
	Mix_ResumeMusic(); // resume music
}