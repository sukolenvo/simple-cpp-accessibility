#ifndef AT_SPI_DECORATOR__ACCESSIBLEERROR_HPP_
#define AT_SPI_DECORATOR__ACCESSIBLEERROR_HPP_

#include <glib.h>

#include <exception>
#include <string>
#include <utility>

class AccessibleError : public std::exception {
 private:
  std::string message;
 public:
  explicit AccessibleError(std::string message) : message(std::move(message)) {}
  AccessibleError(std::string message, GError *error) : message(std::move(message) + std::to_string(error->code) + ": " + std::string(error->message)) {}

  [[nodiscard]] const char *what() const noexcept override;
};

#endif //AT_SPI_DECORATOR__ACCESSIBLEERROR_HPP_
