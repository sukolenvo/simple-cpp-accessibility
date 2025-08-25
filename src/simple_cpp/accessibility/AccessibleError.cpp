#include "AccessibleError.hpp"

const char *AccessibleError::what() const noexcept {
  return message.c_str();
}
