#!/bin/bash

output=${1-"output.gif"}
prev_delay=0
skipped=0

pngs=$(find . -maxdepth 1 -name '*.png'| grep -v "$output" | sort | xargs)

# remove -loop 0 if you don't want it to repeat
_convert="convert -loop 0 "

for png in $pngs; do

    file=${png##*/} 
    name=${file%.png}
    delay=$(echo "${name#*_} * 0.1" | bc)

    # remove this is you don't want to trim zero delay frames
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

    _convert="$_convert -delay $delay $png"
done;

_convert="$_convert -layers Optimize $output"

echo "creating animated gif: $output"

eval "$_convert"

echo "deleting temporary pngs"

rm $pngs

