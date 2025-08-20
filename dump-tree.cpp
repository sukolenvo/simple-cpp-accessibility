#include <atspi/atspi.h>
#include <unistd.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <iostream>
#include "AccessibleObject.hpp"
#include "AccessibleError.hpp"

static void print_info(const AccessibleObject &accessible, std::string padding) {
  auto name = accessible.getName();
  auto description = accessible.getDescription();
  auto role_name = accessible.getRoleName();
  if (name == "Address and search bar") {
    std::cout << accessible.getText() << std::endl;
  }
}

static void traverse(const AccessibleObject &node, const std::string &padding = "") {
  if (!node.isVisible()) {
    return;
  }
  print_info(node, padding);

  for (auto i = 0; i < node.getChildCount(); ++i) {
    try {
      auto child = node.getChild(i);
      traverse(child, padding + "  ");
    } catch (const AccessibleError &e) {
      std::cerr << "ERROR: " << e.what() << std::endl;
    }
  }
}

static pid_t get_active_window_pid() {
  Display *display;
  Window root, active_window;
  Atom net_active_window, net_wm_pid;
  Atom actual_type;
  int actual_format;
  unsigned long nitems, bytes_after;
  unsigned char *prop;
  pid_t pid = 0;

  display = XOpenDisplay(NULL);
  if (!display) {
    return 0;
  }

  root = DefaultRootWindow(display);
  net_active_window = XInternAtom(display, "_NET_ACTIVE_WINDOW", False);
  net_wm_pid = XInternAtom(display, "_NET_WM_PID", False);

  // Get the active window
  if (XGetWindowProperty(display, root, net_active_window, 0, 1, False,
                         XA_WINDOW, &actual_type, &actual_format,
                         &nitems, &bytes_after, &prop) == Success && prop) {
    active_window = *(Window *)prop;
    XFree(prop);

    // Get the PID of the active window
    if (XGetWindowProperty(display, active_window, net_wm_pid, 0, 1, False,
                           XA_CARDINAL, &actual_type, &actual_format,
                           &nitems, &bytes_after, &prop) == Success && prop) {
      pid = *(pid_t *)prop;
      XFree(prop);
    }
  }

  XCloseDisplay(display);
  return pid;
}

int main(int argc, gchar **argv) {
  gint i;
  gboolean found = FALSE;
  atspi_init();

  AccessibleObject desktop = AccessibleObject(atspi_get_desktop(0));
  while (true) {
    pid_t foreground_pid = get_active_window_pid();
    if (foreground_pid == 0) {
      g_print("ERROR: Could not get active window PID\n");
    } else {

      g_print("Active window PID: %d\n", foreground_pid);
      for (i = 0; i < desktop.getChildCount(); i++) {
        auto app = desktop.getChild(i);
        auto app_pid = app.getProcessId();
        if (app_pid == foreground_pid) {
          for (auto j = 0; j < app.getChildCount(); j++) {
            auto child = app.getChild(j);
            if (child.isActive()) {
              found = TRUE;
              traverse(child);
            }
          }
        }
      }

      if (!found) {
        g_print("ERROR: Application with PID %d not found\n", foreground_pid);
      }
    }

    sleep(3);
  }

  return 1;
}
