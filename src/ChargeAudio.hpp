#ifndef CHARGE_AUDIO_BASE_H
#define CHARGE_AUDIO_BASE_H
#include <Corrade/Containers/Containers.h>
#include <Magnum/Magnum.h>
#include <Magnum/Math/Vector.h>
#include <string>

namespace ChargeAudio {
namespace _ma {
#include "miniaudio/miniaudio.h"
}
using namespace Corrade;
using namespace _ma;
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
  ~Engine();
  Containers::Pointer<Sound> CreateSound(std::string filepath);

  void SetPosition(Magnum::Vector3 position);
  Magnum::Vector3 GetPosition();
  void SetVolume(float value);
  float GetVolume();

private:
  ma_engine maEngine;
  ma_result maResponse;
};

} // namespace ChargeAudio
#endif
