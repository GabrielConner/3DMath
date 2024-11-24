#pragma once
#include "SFML/Audio.hpp"

#if defined(_FLOW_CONTROL) | defined (_SOUND)

namespace Sound {
    void start();
    void update();
    void end();
}

#endif

bool playSound(sf::SoundBuffer& sound);
int playMusic(const char* file);
void startMusic(int index);
void pauseMusic(int index);
void stopMusic(int index);
bool musicLoaded(int index);
bool musicPlaying(int index);