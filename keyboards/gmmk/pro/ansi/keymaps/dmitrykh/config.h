#ifndef CONFIG_USER_H
#define CONFIG_USER_H

// place overrides here
#define LEADER_TIMEOUT 500
#define RGB_MATRIX_KEYPRESSES

#ifdef RGB_MATRIX_ENABLE
    #define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_SOLID_COLOR
    #define RGB_DISABLE_WHEN_USB_SUSPENDED
#endif

#include "../../config.h"

#endif
