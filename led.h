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

#ifndef LED_H
#define LED_H

#define LED_RED_ON()    PORTC |= (1 << 3)
#define LED_RED_OFF()   PORTC &= ~(1 << 3)
#define LED_RED_TOGGLE()   PORTC ^= (1 << 3)
#define LED_GREEN_ON()  PORTD |= (1 << 7)
#define LED_GREEN_OFF() PORTD &= ~(1 << 7)
#define LED_GREEN_TOGGLE() PORTD ^= (1 << 7)

void led_init();

#endif /* LED_H */
