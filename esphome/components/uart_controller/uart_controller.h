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

};


// using SensorSet = std::set<SensorItem *, SensorItemsComparator>;
using SensorSet = std::set<SensorItem *>;

/** UART controller class.
 *   Each instance handles the modbus commuinication for all sensors with the same modbus address
 *
 * when esphome calls UartController::Update the commands for each range are created and sent
 */

class UartController : public PollingComponent, public uart::UARTDevice {
 public:
  void dump_config() override;
  void loop() override;
  void setup() override;
  void update() override;

  /// Registers a sensor with the controller. Called by esphomes code generator
  void add_sensor_item(SensorItem *item) { sensorset_.insert(item); }

 protected:
  /// parse sensormap_ and create range of sequential addresses
  // size_t create_register_ranges_();
  // find register in sensormap. Returns iterator with all registers having the same start address
  // SensorSet find_sensors_(ModbusRegisterType register_type, uint16_t start_address) const;
  // /// submit the read command for the address range to the send queue
  // void update_range_(RegisterRange &r);
  // /// parse incoming modbus data
  // void process_modbus_data_(const ModbusCommandItem *response);
  // /// send the next modbus command from the send queue
  // bool send_next_command_();
  // /// dump the parsed sensormap for diagnostics
  void dump_sensors_();
  /// Collection of all sensors for this component
  SensorSet sensorset_;
  /// Continuous range of modbus registers
  // std::vector<RegisterRange> register_ranges_;
  // /// Hold the pending requests to be sent
  // std::list<std::unique_ptr<ModbusCommandItem>> command_queue_;
  // /// modbus response data waiting to get processed
  // std::queue<std::unique_ptr<ModbusCommandItem>> incoming_queue_;
  // /// when was the last send operation
  // uint32_t last_command_timestamp_;
  // /// min time in ms between sending modbus commands
  // uint16_t command_throttle_;
  // /// if module didn't respond the last command
  // bool module_offline_;
  // /// how many updates to skip if module is offline
  // uint16_t offline_skip_updates_;
};


}  // namespace uart_controller
}  // namespace esphome
