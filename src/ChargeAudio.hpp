#include "../lib/miniaudio/miniaudio.c"
#include "../lib/miniaudio/miniaudio.h"
#include <string>

namespace ChargeAudio {
class Sound {
public:
  ~Sound();
  void Play();
  void Pause();
  void Reset();
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
