#include "ChargeAudio.hpp"
#include <Corrade/Containers/Containers.h>
#include <Corrade/Containers/Pointer.h>
#include <Corrade/Tags.h>
#include <Corrade/Utility/Debug.h>
#include <Magnum/Math/Vector3.h>

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

Engine::~Engine() { ma_engine_uninit(&maEngine); }

SoundContainer Engine::CreateSound(const std::string filepath,
                                   bool streamFile) {
  return SoundContainer(new Sound(
      this,
      [filepath, streamFile](Sound *sound) {
        sound->maConfig.pFilePath = filepath.c_str();
        sound->maConfig.flags = (streamFile ? MA_SOUND_FLAG_STREAM : 0);
      },
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
