#include "ChargeAudio.hpp"
#include <stdexcept>

void ChargeAudio::ThrowOnRuntimeError(std::string message,
                                      ma_result errorType) {
  if (errorType == MA_SUCCESS) {
    return;
  }

  Utility::Error{} << message << " (" << errorType << ")";
  throw new std::runtime_error(message + ". Check STDERR for more info.\n");
}
