#include "ChargeAudio.hpp"
#include "miniaudio/miniaudio.h"

#include <cstdint>

using namespace ChargeAudio;
using namespace Corrade;

Engine::Engine(uint32_t sampleRate, uint32_t channels) {
  maConfig = ma_engine_config_init();
  maConfig.sampleRate = sampleRate;
  maConfig.channels = channels;
  frameSize = ma_get_bytes_per_sample(PCMFormat) * channels;

  ThrowOnRuntimeError("Failed to init miniaudio engine",
                      ma_engine_init(&maConfig, &maEngine));
}

Engine::~Engine() { ma_engine_uninit(&maEngine); }

uint32_t Engine::GetSampleRate() { return maEngine.sampleRate; }
uint32_t Engine::GetChannelCount() { return ma_engine_get_channels(&maEngine); }

// Use case: Stream of PCM data
SoundContainer Engine::CreateSound(int bufferLengthInSeconds) {
  return SoundContainer(new Sound(
      this,
      [length = bufferLengthInSeconds, channels = GetChannelCount(),
       sampleRate = GetSampleRate()](Sound *sound) {
        ma_result result = ma_pcm_rb_init(
            sound->baseEngine->PCMFormat, channels, sampleRate * length,
            nullptr, nullptr, &sound->maRingBuffer);
        ThrowOnRuntimeError("Failed to create a new ring buffer!", result);
        sound->maConfig.pDataSource = &sound->maRingBuffer;
      },
      Sound::SoundType::StreamedRawPCM,
      "Failed to create the sound from ring buffer: "));
}

// Use case: 1 time set up and use audio
SoundContainer Engine::CreateSound(uint8_t *data, int length) {
  return SoundContainer(new Sound(
      this,
      [data, length, channels = GetChannelCount(),
       sampleRate = GetSampleRate()](Sound *sound) {
        ma_audio_buffer_config config = ma_audio_buffer_config_init(
            sound->baseEngine->PCMFormat, channels,
            length / sound->baseEngine->frameSize, data, nullptr);
        ma_result result =
            ma_audio_buffer_init_copy(&config, &sound->maAudioBuffer);

        ThrowOnRuntimeError("Failed to create a new audio buffer!", result);
        sound->maConfig.pDataSource = &sound->maAudioBuffer;
      },
      Sound::SoundType::RawPCM,
      "Failed to create the sound from the PCM data: "));
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
