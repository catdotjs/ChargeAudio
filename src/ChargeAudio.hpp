#ifndef CHARGE_AUDIO_BASE_H
#define CHARGE_AUDIO_BASE_H
#include <Magnum/Magnum.h>
#include <Magnum/Math/Vector.h>
#include <memory>
#include <string>

namespace ChargeAudio {
namespace {
#include "miniaudio/miniaudio.h"
}
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
  std::unique_ptr<Sound> CreateSound(std::string filepath);
  void SetVolume(float value);
  float GetVolume();

private:
  ma_engine maEngine;
  ma_result maResponse;
};

} // namespace ChargeAudio
#endif
