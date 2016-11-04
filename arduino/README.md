# Actuator Sensor Unit

### How it works

There are two **important** files:

1. `ASU.ino`
2. `config.h`

The first file contains all the code you need to make work the devices.

The second one allows you to choose the right module for your needs.

> i.e. : If you need to configure the Pulse Counter, maybe you should comment everything unless the macros of `BOARD_ETHERNET_SHIELD_W5100`, `FUNCTION_WEBSERVER`, `PULSE_SO` and `COUNTER` (the last one has been added recently, and not mentioned in the paper thesis).

####So, for each device type, <strong>modify</strong> `config.ino` accordingly.

This part of the project benefits from the collaboration of <em>Vincenzo Suraci</em>.
