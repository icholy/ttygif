#!/usr/bin/env bash

# avoid deleting file, if convert will fail
set -e

output=${1-"output.gif"}
prev_delay=0
prev_img=""
skipped=0

uname=`uname -s`
if [ "$uname" == 'Darwin' ]; then
	format="png"
else
	format="xwd"
fi

imgs=$(find . -maxdepth 1 -name "*.$format"| grep -v "$output" | sort | xargs)

# remove -loop 0 if you don't want it to repeat
_convert="convert -loop 0 "

for img in $imgs; do

    file=${img##*/} 
    name=${file%.$format}
    delay=$(echo "${name#*_} * 0.1" | bc)

    # remove this is you don't want to trim zero delay frames
    if [ -n "$prev_img" ] && [ $delay == 0 ] && [ $prev_delay == 0 ]; then
        if [ $skipped -lt 5 ]; then
          skipped=$(($skipped + 1))
          prev_delay=$delay
          continue
        else
          skipped=0
        fi
    fi

    prev_delay=$delay

    if [ -n "$prev_img" ]; then
        _convert="$_convert -delay $delay $prev_img"
    fi
    prev_img=$img
done;
_convert="$_convert $img"

_convert="$_convert -layers Optimize $output"

echo "creating animated gif: $output"

eval "$_convert"

echo "deleting temporary gifs"

rm $imgs

