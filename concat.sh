#!/bin/bash

output=${1-"output.gif"}

gifs=$(find . -name '*.gif' | sort | xargs)

_CONVERT="convert "

for gif in $gifs; do

    file=${gif##*/} name=${file%.gif}
    index=$(echo ${name%_*} | sed 's/0*//')
    delay=$((${name#*_}))
    ticks=$(echo "$delay * 0.1" | bc)

    if [ -z "$index" ]; then
      index=0
    fi

    _CONVERT="${_CONVERT} -delay $ticks $gif"
done;

_CONVERT="${_CONVERT} -layers Optimize $output"

echo "creating animated gif: $output"

eval "$_CONVERT"

