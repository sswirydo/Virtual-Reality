#ifndef SOUND_HPP
#define SOUND_HPP

//#include <AL/al.h>
//#include <AL/alc.h>
//#include "AL/alext.h"
//
//#include <sndfile.h>
#include <irrKlang.h>
#include <vector>
#include <iostream>
using namespace irrklang;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
class Sound
{
public:
	void enable();
	void disable();
	void playGameMainTheme();
	void playCollision();
	ISoundEngine* engine = createIrrKlangDevice();
	Sound();
	~Sound();
	bool enabled = true;
protected:
};

#endif //! SOUND_HPP