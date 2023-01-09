#include "../headers/Sound.hpp"


Sound::Sound(){
    if (!engine)
    {
        std::cout << "Error initialising sound" << std::endl;
    }
}

void Sound::setVolume(float volume) { engine->setSoundVolume(volume); }

void Sound::enable() { this->enabled = true; engine->setAllSoundsPaused(false); }
void Sound::disable() { this->enabled = false; engine->setAllSoundsPaused(true); }


void Sound::playGameMainTheme(){
    engine->play2D("assets/audio/BoxCat Games - Epic Song.mp3",  true, false, true);
}

void Sound::playCollision(){
    if (enabled) engine->play2D("assets/audio/collision.mp3", false);
}
void Sound::playKlaxon(){
    if (enabled) engine->play2D("assets/audio/klaxon.mp3", false);
}

Sound::~Sound(){
    engine->drop(); // delete engine
}