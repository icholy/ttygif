#!/bin/bash

output=${1-"output.gif"}
prev_delay=0
skipped=0

gifs=$(find . -name '*.gif'| grep -v "$output" | sort | xargs)
_CONVERT="convert -loop 0 "

for gif in $gifs; do

    file=${gif##*/} 
    name=${file%.gif}
    index=$(echo ${name%_*} | sed 's/0*//')
    delay=$((${name#*_}))
    ticks=$(echo "$delay * 0.1" | bc)

    if [ $delay == 0 ] && [ $prev_delay == 0 ]; then
        if [ $skipped -lt 5 ]; then
          skipped=$(($skipped + 1))
          prev_delay=$delay
          continue
        else
          skipped=0
        fi
    fi

    prev_delay=$delay

    if [ -z "$index" ]; then
      index=0
    fi

    _CONVERT="${_CONVERT} -delay $ticks $gif"
done;

_CONVERT="${_CONVERT} -layers Optimize $output"

echo "creating animated gif: $output"

eval "$_CONVERT"

