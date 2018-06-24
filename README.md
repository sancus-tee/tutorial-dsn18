# Application overview and objectives

Participants of this tutorial will develop a distributed trusted computing
application that features secure, authenticated communication between an embedded
MSP430 [Sancus](https://distrinet.cs.kuleuven.be/software/sancus) enclave and
an off-the-shelf x86 [Intel SGX](https://software.intel.com/en-us/sgx) enclave.
Particularly, as depicted below, the application's goal is to cryptographically
prove to a trusted SGX enclave that an expected Sancus enclave is executing
untampered on a genuine Sancus-enabled CPU (i.e., _remote attestation_
guarantee). The Sancus enclave itself will furthermore establish local trust
relations with dedicated Sancus driver enclaves for secure I/O (i.e., _local
attestation_ guarantee).  All communication between the SGX and Sancus sides
proceeds over an _untrusted_ automotive industry-standard
[CAN](https://en.wikipedia.org/wiki/CAN_bus) bus.

![tutorial-overview](https://distrinet.cs.kuleuven.be/software/sancus/images/dsn18-overview.png)

# Tutorial organization

This tutorial is aimed at a target audience of C programmers with an interest
in security. The tutorial takes about three hours, with an equal split between
lectures and practical exercises (further details are provided on the
[webpage](https://distrinet.cs.kuleuven.be/software/sancus/tutorial.php)).

To get started, simply clone this repository:

```bash
$ git clone https://github.com/sancus-pma/tutorial-dsn18.git
$ cd tutorial-dsn18
$ git submodule init
$ git submodule update
```

The source code in this repository is organized as follows:

- **app/sancus:** instructions and source code exercises for the Sancus-enabled
  MSP430 FPGA side of the distributed enclave application.
- **app/sgx:** instructions and source code exercises for the SGX-capable PC
  side of the distributed enclave application.
- **scrips:** collection of utility scrips for configuring the teaching setup
  boxes (not needed for participants).
- **usbtin:** utilities for configuring the CAN2USB interface (not needed for
  participants).

# Teaching setup overview

One teaching setup box features five Sancus-enabled Xess
[XuLA2-LX25](http://www.xess.com/shop/product/xula2-lx25/) Spartan-6 FPGAs,
each synthesized with a Sancus-enabled OpenMSP430
[core](https://github.com/sancus-pma/sancus-core) and mounted on a
[Stickit!-2 board](http://www.xess.com/shop/product/stickit-mb-4_0/).
All Sancus nodes are interfaced with a [Pmod
LED](https://store.digilentinc.com/pmod-led-four-high-brightness-leds/), a
[Pmod BTN](https://store.digilentinc.com/pmodbtn-4-user-pushbuttons/), and a
[NiRen](http://henrysbench.capnfatz.com/henrys-bench/arduino-projects-tips-and-more/arduino-can-bus-module-pin-outs-and-schematics/)
SPI CAN transceiver peripheral module. Each FPGA connects to the PC side
via a dedicated [USBtin](https://www.fischl.de/usbtin/) connector.

![fpga-box](https://distrinet.cs.kuleuven.be/software/sancus/images/dsn18-box.jpeg)
