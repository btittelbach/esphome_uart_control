import binascii
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ADDRESS, CONF_ID, CONF_NAME, CONF_LAMBDA, CONF_OFFSET
from esphome.cpp_helpers import logging
from .const import (
    CONF_CTRL_CHAR_ON,
    CONF_CTRL_CHAR_OFF,
    CONF_UART_CONTROLLER,
    CONF_UART_ID,
)

CODEOWNERS = ["@btittelbach"]

AUTO_LOAD = ["uart"]

MULTI_CONF = True

uart_controller_ns = cg.esphome_ns.namespace("uart_controller")
UartController = uart_controller_ns.class_(
    "UartController", cg.PollingComponent, uart.UARTDevice
)

SensorItem = uart_controller_ns.struct("SensorItem")

_LOGGER = logging.getLogger(__name__)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(UartController),
            cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    # .extend(uart.uart_device_schema(0x01))
)


UartItemBaseSchema = cv.Schema(
    {
        cv.GenerateID(CONF_UART_CONTROLLER): cv.use_id(UartController),
    },
)


async def add_uart_base_properties(
    var, config, sensor_type, lamdba_param_type=cg.float_, lamdba_return_type=float
):
    if CONF_CUSTOM_COMMAND in config:
        cg.add(var.set_custom_data(config[CONF_CUSTOM_COMMAND]))

    if config[CONF_RESPONSE_SIZE] > 0:
        cg.add(var.set_register_size(config[CONF_RESPONSE_SIZE]))

    if CONF_LAMBDA in config:
        template_ = await cg.process_lambda(
            config[CONF_LAMBDA],
            [
                (sensor_type.operator("ptr"), "item"),
                (lamdba_param_type, "x"),
                (
                    cg.std_vector.template(cg.uint8).operator("const").operator("ref"),
                    "data",
                ),
            ],
            return_type=cg.optional.template(lamdba_return_type),
        )
        cg.add(var.set_template(template_))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    # cg.add(var.set_command_throttle(config[CONF_COMMAND_THROTTLE]))
    # cg.add(var.set_offline_skip_updates(config[CONF_OFFLINE_SKIP_UPDATES]))
    await register_uart_device(var, config)


async def register_uart_device(var, config):
    # cg.add(var.set_address(config[CONF_ADDRESS]))
    await cg.register_component(var, config)
    return await uart.register_uart_device(var, config)

