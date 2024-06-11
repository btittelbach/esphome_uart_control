# esphome UART Control Component

This is a custom component for [ESPHome](https://esphome.io/) that allows you to control an ESPHome device using a UART connection. This is useful for controlling an ESPHome device from a computer or another microcontroller instead of HomeAssistant or a Webinterface.

This allows e.g. to leverage the ease and power of esphome in rapid prototyping.

## Basic usage and premisses

- The controll.ing device (computer, embedded system or second μC) is assumed to connect to the esphome μC via TTL-Serial or USB-Serial.
- The controlling device sends single character commands. (by disallowing multi-character strings, we avoid any synchronisation or serial-buffer problems)
- On the esphome-device, the user defines multiple `sensor` and `binary_sensor` which trigger when single character commands are received
- The user attaches arbitrary `then:`-sequence to the sensors.

## Usage in esphome yaml and example

Include the following into your esphome yaml file:


```yaml
external_components:
  - source:
      type: git
      url: https://github.com/btittelbach/esphome_uart_control
      ref: main
    components: [ "uart_controller" ]

## define USB-UART Input
## NOTE: do not include a "logger:" anywhere
uart:
  baud_rate: 115200
  id: ctrluart
  tx_pin: TX
  rx_pin: RX

## include Controller
uart_controller:
- id: uart_ctrl_1
  uart_id: ctrluart

```

Then use it as a `sensor` or `binary_sensor`:

```yaml
sensor:
  - platform: uart_controller
    control_character_trigger: '.'
    uart_controller: uart_ctrl_1
    on_value:
      then:
        - .... your actions in response to character . being sent via serial

binary_sensor:
  - platform: uart_controller
    control_character_on: '('
    control_character_off: ')'
    uart_controller: uart_ctrl_1
    on_press:
      then:
        - .... your actions in response to character ( being sent via serial
    on_release:
      then:
        - .... your actions in response to character ) being sent via serial
```

see [examples/](./examples/) for full examples.

## Copyright and License

This project is licensed under the MIT License - see the [LICENSE](LICENSE.txt) file for details.

