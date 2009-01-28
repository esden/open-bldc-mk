/* 
 * Open-BLDC Firmware, Firmware for BrushLess Drive Controllers
 * Copyright (C) 2009 Piotr Esden-Tempski <piotr at esden.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <avr/interrupt.h>
#include <stdint.h>

#include "fade.h"

#include "timer.h"

uint16_t timer_sw_counter = 0;

//#define TIMER_VAL (((uint16_t)TCNT0)|(((uint16_t)timer_overflow_counter)<<8))
#define TIMER_VAL TCNT0

void timer_init_0();

void timer_init(){
    timer_init_0();
}

void timer_init_0(){
    TIMSK = (1 << TOIE0); // Timer/Counter0 Overflow Interrupt Enable
    TCCR0 = (0 << CS02) | (1 << CS01) | (0 << CS00); // clk I/O/(No prescaling)
}

void timer_wait(uint8_t time){
    time = (uint8_t)TCNT0 + time;
    while((TCNT0 - time) & 0x80){asm("nop");}
}

/* start new stopwatch */
uint16_t timer_new_sw(uint16_t time){
    return (timer_sw_counter + time);
}

uint16_t timer_sw_check(uint16_t sw){
    return ((timer_sw_counter - sw) & 0x8000) >> 8;
}

ISR(TIMER0_OVF_vect){
    timer_sw_counter++;
    //fade();
}
