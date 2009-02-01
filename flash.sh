#!/bin/sh
avrdude -p m8 -c avrispv2 -P usb -U flash:w:open-bldc.hex #-U lfuse:w:0x84:m -U hfuse:w:0xDA:m
