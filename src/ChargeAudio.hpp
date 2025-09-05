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

class Listener {
public:
  void SetEnabled(bool isEnabled);
  bool GetEnabled();
  void SetPosition(Magnum::Vector3 position);
  Magnum::Vector3 GetPosition();

private:
  Listener();
  class Engine *baseEngine;
  ma_uint32 listenerID;
  friend class Engine;
};

class Engine {
public:
  Engine();
  ~Engine();

  // Creating tools
  Containers::Pointer<Sound> CreateSound(std::string filepath);
  Containers::Pointer<Listener> CreateListener();

  void SetVolume(float value);
  float GetVolume();

private:
  ma_engine maEngine;
  ma_result maResponse;
  ma_uint64 listenerCounter = 0;
  friend class Listener;
};

} // namespace ChargeAudio
#endif
