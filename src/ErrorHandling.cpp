#include "ChargeAudio.hpp"
#include "magic_enum/include/magic_enum/magic_enum.hpp"
#include <Corrade/Utility/Debug.h>
#include <stdexcept>

void ChargeAudio::ThrowOnRuntimeError(std::string message,
                                      ma_result errorType) {
  if (errorType == MA_SUCCESS) {
    return;
  }

  Utility::Error{} << message.c_str()
                   << magic_enum::enum_name(errorType).data();
  throw new std::runtime_error(message + " Check STDERR for more info.\n");
}
