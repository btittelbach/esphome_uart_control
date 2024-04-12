#include "uart_binarysensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace uart_controller {

static const char *const TAG = "uart_controller.binary_sensor";


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
  // call it with the pre converted value and the raw data
  if (this->f_.has_value()) {
    // the lambda can parse the response itself
    auto val = (*this->f_)(this, value, data); // data needs is char but needs to be 'const std::vector<unsigned char>&' for lambda
    if (val.has_value()) {
      ESP_LOGV(TAG, "Value overwritten by lambda");
      value = val.value();
    }
  }
  this->publish_state(value);
}

// void UARTBinarySensor::dump_config() { LOG_BINARY_SENSOR("", "UART Controller Binary Sensor", this); }
void UARTBinarySensor::dump_config() {
#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_VERBOSE
  LOG_BINARY_SENSOR(TAG, "UARTBinarySensor:  Sensor on_char=%c off_char=%c", ctrl_char_on, ctrl_char_off);
#endif
}

}  // namespace uart_controller
}  // namespace esphome
