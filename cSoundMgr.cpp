/*
==========================================================================
cSoundMgr.cpp
=========================================================================
*/
#include "cSoundMgr.h"
cSoundMgr* cSoundMgr::pInstance = NULL;
/*
===========================================================================
Constructor
===========================================================================
*/
cSoundMgr::cSoundMgr()
{
}
/*
===========================================================================
Singleton Design Pattern
===========================================================================
*/
cSoundMgr* cSoundMgr::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cSoundMgr();
	}
	return cSoundMgr::pInstance;
}
void cSoundMgr::add(const string sndName, const string fileName, soundType
	sndType)
{
	if (!getSnd(sndName.c_str()))
	{
		cSound* newSnd = new cSound(sndType);
		newSnd->load(fileName.c_str());
		gameSnds.insert(make_pair(sndName.c_str(), newSnd));
	}
}
cSound* cSoundMgr::getSnd(const string sndName)
{
	map<const string, cSound*>::iterator snd = gameSnds.find(sndName.c_str());
	if (snd != gameSnds.end())
	{
		return snd->second;
	}
	else
	{
		return NULL;
	}
}
void cSoundMgr::deleteSnd()
{
	for (map<const string, cSound*>::iterator snd = gameSnds.begin(); snd
		!= gameSnds.end(); ++snd)
	{
		delete snd->second;
	}
}
bool cSoundMgr::initMixer()
{
	if (SDL_Init(SDL_INIT_AUDIO) != 0)
	{
		cout << "SDL_Init_AUDIO Failed: " << SDL_GetError() << endl;
		return false;
	}
	//Initialise SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096)
		!= 0)
	{
		cout << "Mix_OpenAudio Failed: " << SDL_GetError() << endl;
		return false;
	}
	return true;
}
cSoundMgr::~cSoundMgr()
{
	deleteSnd();
	Mix_CloseAudio();
	Mix_Quit();
}
