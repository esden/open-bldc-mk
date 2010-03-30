/* 
 * Open-BLDC Firmware, Firmware for BrushLess Drive Controllers
 * Copyright (C) 2009 Piotr Esden-Tempski <piotr at esden.net>
 * 2010 David Kiliani <mail@davidkiliani.de>
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
#include <util/twi.h>

#include "twi.h"

uint8_t twi_data = 0;

void twi_init(){
    TWAR = SLAVE_ADR;
    TWCR = _BV(TWINT) | _BV(TWIE) | _BV(TWEN) | _BV(TWEA);
}

ISR(TWI_vect) {
	switch (TW_STATUS) {
	case TW_SR_SLA_ACK:
		TWCR |= _BV(TWINT);
		return;
	case TW_SR_DATA_ACK:
		twi_data = TWDR;
		TWCR |= _BV(TWINT);
		return;
    case TW_BUS_ERROR:
    case TW_NO_INFO:
        TWCR |= _BV(TWSTO) | _BV(TWINT);
	}
	twi_init();
}
