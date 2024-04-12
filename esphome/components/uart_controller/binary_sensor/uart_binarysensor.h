#pragma once

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/uart_controller/uart_controller.h"
#include "esphome/core/component.h"

#include <vector>

namespace esphome {
namespace uart_controller {

class UARTBinarySensor : public Component, public binary_sensor::BinarySensor, public SensorItem {
 private:
  char ctrl_char_on = '\0';
  char ctrl_char_off = '\0';
  bool state = false;

 public:
  UARTBinarySensor(const char *ctrl_char_on_s, const char *ctrl_char_off_s) {
    this->ctrl_char_on = ctrl_char_on_s[0];
    this->ctrl_char_off = ctrl_char_off_s[0];
  }

  void parse_input(char data) override;
  void set_state(bool state) { this->state = state; }

  void dump_config() override;

  using f_t = std::function<optional<bool>(UARTBinarySensor *, bool, char)>;
  void set_template(f_t &&f) { this->f_ = f;}

 protected:
  optional<f_t> f_{};
};

}  // namespace uart_controller
}  // namespace esphome
