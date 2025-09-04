#include "ChargeAudio.hpp"
#include <Magnum/Magnum.h>
#include <Magnum/Math/Vector3.h>

using namespace ChargeAudio;
Sound::Sound() {}
Sound::~Sound() { ma_sound_uninit(&maSound); }

// Controls
void Sound::Play() { ma_sound_start(&maSound); }
void Sound::Pause() { ma_sound_stop(&maSound); }
void Sound::Reset() { ma_sound_seek_to_pcm_frame(&maSound, 0); }

void Sound::SetPosition(Magnum::Vector3 position) {
  ma_sound_set_position(&maSound, position.x(), position.y(), position.z());
}
Magnum::Vector3 Sound::GetPosition() {
  ma_vec3f pos = ma_sound_get_position(&maSound);
  Magnum::Vector3 position(pos.x, pos.y, pos.z);
  return position;
}
void Sound::SetVolume(float value) { ma_sound_set_volume(&maSound, value); }
float Sound::GetVolume() { return ma_sound_get_volume(&maSound); }
