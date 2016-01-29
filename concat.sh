#!/usr/bin/env bash

# avoid deleting file, if convert will fail
set -e

output=${1-"output.gif"}
prev_delay=0
prev_xwd=""
skipped=0

xwds=$(find . -maxdepth 1 -name '*.xwd'| grep -v "$output" | sort | xargs)

# remove -loop 0 if you don't want it to repeat
_convert="convert -loop 0 "

for xwd in $xwds; do

    file=${xwd##*/} 
    name=${file%.xwd}
    delay=$(echo "${name#*_} * 0.1" | bc)

    # remove this is you don't want to trim zero delay frames
    if [ -n "$prev_png" ] && [ $delay == 0 ] && [ $prev_delay == 0 ]; then
        if [ $skipped -lt 5 ]; then
          skipped=$(($skipped + 1))
          prev_delay=$delay
          continue
        else
          skipped=0
        fi
    fi

    prev_delay=$delay

    if [ -n "$prev_xwd" ]; then
        _convert="$_convert -delay $delay $prev_xwd"
    fi
    prev_xwd=$xwd
done;
_convert="$_convert $xwd"

_convert="$_convert -layers Optimize $output"

echo "creating animated gif: $output"

eval "$_convert"

echo "deleting temporary gifs"

rm $xwds

