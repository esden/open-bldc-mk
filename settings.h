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

#ifndef SETTINGS_H
#define SETTINGS_H

#define SPEED_DEMO 0 /* Run a speed ramp demo */

#define PWM_START 20 /* PWM to use for starting the motor */
#define PWM_MIN 30 /* Minimum pwm so that the motor runs */
#define PWM_MAX 255 /* Maximal allowed pwm value (unsigned char) */

#define BEMF_DEBOUNCE_COUNT 15 /* How many cycles to debounce the bemf signal */
#define BEMF_DEBOUNCE_DEC 1 /* How much to decrease the debounce count when a 
                               swing has been detected */

#define START_MAX 300 /* Maximal delay between commutations during the motor
                         startup */
#define START_MIN 25 /* Minimum delay between commuttations during the motor
                        startup */
#define START_DEC(T) (T/15+1) /* Formula for calculating the next commutation 
                                 delay in the startup signal generation */
#define START_MAINTAIN 10 /* How many commutations should be done when the 
                             maximal start signal frequency is reached */

#endif /* SETTINGS_H */
