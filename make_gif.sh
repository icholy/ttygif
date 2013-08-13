#!/bin/bash

convert -delay 0 -loop 0 *.gif output.gif

for gif in *.gif; do

    name=${gif%.gif};
    index=$(echo ${name%-*} | sed 's/0*//')
    delay=$((${name#*-}))
    ticks=$(echo "$delay * 0.1" | bc)

    if [ -z "$index" ]; then
      index=0
    fi

    echo "frame: $index, delay: $delay"

    convert output.gif \( -clone $index -set delay $ticks \) -swap $index +delete \( +clone -set delay $ticks \) +swap +delete output.gif

done;
