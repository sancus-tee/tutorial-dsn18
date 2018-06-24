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

for (( unum=$firstunum; unum<=$lastunum; unum++ )); do
  USR=$(printf "%b%02d" $USER_PREFIX $unum)
  EXP=$(date -d "$USER_EXPIRE" "+%Y-%m-%d")
  echo -n "User $USR expires on $EXP: "

  if [ $delete -eq 1 ]; then
    userdel -r $USR
    if [ $? -ne 0 ]; then
      echo "failed."; exit 1;
    fi
    echo "deleted."
  else
    useradd --groups dialout --expiredate $EXP -m -U $USR
    if [ $? -ne 0 ]; then
      echo "failed."; exit 1;
    fi
    echo "$USR:$USR" | chpasswd
    if [ $? -ne 0 ]; then
      echo "failed."; exit 1;
    fi
    echo "created."
  fi
done

