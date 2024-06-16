
#include <device.h>
#include <drivers/behavior.h>
#include <logging/log.h>
#include <zephyr.h>
#include <zmk/mouse.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static int mouse_sensitivity = ZMK_MOUSE_DEFAULT_MOVE_VAL;

static int
behavior_increase_sensitivity(struct device *dev,
                              struct behavior_driver_api *api,
                              const struct zmk_behavior_binding *binding,
                              struct zmk_behavior_binding_event event) {
  if (mouse_sensitivity < 3000) { // Max sensitivity limit
    mouse_sensitivity += 100;
    zmk_mouse_set_sensitivity(mouse_sensitivity);
    LOG_INF("Increased mouse sensitivity to %d", mouse_sensitivity);
  }
  return 0;
}

static int
behavior_decrease_sensitivity(struct device *dev,
                              struct behavior_driver_api *api,
                              const struct zmk_behavior_binding *binding,
                              struct zmk_behavior_binding_event event) {
  if (mouse_sensitivity > 100) { // Min sensitivity limit
    mouse_sensitivity -= 100;
    zmk_mouse_set_sensitivity(mouse_sensitivity);
    LOG_INF("Decreased mouse sensitivity to %d", mouse_sensitivity);
  }
  return 0;
}

static const struct behavior_driver_api behavior_increase_sensitivity_api = {
    .binding_pressed = behavior_increase_sensitivity,
    .binding_released = NULL,
};

static const struct behavior_driver_api behavior_decrease_sensitivity_api = {
    .binding_pressed = behavior_decrease_sensitivity,
    .binding_released = NULL,
};

DEVICE_DT_DEFINE(DT_INST(0, custom_behavior), behavior_init, NULL, NULL, NULL,
                 POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                 &behavior_increase_sensitivity_api);
DEVICE_DT_DEFINE(DT_INST(1, custom_behavior), behavior_init, NULL, NULL, NULL,
                 POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                 &behavior_decrease_sensitivity_api);
