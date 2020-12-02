#!/bin/bash

USER_PREFIX="dsn"


num_users=1
first_user=0
delete=0
user_prefix=${USER_PREFIX}
dont=0

while getopts "hdDn:f:u:" opt; do
  case "$opt" in
    h)
       echo "Synopsis: $0 [-h] [-n num_users:int] [-f first_user:int] \\"
       echo "  [-u user_prefix:str] [-D] [-d]";
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
    u) user_prefix=$OPTARG;
       ;;
    d) dont=1;
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
  USR=$(printf "%b%02d" $user_prefix $unum)

  echo "User $USR: /dev/ttyACM${acmnum} -> slcan${slcnum}:"
  echo -n "  "

  if [ $delete -eq 1 ]; then
    if [ $dont -eq 0 ]; then
      ./slcanconfig.sh -d${acmnum} -i${slcnum} -D;
    fi
    if [ $? -ne 0 ]; then
      echo "failed."; exit 1;
    fi
    echo "deleted."
  else
    if [ $dont -eq 0 ]; then
      chown ${USR}:${USR} \
        /dev/ttyACM${acmnum} \
        /dev/ttyUSB${ttyctr} /dev/ttyUSB${ttydbg};
    fi
    if [ $? -ne 0 ]; then
      echo "failed."; exit 1;
    fi
    if [ $dont -eq 0 ]; then
      ./slcanconfig.sh -d${acmnum} -i${slcnum};
    fi
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

