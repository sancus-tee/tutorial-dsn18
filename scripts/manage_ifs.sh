#!/bin/bash

USER_PREFIX="dsn"
USER_EXPIRE="+10 days"


num_users=1
first_user=0
delete=0

while getopts "hDn:f:" opt; do
  case "$opt" in
    h)
       echo "Synopsis: $0 [-h] [-n num_users:int] [-f first_user:int] [-D]";
       echo "Example 1: $0 -n2 -f1     # creates ${USER_PREFIX}01, ${USER_PREFIX}02";
       exit 0;
       ;;
    n)
       num_users=$OPTARG;
       if [ $num_users -lt 0 ]; then
         echo "Bad num_users."; exit 1;
       fi
       ;;
    f)
       first_user=$OPTARG;
       if [ $first_user -lt 0 ]; then
         echo "Bad first_user."; exit 1;
       fi
       ;;
    D)
       delete=1;
       ;;
  esac
done

firstunum=$first_user
lastunum=$(expr $first_user + $num_users - 1)

acmnum=0  # /dev/ttyACM${acmnum}
slcnum=0  # slcan${slcnum}
ttyctr=0  # /dev/ttyUSB${ttyctr} -- program upload
ttydbg=1  # /dev/ttyUSB${ttydbg} -- debug output

for (( unum=$firstunum; unum<=$lastunum; unum++ )); do
  USR=$(printf "%b%02d" $USER_PREFIX $unum)

  echo "User $USR: /dev/ttyACM${acmnum} -> slcan${slcnum}:"
  echo -n "  "

  if [ $delete -eq 1 ]; then
    ./slcanconfig.sh -d${acmnum} -i${slcnum} -D
    if [ $? -ne 0 ]; then
      echo "failed."; exit 1;
    fi
    echo "deleted."
  else
    chown ${USR}:${USR} \
      /dev/ttyACM${acmnum} \
      /dev/ttyUSB${ttyctr} /dev/ttyUSB${ttydbg}
    if [ $? -ne 0 ]; then
      echo "failed."; exit 1;
    fi
    ./slcanconfig.sh -d${acmnum} -i${slcnum}
    if [ $? -ne 0 ]; then
      echo "failed."; exit 1;
    fi
    echo "created."
  fi

  acmnum=$(( $acmnum + 1 ))
  slcnum=$(( $slcnum + 1 ))
  ttyctr=$(( $ttyctr + 2 ))
  ttydbg=$(( $ttydbg + 2 ))
done

