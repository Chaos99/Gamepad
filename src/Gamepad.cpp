/*
  Gamepad.cpp

  Copyright (c) 2022, Benjamin Aigner <beni@asterics-foundation.org>
  Implementation loosely based on:
  * Mouse library from https://github.com/earlephilhower/arduino-pico
  * Gamepad functions from Teensyduino https://github.com/PaulStoffregen

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Gamepad.h"
#include "Arduino.h"
#include <RP2040USB.h>

#include "tusb.h"
#include "class/hid/hid_device.h"

// Weak function override to add our descriptor to the TinyUSB list
void __USBInstallGamepad() { /* noop */ }

//================================================================================
//================================================================================
//	Gamepad/Gamepad

Gamepad_::Gamepad_(void)
{
    _use8bit = false;
    _autosend = true;
    memset(&data,0,sizeof(data));
	//_X_axis = _Y_axis = _Z_axis = _Zrotate = _triggerLeft = _triggerRight = _dpad = data.buttons = 0;
}

/** define the mapping of axes values
  default: axes methods are accepting values from 0-1023 (compatibility to other Gamepad libraries)
		and are mapped internally to int8_t
  if use8bit(true) is called, -127 to 127 values are used.*/
void Gamepad_::use8bit(bool mode)
{
	_use8bit = mode;
}

//if set, the gamepad report is not automatically sent after an update of axes/buttons; use send_now to update
void Gamepad_::useManualSend(bool mode)
{
	_autosend = !mode;
}

/** Maps values from 8bit signed or 10bit unsigned to report values
 * 
 * Depending on the setting via use8bit(), either values from 0-1023 or -127 - 127
 * are mapped.
 */
int Gamepad_::map8or10bit(int const value)
{
	if(_use8bit)
	{
		if     (value < -127) return -127;
		else if(value >  127) return 127;
		else               return value;
	} else {
		if(value < 0) return 0;
		if(value > 1023) return 1023;
		return map(value, 0, 1023, -127, 127);
	}
}

void Gamepad_::begin(void) 
{
}

void Gamepad_::end(void) 
{
}

void Gamepad_::button(uint8_t button, bool val)
{
	//I've no idea why, but without a second dword, it is not possible.
	//maybe something with the alignment when using bit set/clear?!?
	static uint32_t buttons_local = 0;

	if(button >= 1 && button <= 32) {
		if(val){
			buttons_local |= (1UL << (button-1));
		} else {
			buttons_local &= ~(1UL << (button-1));
		}
		
		data.buttons = buttons_local;
		if(_autosend) send_now();
	}
}

void Gamepad_::setButton(uint8_t btn, bool val)
{
  //simply call button, but we setButton uses 0-31; button 1-32
  button(btn+1, val);
}

void Gamepad_::LX(int val)
{
	data.x = map8or10bit(val);
	if(_autosend) send_now();
}
void Gamepad_::LY(int val)
{
	data.y = map8or10bit(val);
	if(_autosend) send_now();
}
void Gamepad_::RX(int val)
{
	data.x = map8or10bit(val);
	if(_autosend) send_now();
}
void Gamepad_::RY(int val)
{
	data.y = map8or10bit(val);
	if(_autosend) send_now();
}
void Gamepad_::triggerLeft(int val)
{
	data.rx = map8or10bit(val);
	if(_autosend) send_now();
}
void Gamepad_::triggerRight(int val)
{
	data.ry = map8or10bit(val);
	if(_autosend) send_now();
}
void Gamepad_::positionL(int X, int Y)
{
	data.x = map8or10bit(X);
	data.y = map8or10bit(Y);
	if(_autosend) send_now();
}

void Gamepad_::positionR(int X, int Y)
{
	data.z = map8or10bit(X);
	data.rz = map8or10bit(Y);
	if(_autosend) send_now();
}

//set the dpad value, from 0-360. -1 is rest position
void Gamepad_::dpad(int8_t direction)
{
	switch (direction)
	{
		case 0: data.hat = hid_gamepad_hat_t::GAMEPAD_HAT_CENTERED;
		break;
		case 1: data.hat = hid_gamepad_hat_t::GAMEPAD_HAT_UP;
		break;
		case 2: data.hat = hid_gamepad_hat_t::GAMEPAD_HAT_UP_RIGHT;
		break;
		case 3: data.hat = hid_gamepad_hat_t::GAMEPAD_HAT_RIGHT;
		break;
		case 4: data.hat = hid_gamepad_hat_t::GAMEPAD_HAT_DOWN_RIGHT;
		break;
		case 5: data.hat = hid_gamepad_hat_t::GAMEPAD_HAT_DOWN;
		break;
		case 6: data.hat = hid_gamepad_hat_t::GAMEPAD_HAT_DOWN_LEFT;
		break;
		case 7: data.hat = hid_gamepad_hat_t::GAMEPAD_HAT_LEFT;
		break;
		case 8: data.hat = hid_gamepad_hat_t::GAMEPAD_HAT_UP_LEFT;
		break;
	}
	if(_autosend) send_now();
}

//send back the Gamepad report
void Gamepad_::getReport(hid_gamepad_report_t *report)
{
  memcpy(report,&data,sizeof(data));
}
  
//immediately send an HID report
void Gamepad_::send_now(void)
{
	CoreMutex m(&__usb_mutex);
    tud_task();
    if (tud_hid_ready()) {
		tud_hid_n_report(0, __USBGetGamepadReportID(), &data, sizeof(data));
    }
    tud_task();
}

Gamepad_ Gamepad;
