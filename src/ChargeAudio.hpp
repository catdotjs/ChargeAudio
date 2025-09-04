#ifndef CHARGE_AUDIO_BASE_H
#define CHARGE_AUDIO_BASE_H
#include "../lib/miniaudio/miniaudio.h"
#include <Magnum/Magnum.h>
#include <Magnum/Math/Vector.h>
#include <string>

namespace ChargeAudio {
class Sound {
public:
  ~Sound();
  void Play();
  void Pause();
  void Reset();
  void SetPosition(Magnum::Vector3 position);
  Magnum::Vector3 GetPosition();
  void SetVolume(float value);
  float GetVolume();

private:
  Sound();
  class Engine *baseEngine;
  ma_sound maSound;
  friend class Engine;
};

class Engine {
public:
  Engine();
  Sound CreateSound(std::string filepath);
  void SetVolume(float value);
  float GetVolume();

private:
  ma_engine maEngine;
  ma_result maResponse;
};
} // namespace ChargeAudio
#endif
