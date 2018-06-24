#!/bin/bash

OPTIND=1  # for getopt

acmnum="-1"
slcnum="-1"
detach="-1"

while getopts "hDd:i:" opt; do
  case "$opt" in
    h)
       echo "Synopsis: $0 [-h] [-d ttyACMX:int] [-i slcanY:int] [-D]";
       echo "Example 1: $0 -d3 -i0     # attaches slcan0 to /dev/ttyACM3";
       echo "Example 2: $0 -d3 -i0 -D  # detaches slcan0 from /dev/ttyACM3";
       exit 0;
       ;;
    d)
       acmnum=$OPTARG;
       if [ $acmnum -lt 0 ]; then
         echo "Bad acmnum."; exit 1;
       fi
       ;;
    i)
       slcnum=$OPTARG;
       if [ $slcnum -lt 0 ]; then
         echo "Bad slnum."; exit 1;
       fi
       ;;
    D)
       detach=1;
       ;;
  esac
done

if [ $acmnum -eq -1 -o $slcnum -eq -1 ]; then
  echo "We cannot. Provide -dX and -iY.";
  exit 1;
fi

SPEED=6                     # baudrate 500k
DEV="/dev/ttyACM$acmnum"    # serial device
ICAN="slcan$slcnum"         # network interface

if [ $detach -eq 1 ]; then
  echo "Detaching $ICAN and $DEV..."
  psline=`ps ax | grep slcand | grep $DEV | grep $ICAN`
  if [ $? -ne 0 ]; then
    echo "-  We cannot. Device not configured."; exit 1;
  fi
  pid=`echo $psline | awk -F" " '{if ($1) print $1}'`
  numpids=`echo $pid | wc -w`
  if [ $numpids -ne 1 ]; then
    echo "-  We cannot. Multiple slcand for this configuration."; exit 1;
  fi
  ifconfig $ICAN down
  if [ $? -ne 0 ]; then
    echo "-  We cannot."; exit 1;
  fi
  kill $pid
  ifconfig $ICAN down
  if [ $? -ne 0 ]; then
    echo "-  We cannot."; exit 1;
  fi
  slcan_attach -c -d $DEV
  if [ $? -ne 0 ]; then
    echo "-  We cannot."; exit 1;
  fi
  echo "+  Success."
else
  echo "Attaching $ICAN and $DEV..."

  # https://www.fischl.de/usbtin/linux_can_socketcan/
  slcan_attach -f -s$SPEED -o $DEV
  if [ $? -ne 0 ]; then
    echo "-  We cannot."; exit 1;
  fi
  slcand $DEV $ICAN 
  if [ $? -ne 0 ]; then
    echo "-  We cannot."; exit 1;
  fi
  ifconfig $ICAN up
  if [ $? -ne 0 ]; then
    echo "-  We cannot."; exit 1;
  fi
  echo "+  Success."
fi

