#include "uart_eventsensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace uart_controller {

static const char *const TAG = "uart_controller.sensor";


void UARTEventSensor::parse_input(char data) {
  float value;

  if (data == ctrl_char_trigger) {
    value = millis();
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

void UARTEventSensor::dump_config() {
#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_VERBOSE
  LOG_SENSOR(TAG, "UARTEventSensor:  Sensor trigger_char=%c", ctrl_char_trigger);
#endif
}

}  // namespace uart_controller
}  // namespace esphome
