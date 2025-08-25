#ifndef AT_SPI_DECORATOR__ACCESSIBLEOBJECT_HPP_
#define AT_SPI_DECORATOR__ACCESSIBLEOBJECT_HPP_

#include <atspi/atspi-types.h>

#include <string>

class AccessibleObject {
 private:
  AtspiAccessible *value;
 public:
  explicit AccessibleObject(AtspiAccessible* value);

  AccessibleObject(const AccessibleObject&);
  AccessibleObject& operator=(const AccessibleObject&);
  AccessibleObject(AccessibleObject&&);
  AccessibleObject& operator=(AccessibleObject &&);
  ~AccessibleObject();

  std::string getName() const;

  std::string getDescription() const;

  std::string getRoleName() const;

  std::string getText() const;

  std::string getValue() const;

  int getId() const;

  std::string getAccessibleId() const;

  int getChildCount() const;

  AccessibleObject getChild(int index) const;

  unsigned int getProcessId() const;

  bool isActive() const;

  bool isVisible() const;
};

AccessibleObject desktop();

#endif //AT_SPI_DECORATOR__ACCESSIBLEOBJECT_HPP_
