#!/bin/sh

SDK=docker.cesanta.com:5000/esp8266-build-oss:1.3.0-cesanta

if [ "$1" = "RTOS" ]; then
    shift
    SDK=docker.cesanta.com:5000/esp8266-build-rtos:1.2.0-cesanta-r3
fi

V7DIR=$(dirname $(dirname $PWD))
DIR=/cesanta/${PWD##$V7DIR}
# Note: "//" in "//bin/bash" is a workaround for a boot2docker bug on Windows.
exec docker run --rm -it -v /$V7DIR:/cesanta ${SDK} //bin/bash -c \
  "cd $DIR && make $@ && python ./tools/showbreakdown.py"
