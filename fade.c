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

#include <avr/io.h>

#include <stdint.h>

#include "led.h"

#include "fade.h"

#define LED_DUTY_CYCLE_SIZE 200
uint8_t led_bright_red_counter = 0;
uint8_t led_bright_red_dir = 1;
uint8_t led_bright_red = 0;
uint8_t led_bright_green_counter = 200;
uint8_t led_bright_green_dir = -1;
uint8_t led_bright_green = 0;
uint8_t led_duty_step = 0;

uint8_t calculate_led_bright_red();
uint8_t calculate_led_bright_green();

void fade(){
    if(led_duty_step == 0){
        LED_RED_ON();
        LED_GREEN_ON();
    }
    if(led_duty_step == led_bright_red){
        LED_RED_OFF();
    }
    if(led_duty_step == led_bright_green){
        LED_GREEN_OFF();
    }

    led_duty_step++;
    if(led_duty_step > LED_DUTY_CYCLE_SIZE){
        led_duty_step = 0;

        led_bright_red = calculate_led_bright_red();
        led_bright_green = calculate_led_bright_green();
    }
}

uint8_t calculate_led_bright_red(){
    led_bright_red_counter+=led_bright_red_dir;
    if(led_bright_red_counter == 200) led_bright_red_dir = -1;
    if(led_bright_red_counter == 0) led_bright_red_dir = 1;
    return led_bright_red_counter;
}

uint8_t calculate_led_bright_green(){
    led_bright_green_counter+=led_bright_green_dir;
    if(led_bright_green_counter == 200) led_bright_green_dir = -1;
    if(led_bright_green_counter == 0) led_bright_green_dir = 1;
    return (led_bright_green_counter * 0.3);
}
