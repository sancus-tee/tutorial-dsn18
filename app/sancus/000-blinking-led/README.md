# Blinking-led: unprotected hardware setup test program

## Application overview

The purpose of this application is to test the basic hardware setup for an
_unprotected_ example program (i.e., without leveraging the secure Sancus CPU
extensions yet). The application should fist cycle through LEDs1-3, while
waiting for user input from the BTNs module. Pressing BTNs1-3 will toggle the
corresponding LEDs, whereas BTN0 will send out a CAN message (this may block
when no other device is listening on the CAN bus).

![fpga-board](https://distrinet.cs.kuleuven.be/software/sancus/images/dsn18-fpga.jpg)

## Your task

Build and run the unmodified example program, play around with
the buttons, and inspect associated debug and CAN output.

![tutorial-hardware](https://distrinet.cs.kuleuven.be/software/sancus/images/dsn18-hardware.png)

## Building and running

The FPGA is connected to the PC via two UART USB serial lines (`/dev/ttyUSB*`) and
one CAN network interface (`slcan*`).
Before uploading and running the MSP430 application, you should prepare the PC
side to receive CAN messages and debug output in separate terminals.

### Monitoring application debug output via downlink

We rely on the GNU `screen` utility to read the stdio UART serial device at a baudrate of 115200.
Simply execute (in a separate terminal):

```bash
$ screen /dev/ttyUSB1 115200 # in case of permission errors, replace `USB1` with the
                             # downlink UART USB device that was assigned to your FPGA
```

### Monitoring application CAN network interface

To dump all incoming CAN frames at the PC side, simply execute (in a separate
terminal):

```bash
$ candump slcan0 # in case of permission errors, replace `slcan0` with the CAN
                 # network interface that was assigned to your FPGA
```

### Uploading application via uplink

Finally, use `make load` to build and upload the application binary to the FPGA
over the downlink UART USB connection `/dev/ttyUSB0`.

```bash
$ FPGA_DEV=/dev/ttyUSB0 make load # in case of permission errors, replace `USB0` with the
                                  # uplink UART USB device that was assigned to your FPGA
```

## Expected behavior

Pressing BTNs1-3 should light up the corresponding LEDs and result in debug
output on the downlink. Pressing BTN0 should result in the following CAN
message being printed by `candump`:

```
slcan0  040   [8]  00 11 22 33 44 55 66 77
```
