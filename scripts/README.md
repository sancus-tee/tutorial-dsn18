For tutorial participants, the teaching setup boxes should already be flashed
and properly connected to the PC. If you are not configuring the box, you can
safely ignore the instructions below.

## Check connected FPGA boards

This assumes five connected boards (i.e. one teaching setup). Check the
script for details on programming different numbers of boards or individual
FPGA units.

```bash
% sudo scripts/checknflash.sh
```

## Flash connected FPGA boards

```bash
$ sudo scripts/checknflash.sh -f1
```

## Create `dsnXX` user accounts with default password `dsnXX` in `dialout` group

```bash
$ sudo scripts/create-users.sh -n2 -f0     # Creates dsn00, dsn01
$ sudo scripts/create-users.sh -n2 -f0 -D  # Removes dsn00, dsn01
```

## Setup CAN network interface

```bash
$ sudo scripts/slcanconfig.sh -d0 -i0      # attaches slcan0 to /dev/ttyACM0
$ sudo scripts/slcanconfig.sh -d0 -i0 -D   # detaches slcan0 from /dev/ttyACM0
```

## Configure user/group permissions for CAN and USB devices





