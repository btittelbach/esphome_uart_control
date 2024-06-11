#pragma once

#include "esphome/core/component.h"

#include "esphome/components/uart/uart.h"
#include "esphome/core/automation.h"

#include <list>
#include <queue>
#include <set>
#include <vector>

namespace esphome {
namespace uart_controller {

class UartController;

class SensorItem {
 public:
  virtual void parse_input(char data) = 0;
  virtual void dump_config() = 0;

};

using SensorSet = std::set<SensorItem *>;

class UartController : public PollingComponent, public uart::UARTDevice {
 public:
  void dump_config() override;
  void loop() override;
  void setup() override;
  void update() override;

  /// Registers a sensor with the controller. Called by esphomes code generator
  void add_sensor_item(SensorItem *item) { sensorset_.insert(item); }

 protected:
  void dump_sensors_();
  /// Collection of all sensors for this component
  SensorSet sensorset_;
};


}  // namespace uart_controller
}  // namespace esphome
