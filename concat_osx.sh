#!/usr/bin/env bash

output=${1-"output.gif"}
prev_png=""

pngs=$(find . -maxdepth 1 -name '*.png'| grep -v "$output" | sort | xargs)

# remove -loop 0 if you don't want it to repeat
_convert="convert -loop 0 "

for png in $pngs; do

    file=${png##*/} 
    name=${file%.png}
    delay=$(echo "${name#*_} * 0.1" | bc)

    if [ -n "$prev_png" ]; then
        _convert="$_convert -delay $delay $prev_png"
    fi
    prev_png=$png
done;
_convert="$_convert $png"

_convert="$_convert -layers Optimize $output"

echo "creating animated gif: $output"

eval "$_convert"

echo "deleting temporary pngs"

rm $pngs

