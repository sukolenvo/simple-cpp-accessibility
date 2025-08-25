#include "AccessibleObject.hpp"
#include "AccessibleError.hpp"

#include <atspi/atspi.h>

AccessibleObject::~AccessibleObject() {
  if (value != nullptr) {
    g_object_unref(value);
  }
}

AccessibleObject::AccessibleObject(AtspiAccessible *value) : value(value) {

}

std::string AccessibleObject::getName() const {
  GError *error = nullptr;
  gchar *name = atspi_accessible_get_name(value, &error);
  if (error != nullptr) {
    g_error_free(error);
    return "";
  }

  if (name == nullptr) {
    return "";
  }
  std::string result(name);
  g_free(name);
  return result;
}

std::string AccessibleObject::getDescription() const {
  GError *error = nullptr;
  gchar *description = atspi_accessible_get_description(value, &error);
  if (error != nullptr) {
    g_error_free(error);
    return "";
  }

  if (description == nullptr) {
    return "";
  }
  std::string result(description);
  g_free(description);
  return result;
}

std::string AccessibleObject::getRoleName() const {
  GError *error = nullptr;
  gchar *role_name = atspi_accessible_get_role_name(value, &error);
  if (error != nullptr) {
    g_error_free(error);
    return "";
  }

  if (role_name == nullptr) {
    return "";
  }
  std::string result(role_name);
  g_free(role_name);
  return result;
}

std::string AccessibleObject::getText() const {
  AtspiText *iface = atspi_accessible_get_text_iface(value);
  if (iface == nullptr) {
    return "";
  }
  GError *error = nullptr;
  gchar *text = atspi_text_get_text(iface, 0, -1, &error);
  if (error != nullptr) {
    g_error_free(error);
    g_object_unref(iface);
    return "";
  }
  if (text == nullptr) {
    g_object_unref(iface);
    return "";
  }
  std::string result(text);
  g_free(text);
  g_object_unref(iface);
  return result;
}

std::string AccessibleObject::getValue() const {
  AtspiValue *iface = atspi_accessible_get_value_iface(value);
  if (iface == nullptr) {
    return "";
  }
  GError *error = nullptr;
  gchar *value_text = atspi_value_get_text(iface, &error);
  if (error != nullptr) {
    g_error_free(error);
    g_object_unref(iface);
    return "";
  }
  if (value_text == nullptr) {
    g_object_unref(iface);
    return "";
  }
  std::string result(value_text);
  g_free(value_text);
  g_object_unref(iface);
  return result;
}

int AccessibleObject::getId() const {
  GError *error = nullptr;
  gint id = atspi_accessible_get_id(value, &error);
  if (error != nullptr) {
    g_error_free(error);
    return -1;
  }
  return id;
}

std::string AccessibleObject::getAccessibleId() const {
  GError *error = nullptr;
  gchar *accessible_id = atspi_accessible_get_accessible_id(value, &error);
  if (error != nullptr) {
    g_error_free(error);
    return "";
  }
  if (accessible_id == nullptr) {
    return "";
  }
  std::string result(accessible_id);
  g_free(accessible_id);
  return result;
}

int AccessibleObject::getChildCount() const {
  GError *error = nullptr;
  gint count = atspi_accessible_get_child_count(value, &error);
  if (error != nullptr) {
    g_error_free(error);
    return -1;
  }
  return count;
}

AccessibleObject AccessibleObject::getChild(int index) const {
  GError *error = nullptr;
  AtspiAccessible *child = atspi_accessible_get_child_at_index(value, index, &error);
  if (error != nullptr) {
    auto exception = AccessibleError("Failed to get child at index " + std::to_string(index), error);
    g_error_free(error);
    throw exception;
  }
  if (child == nullptr) {
    throw AccessibleError("Child at index " + std::to_string(index) + " does not exist");
  }
  return AccessibleObject(child);
}
unsigned int AccessibleObject::getProcessId() const {
  GError *error = nullptr;
  unsigned int pid = atspi_accessible_get_process_id(value, &error);
  if (error != nullptr) {
    g_error_free(error);
    return 0;
  }
  return pid;
}

bool AccessibleObject::isActive() const {
  AtspiStateSet *state_set = atspi_accessible_get_state_set(value);
  if (state_set == nullptr) {
    return false;
  }
  gboolean active = atspi_state_set_contains(state_set, ATSPI_STATE_ACTIVE);
  g_object_unref(state_set);
  return active;
}

bool AccessibleObject::isVisible() const {
    AtspiStateSet *state_set = atspi_accessible_get_state_set(value);
    if (state_set == nullptr) {
        return false;
    }
    gboolean visible = atspi_state_set_contains(state_set, ATSPI_STATE_VISIBLE);
    g_object_unref(state_set);
    return visible;
}
AccessibleObject desktop() {
  auto init_result = atspi_init();
  if (init_result != 0 && init_result != 1) {
    throw AccessibleError("Failed to initialize AT-SPI " + std::to_string(init_result));
  }
  AtspiAccessible *desktop = atspi_get_desktop(0);
  if (desktop == nullptr) {
    throw AccessibleError("Failed to get desktop accessible object");
  }
  return AccessibleObject(desktop);
}

AccessibleObject::AccessibleObject(const AccessibleObject &other) : value(other.value) {
  g_object_ref(value);
}

AccessibleObject &AccessibleObject::operator=(const AccessibleObject &other) {
  if (this != &other) {
    if (value != nullptr) {
      g_object_unref(value);
    }
    value = other.value;
    if (value != nullptr) {
      g_object_ref(value);
    }
  }
  return *this;
}

AccessibleObject::AccessibleObject(AccessibleObject &&other) : value(other.value) {
  other.value = nullptr;
}

AccessibleObject& AccessibleObject::operator=(AccessibleObject &&other) {
  if (this != &other) {
    if (value != nullptr) {
      g_object_unref(value);
    }
    value = other.value;
    other.value = nullptr;
  }
  return *this;
}