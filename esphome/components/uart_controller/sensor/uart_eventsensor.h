#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart_controller/uart_controller.h"
#include "esphome/core/component.h"

#include <vector>

namespace esphome {
namespace uart_controller {

class UARTEventSensor : public Component, public sensor::Sensor, public SensorItem {
 private:
  char ctrl_char_trigger = '\0';
  bool state = false;

 public:
  UARTEventSensor(const char *ctrl_char_trigger_s) {
    this->ctrl_char_trigger = ctrl_char_trigger_s[0];
  }

  void parse_input(char data) override;
  void set_state(bool state) { this->state = state; }

  void dump_config() override;

  using f_t = std::function<optional<bool>(UARTEventSensor *, bool, char)>;
  void set_template(f_t &&f) { this->f_ = f;}

 protected:
  optional<f_t> f_{};
};

}  // namespace uart_controller
}  // namespace esphome
