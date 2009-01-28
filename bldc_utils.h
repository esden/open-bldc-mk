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

#ifndef BLDC_UTILS_H
#define BLDC_UTILS_H

#define PWM_OFF() {                                                     \
        OCR1A = 0;      /* set output compare 1A */                     \
        OCR1B = 0;      /* set output compare 1B */                     \
        OCR2  = 0;      /* set output compare 2 */                      \
        TCCR1A = 0x01;  /* set WGM10 (PWM, Phase Correct, 8-bit) */     \
        TCCR2  = 0x41;  /* set WGM20 (PWM, Phase Correct) */            \
                        /* set CS20 (no prescaling) */                  \
        DDRB = 0x0E;    /* set PB[123] to output */                     \
        PORTB &= ~0x0E; /* disable PB[123] */                           \
    } ((void)0)

#define PWM_ON() {                                                      \
/* set COM1A1, COM1B1, WGM10 */                                         \
/* (Clear OC1A/OC1B on Compare Match when up-counting. */               \
/* Set OC1A/OC1B on Compare Match when downcounting.)  */               \
TCCR1A = 0xA1;                                                          \
/* set WGM20, COM21, CS20 */                                            \
/* (Clear OC2 on Compare Match when up-counting. */                     \
/* Set OC2 on Compare Match when downcounting.) */                      \
TCCR2  = 0x61;                                                          \
} ((void)0)

#define PWM_A_ON() {                                                    \
        PWM_ON();                                                       \
        DDRB = 0x08;                                                    \
    } ((void)0)
#define PWM_B_ON() {                                                    \
        PWM_ON();                                                       \
        DDRB = 0x04;                                                    \
    } ((void)0)
#define PWM_C_ON() {                                                    \
        PWM_ON();                                                       \
        DDRB = 0x02;                                                    \
    } ((void)0)

#define PWM_SET(VAL) {                          \
        OCR1A = VAL;                            \
        OCR1B = VAL;                            \
        OCR2 = VAL;                             \
    } ((void) 0)

#define SET_A_H() { PWM_A_ON(); } ((void)0)
#define SET_B_H() { PWM_B_ON(); } ((void)0)
#define SET_C_H() { PWM_C_ON(); } ((void)0)

#define SET_A_L() {                             \
        PORTD &= ~0x30;                         \
        PORTD |= 0x08;                          \
    } ((void)0)
#define SET_B_L() {                             \
        PORTD &= ~0x28;                         \
        PORTD |= 0x10;                          \
    } ((void)0)
#define SET_C_L() {                             \
        PORTD &= ~0x18;                         \
        PORTD |= 0x20;                          \
    } ((void)0)

#define SET_ALL_OFF() {                         \
        PORTD &= ~0x38;                         \
        PWM_OFF();                              \
    } ((void)0)

#define FETS_OFF() {                            \
        PORTD &= ~0x38;                         \
        PORTB &= ~0x0E;                         \
    } ((void)0)

/* 
 * set ADMUX 
 * 
 * Using this macros we select on which phase we want to sense the
 * next BEMF signal.
 */
#define BEMF_SET_A() { ADMUX = 0; } ((void)0)
#define BEMF_SET_B() { ADMUX = 1; } ((void)0)
#define BEMF_SET_C() { ADMUX = 2; } ((void)0)

/* clear ACI (Analog Comparator Interrupt Flag) */
#define CLR_BEMF() { ACSR |= 0x10; } ((void)0)

/* check ACI */
#define BEMF (ACSR & 0x10)

/* check ACO (Analog Comparator Output) */
#define BEMF_L (!(ACSR & 0x20))
#define BEMF_H ((ACSR & 0x20))

#define ENABLE_BEMF_INT() { ACSR |= 0x0A; } ((void)0)
#define DISABLE_BEMF_INT() { ACSR &= ~0x0A; } ((void)0)

#define BEMF_FALLING_INT() { ACSR &= ~0x01; } ((void)0)
#define BEMF_RISING_INT() { ACSR |= 0x03; } ((void)0)
#define BEMF_TOGGLE_INT() { ACSR &= ~0x03; } ((void)0)

#endif /* BLDC_UTILS_H */
