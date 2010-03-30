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
#include <avr/interrupt.h>
#include <stdint.h>

#include "settings.h"
#include "timer.h"
#include "led.h"
#include "twi.h"

#include "bldc_utils.h"
#include "bldc.h"

volatile uint8_t bldc_phase = 0;
volatile uint16_t bldc_cnt_comm = 0;
uint8_t bldc_pwm = 0;
uint8_t bldc_pwm_max = PWM_MAX;
uint8_t bldc_running = 0;
uint8_t bldc_do_start = 1;
uint16_t bldc_stop_detect_timer;
uint8_t bldc_old_phase = 7;

void bldc_set_pwm();
int8_t bldc_start();
void bldc_set_comm();

void bldc_init(){
    DDRD |= 0x38;
    PORTD = 0x00;
    DDRB |= 0x0E;
    PORTB = 0x31;

    PWM_OFF();

    /* enable analog comperator */
    SFIOR = 0x08;
    BEMF_SET_B();

    /* PWM generator timer */
    TCCR1B = (1 << CS10) | 
        (0 << CS11) | 
        (0 << CS12) | /* clkI/O /1 (No prescaling) */
        (0 << WGM12) | 
        (0 << WGM13) | 
        (0 << ICES1) | 
        (0 << ICNC1); 
}

void bldc_set_pwm(){
    unsigned char pwm_tmp;
    pwm_tmp = bldc_pwm;
    if(pwm_tmp > bldc_pwm_max){
        pwm_tmp = bldc_pwm_max;
        LED_RED_ON();
    }

    PWM_SET(pwm_tmp);
}

/**
 * Generates a manual field rotation to give an initial impulse to the
 * motor.
 */
int8_t bldc_start(){
    unsigned long timer = START_MAX, i;
    unsigned char maintain = START_MAINTAIN;

    bldc_phase = 0;
    DISABLE_BEMF_INT();
    bldc_pwm = PWM_START;
    bldc_set_pwm();

    bldc_set_comm();
    while(1){
        for(i=0; i<timer; i++){
            timer_wait(100);
        }
        timer -= START_DEC(timer);
        if(timer < START_MIN){
            if(maintain){
                timer = START_MIN;
                maintain--;
            }else{
                return 1;
            }
        }
        bldc_set_comm();
        bldc_phase++;
        bldc_phase %= 6;
        if(BEMF){
            LED_GREEN_TOGGLE();
        }
    }

    return 0;
}

#if SPEED_DEMO == 1

/**
 * Run a ramp up and down of motor speeds
 */
void bldc_speed_demo(){
    static uint16_t i = 0;
    static int8_t dir=1;

    if(i>500){
        i=0;
        bldc_pwm+=dir;
        bldc_set_pwm();
        if(bldc_pwm>150){
            dir=-1;
        }
        if(bldc_pwm<16){
            dir=1;
        }
    }else{
        i++;
    }
}
#endif

/**
 * Monitor the motor and control it's speed.
 */
void bldc_run(){

    bldc_stop_detect_timer = timer_new_sw(250);
    while(1){
        if(!bldc_running){
            LED_GREEN_OFF();
        }else{
#if SPEED_DEMO == 1
            bldc_speed_demo();
#endif
        }
        if (twi_data != bldc_pwm) {
            bldc_pwm = twi_data;
            bldc_set_pwm();
        }

        if(bldc_old_phase != bldc_phase){
            bldc_stop_detect_timer = timer_new_sw(270);
            bldc_running = 1;
            bldc_old_phase = bldc_phase;
        }

        if(timer_sw_check(bldc_stop_detect_timer)){
            LED_RED_ON();
            DISABLE_BEMF_INT();
            bldc_running = 0;
            bldc_do_start = 1;
            SET_ALL_OFF();
        }

        if(bldc_do_start){
            LED_RED_OFF();
            bldc_do_start=0;
            if(bldc_start()){
                LED_GREEN_ON();
                bldc_running = 1;
                BEMF_TOGGLE_INT();
                ENABLE_BEMF_INT();
                bldc_pwm = PWM_MIN;
                bldc_set_pwm();
                bldc_stop_detect_timer = timer_new_sw(300);
                while(!timer_sw_check(bldc_stop_detect_timer)){asm("nop");}
                bldc_old_phase = 7;
            }else bldc_do_start=1;
        }
    }
}

/**
 * Enforce commutation setting.
 */
void bldc_set_comm(){
    switch(bldc_phase){
    case 0:
        SET_A_H();
        SET_B_L();
        BEMF_SET_C();
        BEMF_RISING_INT();
        break;
    case 1:
        SET_A_H();
        SET_C_L();
        BEMF_SET_B();
        BEMF_FALLING_INT();
        break;
    case 2:
        SET_B_H();
        SET_C_L();
        BEMF_SET_A();
        BEMF_RISING_INT();
        break;
    case 3:
        SET_B_H();
        SET_A_L();
        BEMF_SET_C();
        BEMF_FALLING_INT();
        break;
    case 4:
        SET_C_H();
        SET_A_L();
        BEMF_SET_B();
        BEMF_RISING_INT();
        break;
    case 5:
        SET_C_H();
        SET_B_L();
        BEMF_SET_A();
        BEMF_FALLING_INT();
        break;
    }
}

/*
 * Handle commutation.
 *
 * The analog comparator compares the positive pin AIN0 and negative
 * pin AIN1. When the voltage on the positive pin AIN0 is higher then
 * the voltage on the negative pin AIN1, the Analog Comparator Output,
 * ACO, is set. The following interrupt gets called on ACO raise, fall
 * or toggle depending on the call of BEMF_RISING_INT(),
 * BEMF_FALLING_INT() or BEMF_TOGGLE_INT() macros.
 */
ISR(ANA_COMP_vect){
    unsigned char i;

    /* debounce the bemf signal */
    for(i=0; i<BEMF_DEBOUNCE_COUNT; i++){
        if(bldc_phase & 1){
            if(BEMF_L) i -= BEMF_DEBOUNCE_DEC;
        }else{
            if(BEMF_H) i -= BEMF_DEBOUNCE_DEC;
        }
    }

    bldc_set_comm();

    bldc_phase++;
    bldc_phase %= 6;

    bldc_cnt_comm++;
}
