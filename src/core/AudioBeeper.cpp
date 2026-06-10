#include "../../include/core/AudioBeeper.hpp"
#include <cmath>
#include <cstdint>


AudioBeeper::AudioBeeper(){
  SAMPLE_RATE = 44100;
  AMPLITUDE = 10000;
  FREQUENCY = 440.0;
  samplesPerWave = SAMPLE_RATE / FREQUENCY;
  halfWave = samplesPerWave / 2.0;

  samples.resize(SAMPLE_RATE);

  for (unsigned i = 0; i < SAMPLE_RATE; ++i) {
    if (std::fmod(i, samplesPerWave) < halfWave) {
      samples[i] = AMPLITUDE;
    } else {
      samples[i] = -AMPLITUDE;
    }
  }

  beepBuffer.loadFromSamples(samples.data(), samples.size(), 1, SAMPLE_RATE);

  beepSound.setBuffer(beepBuffer);
  beepSound.setLoop(true);
}

uint8_t AudioBeeper::isPlaying(){
    return beepSound.getStatus() == sf::Sound::Playing;
}

void AudioBeeper::play(){
    beepSound.play();
}
void AudioBeeper::stop(){
    beepSound.stop();
}
