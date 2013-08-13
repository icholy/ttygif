#!/bin/bash

output=${1-"output.gif"}

if [ ! -e $output ]; then
  echo "[-] $output does not exist"
  exit
fi

echo "[+] setting frame delays ..."

for gif in *.gif; do

    if [ $gif == $output ]; then
      continue
    fi

    name=${gif%.gif};
    index=$(echo ${name%_*} | sed 's/0*//')
    delay=$((${name#*_}))
    ticks=$(echo "$delay * 0.1" | bc)

    if [ -z "$index" ]; then
      index=0
    fi

    echo "frame: $index, delay: $delay, ticks: $ticks"

    convert $output \( -clone $index -set delay $ticks \) -swap $index +delete \( +clone -set delay $ticks \) +swap +delete $output

    if [ $? -ne 0 ]; then
      echo "[-] error setting frame delay"
      exit
    fi

done;
