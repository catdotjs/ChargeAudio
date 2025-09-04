#include "ChargeAudio.hpp"
#include <Corrade/Utility/Debug.h>

#include <cstddef>
#include <stdexcept>
#include <string>

using namespace ChargeAudio;
using namespace Corrade;

Engine::Engine() {
  if ((maResponse = ma_engine_init(NULL, &maEngine)) != MA_SUCCESS) {
    Utility::Error{} << "Could not init miniaudio (" << maResponse << ")";
    throw new std::runtime_error(
        "Failed to init miniaudio engine. Check STDERR for more info.");
  }
}

std::unique_ptr<Sound> Engine::CreateSound(std::string filepath) {
  auto sound = std::unique_ptr<Sound>(new Sound());
  sound->baseEngine = this;

  maResponse = ma_sound_init_from_file(&maEngine, filepath.c_str(), 0, NULL,
                                       NULL, &sound->maSound);
  if (maResponse != MA_SUCCESS) {
    Utility::Error{} << "Failed to create the sound from the file: "
                     << filepath.c_str() << " (" << maResponse << ")";
    throw new std::runtime_error(
        "Failed to create the sound from file. Check STDERR for more info.");
  }

  return sound;
}

// Controls
void Engine::SetVolume(float value) { ma_engine_set_volume(&maEngine, value); }
float Engine::GetVolume() { return ma_engine_get_volume(&maEngine); }
