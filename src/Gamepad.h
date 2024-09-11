/*
  Gamepad.h

  Copyright (c) 2022, Benjamin Aigner <beni@asterics-foundation.org>

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

#ifndef _GAMEPAD_h
#define _GAMEPAD_h

#include <Arduino.h>
#include "class/hid/hid.h"

// using structs instead of enum classes to allow implicit int conversion
// and aliases

struct Dpad {
  static const uint8_t RELEASE = 0;
  static const uint8_t UP = 1;
  static const uint8_t UPPERRIGHT = 2;
  static const uint8_t RIGHT = 3;
  static const uint8_t LOWERRIGHT = 4;
  static const uint8_t DOWN = 5;
  static const uint8_t LOWERLEFT = 6;
  static const uint8_t LEFT = 7;
  static const uint8_t UPPERLEFT = 8;
};

struct Gamepad_Button {
  static const uint8_t A       = 0;
  static const uint8_t SOUTH   = 0;
  static const uint8_t CROSS   = 0;

  static const uint8_t B       = 1;
  static const uint8_t EAST    = 1;
  static const uint8_t CIRCLE   = 1;

  static const uint8_t C       = 2;

  static const uint8_t X       = 3;
  static const uint8_t NORTH   = 3;
  static const uint8_t TRIANGLE= 3;

  static const uint8_t Y       = 4;
  static const uint8_t WEST    = 4;
  static const uint8_t SQUARE  = 4;

  static const uint8_t Z       = 5;
  static const uint8_t TL      = 6;
  static const uint8_t TR      = 7;
  static const uint8_t TL2     = 8;
  static const uint8_t TR2     = 9;
  static const uint8_t SELECT  = 10;
  static const uint8_t START   = 11;
  static const uint8_t MODE    = 12;
  static const uint8_t THUMBL  = 13;
  static const uint8_t THUMBR  = 14;
};

//======================================================================
class Gamepad_
{
private:
  bool _autosend;
  bool _use8bit;
  hid_gamepad_report_t data;
  int map8or10bit(int const value);
public:
  Gamepad_(void);
  void begin(void);
  void end(void);
  
  //set a selected button to pressed/released
  void button(uint8_t button, bool val);
  //same call as button, but "button" starts with 0
  void setButton(uint8_t btn, bool val);
  //set axis values
  void LX(int val);
  void LY(int val);
  void RX(int val);
  void RY(int val);
  void positionL(int X, int Y);
  void positionR(int X, int Y);
  void triggerLeft(int val);
  void triggerRight(int val);

  
  //dapd translates 8 directions into 1 or 2 simultanious button presses out of 4 buttons
  void dpad(int8_t direction);  
  
  //if set, the gamepad report is not automatically sent after an update of axes/buttons; use send_now to update
  void useManualSend(bool mode);
  //immediately send an HID report
  void send_now(void);
  //define the mapping of axes values
  //default: axes methods are accepting values from 0-1023 (compatibility to other Gamepad libraries)
  // and are mapped internally to int8_t
  //if use8bit(true) is called, -127 to 127 values are used.
  void use8bit(bool mode);

  //get the gamepad report which is sent last.
  void getReport(hid_gamepad_report_t *report);
};
extern Gamepad_ Gamepad;

#endif /* _Gamepad_h */
