#include "ChargeAudio.hpp"
#include "miniaudio/miniaudio.h"

#include <Corrade/Utility/Debug.h>
#include <cstdint>
#include <stdexcept>

using namespace ChargeAudio;
using namespace Corrade;

Engine::Engine(uint32_t sampleRate, uint32_t channels) {
  maConfig = ma_engine_config_init();
  maConfig.sampleRate = sampleRate;
  maConfig.channels = channels;

  if ((maResponse = ma_engine_init(&maConfig, &maEngine)) != MA_SUCCESS) {
    Utility::Error{} << "Could not init miniaudio (" << maResponse << ")";
    throw new std::runtime_error(
        "Failed to init miniaudio engine. Check STDERR for more info.");
  }
}

Engine::~Engine() { ma_engine_uninit(&maEngine); }

uint32_t Engine::GetSampleRate() { return maEngine.sampleRate; }
uint32_t Engine::GetChannelCount() { return ma_engine_get_channels(&maEngine); }

SoundContainer Engine::CreateSound(int bufferLengthInSeconds) {
  return SoundContainer(new Sound(
      this,
      [length = bufferLengthInSeconds, channels = GetChannelCount(),
       sampleRate = GetSampleRate()](Sound *sound) {
        ma_result result = ma_pcm_rb_init(
            ma_format_s32, channels, sampleRate * channels * length, nullptr,
            nullptr, &sound->maRingBuffer);
        if (result != MA_SUCCESS) {
          Utility::Error{} << "Failed to create a new ring buffer!" << " ("
                           << result << ")";
          throw new std::runtime_error("Failed to create a new ring buffer! "
                                       "Check STDERR for more info.");
        }
      },
      Sound::SoundType::RawPCM, "Failed to create the sound from the data: "));
}

SoundContainer Engine::CreateSound(const std::string filepath,
                                   bool streamFile) {
  return SoundContainer(new Sound(
      this,
      [filepath, streamFile](Sound *sound) {
        sound->maConfig.pFilePath = filepath.c_str();
        sound->maConfig.flags = (streamFile ? MA_SOUND_FLAG_STREAM : 0);
      },
      Sound::SoundType::FromFile,
      "Failed to create the sound from the file: " + filepath));
}

ListenerContainer Engine::CreateListener() {
  auto listener = ListenerContainer(new Listener());
  listener->baseEngine = this;
  listener->listenerID = listenerCounter++;

  return listener;
}

// Controls
void Engine::SetVolume(float value) { ma_engine_set_volume(&maEngine, value); }
const float Engine::GetVolume() { return ma_engine_get_volume(&maEngine); }
