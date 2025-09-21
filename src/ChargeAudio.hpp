#ifndef CHARGE_AUDIO_BASE_H
#define CHARGE_AUDIO_BASE_H
#include "miniaudio/miniaudio.h"

#include <Corrade/Containers/Containers.h>
#include <Corrade/Containers/Pointer.h>
#include <Magnum/Math/Vector3.h>

#include <functional>
#include <string>

namespace ChargeAudio {
using namespace Corrade;

typedef Containers::Pointer<class Sound> SoundContainer;
typedef Containers::Pointer<class Listener> ListenerContainer;
class Sound {
public:
  enum class SoundState { Idle, Playing, Paused, Finished };
  enum class SoundType { FromFile, StreamedRawPCM, RawPCM };
  // No copying
  Sound(const Sound &) = delete;
  Sound &operator=(const Sound &) = delete;

  // No moving
  Sound(Sound &&) = delete;
  Sound &operator=(Sound &&) = delete;

  ~Sound();
  void Play();
  void Pause();
  void Reset();

  const SoundState GetState();
  const SoundType GetSoundType();

  const float GetPlaybackTime();
  bool SetPlaybackTime(float time);
  const float GetDuration();
  void SetPosition(Magnum::Vector3 position);
  const Magnum::Vector3 GetPosition();
  void SetVolume(float value);
  const float GetVolume();

private:
  Sound(class Engine *engine, std::function<void(Sound *)> setupFunction,
        SoundType type, std::string additionalErrorMessage = "");
  static void onSoundFinish(void *customData, ma_sound *);

  class Engine *baseEngine;
  ma_sound maSound;
  ma_sound_config maConfig;
  ma_pcm_rb maRingBuffer;
  ma_audio_buffer maAudioBuffer;
  SoundState state = SoundState::Idle;
  SoundType type;

  friend class Engine;
};

class Listener {
public:
  // No copying, can move
  Listener(const Listener &) = delete;
  Listener &operator=(const Listener &) = delete;

  void SetEnabled(bool isEnabled);
  const bool GetEnabled();
  void SetDirection(Magnum::Vector3 position);
  const Magnum::Vector3 GetDirection();
  void SetPosition(Magnum::Vector3 position);
  const Magnum::Vector3 GetPosition();

private:
  Listener();
  class Engine *baseEngine;
  ma_uint32 listenerID;
  friend class Engine;
};

class Engine {
public:
  Engine(uint32_t sampleRate = 44100, uint32_t channels = 2);

  // No copying
  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;

  // No movement
  Engine(Engine &&) = delete;
  Engine &operator=(Engine &&) = delete;

  ~Engine();

  // Creating tools
  SoundContainer CreateSound(int bufferLengthInSeconds);
  SoundContainer CreateSound(uint8_t *data, int length);
  SoundContainer CreateSound(std::string filepath, bool streamFile = false);
  ListenerContainer CreateListener();

  void SetVolume(float value);
  const float GetVolume();

  uint32_t GetSampleRate();
  uint32_t GetChannelCount();

private:
  ma_engine maEngine;
  ma_engine_config maConfig;
  ma_result maResponse;
  ma_decoder maStero;
  ma_uint64 listenerCounter = 0;
  friend class Listener;
  friend class Sound;
};

void ThrowOnRuntimeError(std::string message,
                         ma_result errorType = ma_result::MA_ERROR);

} // namespace ChargeAudio
#endif
