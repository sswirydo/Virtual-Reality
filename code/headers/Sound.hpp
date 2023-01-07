#ifndef SOUND_HPP
#define SOUND_HPP

//#include <AL/al.h>
//#include <AL/alc.h>
//#include "AL/alext.h"
//
//#include <sndfile.h>
#include <irrKlang.h>
#include <vector>
#include <inttypes.h>
#include <iostream>
using namespace irrklang;

// To be able to use the irrKlang.dll file, we need to link with the irrKlang.lib.
// We could set this option in the project settings, but to make it easy we use
// a pragma comment:

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
class Sound
{
public:
	void playGameMainTheme();
	void playCollision();
	ISoundEngine* engine = createIrrKlangDevice();
	Sound();
	~Sound();
};

#endif //! SOUND_HPP