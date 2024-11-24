#define _SOUND
#include "soundLoading.h"
#include <Vector>

sf::Sound** soundQuery = nullptr;
sf::Music** musicQuery = nullptr;

namespace Sound {
    void start() {
        soundQuery = new sf::Sound*[246];
        for (int i = 0; i < 246; i++)
            soundQuery[i] = nullptr;

        musicQuery = new sf::Music*[10];
        for (int i = 0; i < 10; i++)
            musicQuery[i] = nullptr;
    }
    void update() {
        for (int i = 0; i < 246; i++)
            if (soundQuery[i] != nullptr)
                if (soundQuery[i]->getStatus() != sf::SoundSource::Status::Playing) {
                    delete (soundQuery[i]);
                    soundQuery[i] = nullptr;
                }

        for (int i = 0; i < 10; i++)
            if (musicQuery[i] != nullptr)
                if (musicQuery[i]->getStatus() == sf::SoundSource::Status::Stopped) {
                    delete (musicQuery[i]);
                    musicQuery[i] = nullptr;
                }
    }

    void end() {
        for (int i = 0; i < 246; i++) {
            if (soundQuery[i] != nullptr) {
                soundQuery[i]->stop();
                delete (soundQuery[i]);
            }
        }

        for (int i = 0; i < 10; i++) {
            if (musicQuery[i] != nullptr) {
                musicQuery[i]->stop();
                delete (musicQuery[i]);
            }
        }
        delete[] (soundQuery);
        delete[] (musicQuery);
    }
}

bool playSound(sf::SoundBuffer& sound) {
    for (int i = 0; i < 246; i++)
        if (soundQuery[i] == nullptr) {
            soundQuery[i] = new sf::Sound;
            soundQuery[i]->setBuffer(sound);
            soundQuery[i]->play();
            return true;
        }
    return false;
}

int playMusic(const char* file) {
    for (int i = 0; i < 10; i++) {
        if (musicQuery[i] == nullptr) {
            musicQuery[i] = new sf::Music;
            musicQuery[i]->openFromFile(file);
            musicQuery[i]->play();
            return i;
        }
    }
    return -1;
}

void startMusic(int index) {
    if (index < 0)
        return;

    if (musicQuery[index] != nullptr)
        musicQuery[index]->play();
}

void pauseMusic(int index) {
    if (index < 0)
        return;

    if (musicQuery[index] != nullptr)
        musicQuery[index]->pause();
}

void stopMusic(int index) {
    if (index < 0)
        return;

    if (musicQuery[index] != nullptr)
        musicQuery[index]->stop();
}

bool musicLoaded(int index) {
    if (index < 0)
        return false;

    if (musicQuery[index] != nullptr)
        if (musicQuery[index]->getStatus() != sf::SoundSource::Status::Stopped)
            return true;
    return false;
}

bool musicPlaying(int index) {
    if (index < 0)
        return false;

    if (musicQuery[index] != nullptr)
        if (musicQuery[index]->getStatus() == sf::SoundSource::Status::Playing)
            return true;
    return false;
}