#include "ChargeAudio.hpp"
#include <Magnum/Math/Vector3.h>

using namespace ChargeAudio;

Listener::Listener() {}

// Controls
void Listener::SetEnabled(bool isEnabled) {
  ma_engine_listener_set_enabled(&baseEngine->maEngine, listenerID, isEnabled);
}

bool Listener::GetEnabled() {
  return ma_engine_listener_is_enabled(&baseEngine->maEngine, listenerID);
}
void Listener::SetPosition(Magnum::Vector3 position) {
  ma_engine_listener_set_position(&baseEngine->maEngine, listenerID,
                                  position.x(), position.y(), position.z());
}

Magnum::Vector3 Listener::GetPosition() {
  ma_vec3f pos =
      ma_engine_listener_get_position(&baseEngine->maEngine, listenerID);
  return Magnum::Vector3(pos.x, pos.y, pos.z);
}
