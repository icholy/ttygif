#!/bin/bash

# avoid deleting file, if convert will fail
set -e

output=${1-"output.gif"}
prev_delay=0
prev_img=""
skipped=0

imgs=$(find . -maxdepth 1 -regex ".*/[0-9]\{5\}.*[png|gif]" | grep -v "$output" | sort | xargs)

# remove -loop 0 if you don't want it to repeat
_convert="convert -loop 0"

for img in $imgs; do

    file=${img##*/} 
    name=${file%.*}
    delay=$(echo "${name#*_} * 0.1" | bc)

    if [ $delay != 0 ] || [ $prev_delay != 0 ]; then
      _convert="$_convert -delay $delay $prev_img"
    fi

    prev_delay=$delay

    prev_img=$img
done;

_convert="$_convert -delay 500 $prev_img -layers Optimize $output"

echo "creating animated gif: $output"

eval "$_convert"

echo "deleting temporary imgs"

rm $imgs

