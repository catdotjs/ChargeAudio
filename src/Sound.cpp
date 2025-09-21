#include "ChargeAudio.hpp"

#include <functional>

using namespace ChargeAudio;
Sound::Sound(Engine *engine, std::function<void(Sound *)> setupFunction,
             SoundType soundType, std::string additionalErrorMessage)
    : baseEngine(engine) {
  maConfig = ma_sound_config_init_2(&baseEngine->maEngine);
  maConfig.endCallback = Sound::onSoundFinish;
  maConfig.pEndCallbackUserData = this;
  setupFunction(this);
  ma_result maResponse =
      ma_sound_init_ex(&baseEngine->maEngine, &maConfig, &maSound);
  ThrowOnRuntimeError("Failed to create a new sound. " + additionalErrorMessage,
                      maResponse);
  type = soundType;
}

Sound::~Sound() {
  switch (type) {
  case Sound::SoundType::RawPCM:
    ma_audio_buffer_uninit_and_free(&maAudioBuffer);
    break;
  case Sound::SoundType::StreamedRawPCM:
    ma_pcm_rb_uninit(&maRingBuffer);
    break;
  default:
    break;
  }
  ma_sound_uninit(&maSound);
}

const Sound::SoundState Sound::GetState() { return state; }
const Sound::SoundType Sound::GetSoundType() { return type; }

const float Sound::GetDuration() {
  float time;
  ma_sound_get_length_in_seconds(&this->maSound, &time);
  return time;
}

const float Sound::GetPlaybackTime() {
  float time;
  ma_sound_get_cursor_in_seconds(&this->maSound, &time);
  return time;
}

// true or false depending on if the playback was set
bool Sound::SetPlaybackTime(float time) {
  // Better to just catch it from the start
  if (time < 0) {
    return false;
  }

  bool result = ma_sound_seek_to_second(&maSound, time) != MA_SUCCESS;
  if (result) {
    Utility::Error{} << "Failed to set playback time to " << time
                     << " on a sound instance";
  }
  return result;
}

// Controls
void Sound::Play() {
  ma_sound_start(&maSound);
  state = Sound::SoundState::Playing;
}

void Sound::Pause() {
  ma_sound_stop(&maSound);
  state = Sound::SoundState::Paused;
}

void Sound::Reset() {
  if (type == SoundType::StreamedRawPCM) {
    ThrowOnRuntimeError("You cannot reset on Streamed RawPCM sounds!");
  }

  ma_sound_seek_to_pcm_frame(&maSound, 0);
}

void Sound::SetPosition(Magnum::Vector3 position) {
  ma_sound_set_position(&maSound, position.x(), position.y(), position.z());
}
const Magnum::Vector3 Sound::GetPosition() {
  ma_vec3f pos = ma_sound_get_position(&maSound);
  return Magnum::Vector3(pos.x, pos.y, pos.z);
}
void Sound::SetVolume(float value) { ma_sound_set_volume(&maSound, value); }
const float Sound::GetVolume() { return ma_sound_get_volume(&maSound); }

// STATICs
void Sound::onSoundFinish(void *customData, ma_sound *) {
  auto sound = static_cast<Sound *>(customData);
  sound->state = SoundState::Finished;
}
