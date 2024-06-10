from esphome.components import sensor
import esphome.config_validation as cv
import esphome.codegen as cg

from esphome.const import CONF_ADDRESS, CONF_ID
from .. import (
    add_uart_base_properties,
    uart_controller_ns,
    UartItemBaseSchema,
    SensorItem,
    UartController,
)
from ..const import (
    CONF_CTRL_CHAR_TRIGGER,
    CONF_UART_CONTROLLER,
)

DEPENDENCIES = ["uart_controller"]
CODEOWNERS = ["@btittelbach"]


UARTEventSensor = uart_controller_ns.class_(
    "UARTEventSensor", cg.Component, sensor.Sensor, SensorItem
)

def validate_control_character(config):
    if CONF_CTRL_CHAR_TRIGGER not in config:
        raise cv.Invalid(
            f" {CONF_CTRL_CHAR_TRIGGER} is a required property"
        )
    if len(config[CONF_CTRL_CHAR_TRIGGER]) != 1:
        raise cv.Invalid(
            f" {CONF_CTRL_CHAR_TRIGGER} must be exactly one character"
        )
    if config[CONF_CTRL_CHAR_TRIGGER] not in "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!?()[]}{<>&=;:.,*+-_@#":
        raise cv.Invalid(
            f" {CONF_CTRL_CHAR_TRIGGER} is using an invalid character"
        )

    return config


CONFIG_SCHEMA = cv.All(
    sensor.sensor_schema(UARTEventSensor)
    .extend(cv.COMPONENT_SCHEMA)
    .extend(UartItemBaseSchema)
    .extend(
        {
            cv.Required(CONF_UART_CONTROLLER): cv.use_id(UartController),
            cv.Optional(CONF_CTRL_CHAR_TRIGGER): cv.string,
        }
    )
    ,
    validate_control_character,
)


async def to_code(config):
    var = cg.new_Pvariable(
        config[CONF_ID],
        config[CONF_CTRL_CHAR_TRIGGER],
    )
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    paren = await cg.get_variable(config[CONF_UART_CONTROLLER])
    cg.add(paren.add_sensor_item(var))
    await add_uart_base_properties(var, config, UARTEventSensor, bool, bool)

