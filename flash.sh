#!/bin/sh

avrdude -p m8 -c stk500v1 -P /dev/tty.usbserial-A30012Oe -U flash:w:open-bldc.hex -U lfuse:w:0x84:m -U hfuse:w:0xDA:m
