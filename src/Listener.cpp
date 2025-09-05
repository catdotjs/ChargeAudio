#include "ChargeAudio.hpp"
#include <Magnum/Math/Vector3.h>

using namespace ChargeAudio;

Listener::Listener() {}

// Controls
void Listener::SetPosition(Magnum::Vector3 position) {
  ma_engine_listener_set_position(&baseEngine->maEngine, listenerID,
                                  position.x(), position.y(), position.z());
}

Magnum::Vector3 Listener::GetPosition() {
  ma_vec3f pos =
      ma_engine_listener_get_position(&baseEngine->maEngine, listenerID);
  return Magnum::Vector3(pos.x, pos.y, pos.z);
  ;
}
