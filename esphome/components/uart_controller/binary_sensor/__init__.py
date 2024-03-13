from esphome.components import binary_sensor
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
    CONF_CTRL_CHAR_ON,
    CONF_CTRL_CHAR_OFF,
    CONF_UART_CONTROLLER,
)

DEPENDENCIES = ["uart_controller"]
CODEOWNERS = ["@btittelbach"]


UartBinarySensor = uart_controller_ns.class_(
    "UartBinarySensor", cg.Component, binary_sensor.BinarySensor, SensorItem
)

def validate_control_character(config):
    if CONF_CTRL_CHAR_ON not in config:
        raise cv.Invalid(
            f" {CONF_CTRL_CHAR_ON} is a required property"
        )
    if CONF_CTRL_CHAR_OFF not in config:
        raise cv.Invalid(
            f" {CONF_CTRL_CHAR_OFF} is a required property"
        )
    if len(config[CONF_CTRL_CHAR_ON]) != 1:
        raise cv.Invalid(
            f" {CONF_CTRL_CHAR_ON} must be exactly one character"
        )
    if len(config[CONF_CTRL_CHAR_OFF]) != 1:
        raise cv.Invalid(
            f" {CONF_CTRL_CHAR_OFF} must be exactly one character"
        )
    if config[CONF_CTRL_CHAR_ON] == config[CONF_CTRL_CHAR_OFF]:
        raise cv.Invalid(
            f" {CONF_CTRL_CHAR_ON} must differ from {CONF_CTRL_CHAR_OFF}"
        )
    if config[CONF_CTRL_CHAR_ON] not in "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!?<>&=;:.,*+-_@#":
        raise cv.Invalid(
            f" {CONF_CTRL_CHAR_ON} is using an invalid character"
        )
    if config[CONF_CTRL_CHAR_OFF] not in "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!?<>&=;:.,*+-_@#":
        raise cv.Invalid(
            f" {CONF_CTRL_CHAR_OFF} is using an invalid character"
        )

    return config


CONFIG_SCHEMA = cv.All(
    binary_sensor.binary_sensor_schema(UartBinarySensor)
    .extend(cv.COMPONENT_SCHEMA)
    .extend(UartItemBaseSchema)
    .extend(
        {
            cv.Required(CONF_UART_CONTROLLER): cv.use_id(UartController),
            cv.Optional(CONF_CTRL_CHAR_ON): cv.string,
            cv.Optional(CONF_CTRL_CHAR_OFF): cv.string,
        }
    )
    ,
    validate_control_character,
)


async def to_code(config):
    var = cg.new_Pvariable(
        config[CONF_ID],
        config[CONF_CTRL_CHAR_ON],
        config[CONF_CTRL_CHAR_OFF],
        config[CONF_UART_CONTROLLER],
    )
    await cg.register_component(var, config)
    await binary_sensor.register_binary_sensor(var, config)

    paren = await cg.get_variable(config[CONF_UART_CONTROLLER])
    cg.add(paren.add_sensor_item(var))
    await add_uart_base_properties(var, config, UartBinarySensor, bool, bool)

