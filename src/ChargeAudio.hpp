#ifndef CHARGE_AUDIO_BASE_H
#define CHARGE_AUDIO_BASE_H
#include <Corrade/Containers/Containers.h>
#include <Corrade/Containers/Pointer.h>
#include <Magnum/Magnum.h>
#include <Magnum/Math/Vector.h>
#include <functional>
#include <string>

namespace ChargeAudio {
namespace _ma {
#include "miniaudio/miniaudio.h"
}
using namespace Corrade;
using namespace _ma;

typedef Containers::Pointer<class Sound> SoundContainer;
typedef Containers::Pointer<class Listener> ListenerContainer;
class Sound {
public:
  enum SoundState { Idle, Playing, Paused, Finished };

  ~Sound();
  void Play();
  void Pause();
  void Reset();

  SoundState GetState();
  float GetPlaybackTime();
  bool SetPlaybackTime(float time);
  float GetDuration();
  void SetPosition(Magnum::Vector3 position);
  Magnum::Vector3 GetPosition();
  void SetVolume(float value);
  float GetVolume();

private:
  Sound(class Engine *engine, std::function<void(Sound *)> setupFunction,
        std::string additionalErrorMessage = "");
  static void onSoundFinish(void *customData, ma_sound *);

  class Engine *baseEngine;
  ma_sound maSound;
  ma_sound_config maConfig;
  SoundState state = SoundState::Idle;

  friend class Engine;
};

class Listener {
public:
  void SetEnabled(bool isEnabled);
  bool GetEnabled();
  void SetDirection(Magnum::Vector3 position);
  Magnum::Vector3 GetDirection();
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
  SoundContainer CreateSound(std::string filepath, bool streamFile = false);
  ListenerContainer CreateListener();

  void SetVolume(float value);
  float GetVolume();

private:
  ma_engine maEngine;
  ma_result maResponse;
  ma_uint64 listenerCounter = 0;
  friend class Listener;
  friend class Sound;
};

} // namespace ChargeAudio
#endif
