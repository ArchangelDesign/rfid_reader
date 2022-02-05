#include "storage.h"
#include "screen.h"
#include "network.h"
#include "status.h"

#if OLED_DRIVER_U8
#include "u8display.h"
#else
#include "adafruitdisplay.h"
#endif

void init_display() {
  #if OLED_DRIVER_U8
  log_d("Using U8 display driver");
  #else
  log_d("Using Adafruit display driver");
  #endif
  display_init();
  display_print_size(0, 0, 2, "GROWTIX");
  display_print_size(0, 20, 2, STRINGIFY(GT_VERSION));
  display_print_size(0, 40, 2, "BOOTING UP...");
  display_flush();
}

bool refresh_screen() {
  char buf[40] = "";
  display_clear();
  sprintf(buf, "SCANS: %d", scan_counter);
  display_print(1, 20, buf);
  switch (really_connected) {
    case 0:
      sprintf(buf, "%s", "ONLINE");
      break;
    case 1:
      sprintf(buf, "%s", "- - - - -");
      break;
    default:
      sprintf(buf, "%s", "OFFLINE");
  }
  // display_print(1, 30, ip_address);
  display_print(1, 30, STRINGIFY(BT_DEVICE_NAME));
  display_print_size(1,1, 2, buf);
  if (current_mode == gt_tap_in) {
    sprintf(buf, "%s", "TAP IN");
  } else {
    sprintf(buf, "%s", "TAP OUT");
  }
  if (strlen(last_error) > 0) {
    sprintf(buf, "%s", last_error);
  }
  display_print_size(1, 41, 2, buf);
  display_flush();
  return true;
}
