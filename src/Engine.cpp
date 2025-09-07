#include "ChargeAudio.hpp"
#include <Corrade/Containers/Pointer.h>
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

Containers::Pointer<Sound> Engine::CreateSound(const std::string filepath) {
  auto sound = Containers::Pointer<Sound>(new Sound(this));
  sound->maConfig.pFilePath = filepath.c_str();
  sound->maConfig.flags = 0;
  sound->maConfig.pInitialAttachment = NULL;
  sound->maConfig.pDoneFence = NULL;
  sound->init("Failed to create the sound from the file: " + filepath);
  return sound;
}

Containers::Pointer<Listener> Engine::CreateListener() {
  auto listener = Containers::Pointer<Listener>(new Listener());
  listener->baseEngine = this;
  listener->listenerID = listenerCounter++;

  return listener;
}

// Controls
void Engine::SetVolume(float value) { ma_engine_set_volume(&maEngine, value); }
float Engine::GetVolume() { return ma_engine_get_volume(&maEngine); }
