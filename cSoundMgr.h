#ifndef _SOUNDMGR_H
#define _SOUNDMGR_H
// OpenGL Headers
#include "pch.h"
#include "Common.h"
#include "cSound.h"
using namespace std;
class cSoundMgr
{
private:
	static cSoundMgr* pInstance;
protected:
	cSoundMgr();
	~cSoundMgr();
	map <const string, cSound*> gameSnds;
public:
	void add(const string sndName, const string fileName, soundType sndType);
	cSound* getSnd(const string sndName);
	void deleteSnd();
	bool initMixer();
	static cSoundMgr* getInstance();
};
#endif