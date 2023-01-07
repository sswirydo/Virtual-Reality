#include "../headers/Sound.hpp"


Sound::Sound(){
    if (!engine)
    {
        std::cout << "Error initialising sound" << std::endl;
    }
}

void Sound::playGameMainTheme(){
    engine->play2D("assets/audio/test.mp3",  true, false, true);
}

void Sound::playCollision(){
    engine->play2D("assets/audio/collision.mp3", false);
}

Sound::~Sound(){
    engine->drop(); // delete engine
}