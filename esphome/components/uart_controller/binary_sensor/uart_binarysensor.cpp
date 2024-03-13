#include "uart_binarysensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace uart_controller {

static const char *const TAG = "uart_controller.binary_sensor";

void UARTBinarySensor::dump_config() { LOG_BINARY_SENSOR("", "UART Controller Binary Sensor", this); }

void UARTBinarySensor::parse_input(char data) {
  bool value;

  if (data == ctrl_char_on) {
    value = true;
  } else if (data == ctrl_char_off) {
    value = false;
  } else {
    // this is not the character we are looking for. May be another sensor's problem.
    return;
  }
  // Is there a lambda registered
  // call it with the pre converted value and the raw data array
  if (this->transform_func_.has_value()) {
    // the lambda can parse the response itself
    auto val = (*this->transform_func_)(this, value, data);
    if (val.has_value()) {
      ESP_LOGV(TAG, "Value overwritten by lambda");
      value = val.value();
    }
  }
  this->publish_state(value);
}

}  // namespace uart_controller
}  // namespace esphome
