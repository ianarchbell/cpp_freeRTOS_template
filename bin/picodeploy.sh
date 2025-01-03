#!/bin/bash

export PICO_SWD=1

echo "PICO_SWD = " ${PICO_SWD}

if [ X${PICO_SWD} == X ]
then
	echo "PICO_SWD is not. Defaulting to RPI"
    export PICO_SWD="RPI"
fi

elf=`realpath build/src/*.elf`
if [ X${1} != X ]
then
   elf=`realpath ${1}`
fi

echo "Using openocd interface ${PICO_SWD} to flash ${elf}"

interface=cmsis-dap.cfg
speed=5000
target=rp2040.cfg
if [ ${PICO_SWD} == "RPI" ]
then
	speed=1000
    interface=raspberrypi-swd.cfg
fi
if [ ${PICO_SWD} == "PICOPROBE" ]
then
    interface=picoprobe.cfg
fi
if [ ${PICO_SWD} == "DEBUGPROBE" ]
then
    interface=cmsis-dap.cfg
fi
checkrp2350=`picotool info ${elf} | grep RP2350 | wc -l`
if [ ${checkrp2350} -eq 1 ]
then
    target=rp2350.cfg
fi

arm-none-eabi-size ${elf}
openocd -f interface/${interface} -f target/${target} -c "adapter speed ${speed}" -c "program ${elf} verify; reset halt ; rp2040.core1 arp_reset assert 0 ; rp2040.core0 arp_reset assert 0 ; exit"

