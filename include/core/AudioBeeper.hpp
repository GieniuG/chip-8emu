#pragma once

#include <cstdint>
#include <vector>
#include <SFML/Audio.hpp>

class AudioBeeper{
    public:
        AudioBeeper();
        void play();
        void stop();
        uint8_t isPlaying();
    private:
      unsigned SAMPLE_RATE;
      int16_t AMPLITUDE;
      double FREQUENCY;

      double samplesPerWave;
      double halfWave;

      std::vector<int16_t> samples;
      sf::SoundBuffer beepBuffer;
      sf::Sound beepSound;
};

