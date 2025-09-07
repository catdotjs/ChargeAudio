#include "ChargeAudio.hpp"
#include <Magnum/Magnum.h>
#include <Magnum/Math/Vector3.h>

#include <functional>
#include <stdexcept>

using namespace ChargeAudio;
Sound::Sound(Engine *engine, std::function<void(Sound *)> setupFunction,
             std::string additionalErrorMessage)
    : baseEngine(engine) {
  maConfig = ma_sound_config_init_2(&baseEngine->maEngine);
  maConfig.endCallback = Sound::onSoundFinish;
  maConfig.pEndCallbackUserData = this;
  setupFunction(this);
  ma_result maResponse =
      ma_sound_init_ex(&baseEngine->maEngine, &maConfig, &maSound);
  if (maResponse != MA_SUCCESS) {
    Utility::Error{} << "Failed to create a new sound" << " (" << maResponse
                     << ")";
    throw new std::runtime_error(
        "Failed to create a new sound. Check STDERR for more info.\n" +
        additionalErrorMessage);
  }
}

Sound::~Sound() { ma_sound_uninit(&maSound); }
Sound::SoundState Sound::GetState() { return state; }

// Controls
void Sound::Play() {
  ma_sound_start(&maSound);
  state = Sound::SoundState::Playing;
}

void Sound::Pause() {
  ma_sound_stop(&maSound);
  state = Sound::SoundState::Paused;
}

void Sound::Reset() { ma_sound_seek_to_pcm_frame(&maSound, 0); }

void Sound::SetPosition(Magnum::Vector3 position) {
  ma_sound_set_position(&maSound, position.x(), position.y(), position.z());
}
Magnum::Vector3 Sound::GetPosition() {
  ma_vec3f pos = ma_sound_get_position(&maSound);
  return Magnum::Vector3(pos.x, pos.y, pos.z);
}
void Sound::SetVolume(float value) { ma_sound_set_volume(&maSound, value); }
float Sound::GetVolume() { return ma_sound_get_volume(&maSound); }

// STATICs
void Sound::onSoundFinish(void *customData, ma_sound *) {
  auto sound = static_cast<Sound *>(customData);
  sound->state = SoundState::Finished;
}
