# Gamepad library

## Methods

### `Gamepad.begin()`

Must be called before starting using the Gamepad emulation. To end control, use `Gamepad.end()`.

#### Syntax 

```
Gamepad.begin()
```

#### Parameters

None.

#### Returns

None.

#### Example

```
#include <Gamepad.h>

void setup() {
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  // Initialize the Gamepad library when button is pressed
  if (digitalRead(2) == LOW) {
    Gamepad.begin();
  }
}
```

#### See also


* [Gamepad.button()](#gamepadbutton)
* [Gamepad.end()](#gamepadend)
* [Gamepad.send_now()](#gamepadsend_now)
* [Gamepad.positionLeft()](#gamepadpositionleft)
* [Gamepad.LX()](#gamepadlx)
* [Gamepad.dpad()](#gamepaddpadt)
* [Gamepad.use8bit()](#gamepaduse8bit)
* [Gamepad.useManualSend()](#gamepaduseManualSend)

### `Gamepad.setButton()`

Updates gamepad buttons. Same functionality as
Gamepad.button(), but the button numbering starts with 0 here.

#### Notes

* 'button' parameter has a range of 0-31!

### `Gamepad.button()`

Updates a button of the USB gamepad.

#### Syntax 

```
Gamepad.button(1,true);
delay(250);
Gamepad.button(1,false);
```

#### Parameters

* `button`: number of Gamepad button (1-32), which status should be changed
* `val`: state of button, `true` for pressed, `false` for released
  
#### Returns

None.

#### Example

```
#include <Gamepad.h>

void setup() {
  pinMode(2, INPUT_PULLUP);
  Gamepad.begin();
}

void loop() {
  uint8_t i = 1; //counter for the button number 1-32
  if (digitalRead(2) == LOW) { //if button is pressed
    Gamepad.button(i,true); //"press" the Gamepad button
	delay(250); //wait for 0.25s
	Gamepad.button(i,false); //"release" the Gamepad button
	i = i + 1; //increment & use next Gamepad button number
	if(i > 32) i = 1; //we have 32 buttons available, wrap-around
  }
}
```

#### Notes

* Up to 32 buttons are available, numbered as button 1 to button 32.
* If manual_send is active, call `Gamepad.send_now()` to send an update to the host.

#### See also

* [Gamepad.send_now()](#gamepadsend_now)
* [Gamepad.useManualSend()](#gamepaduseManualSend)
* [Gamepad.LX()](#gamepadlx)
* [Gamepad.dpad()](#gamepaddpad)

  
### `Gamepad.end()`

Stops emulating the Gamepad connected to a computer. To start control, use `Gamepad.begin()`.

#### Syntax 

```
Gamepad.end()
```

#### Parameters

None.

#### Returns

None.

#### Example

```
#include <Gamepad.h>

void setup() {
  pinMode(2, INPUT_PULLUP);
  // Initiate the Gamepad library
  Gamepad.begin();
}

void loop() {
  // If the button is pressed, send a button 1 press / release
  if (digitalRead(2) == LOW) {
    Gamepad.button(1,true);
	delay(250);
	Gamepad.button(1,false);
    // Then end the Gamepad emulation
    Gamepad.end();
  }
}
```

#### See also

* [Gamepad.begin()](#gamepadbegin)

### `Gamepad.use8bit()`

Switch axis value range between 10bit and 8bit.
* Default: 10bit, range for an axis from 0 to 1023
* 8bit mode: range from -127 to 127.

__Note:__ due to the gamepad descriptor of tinyUSB, the maximum range is -127/127. 10bit mode enables mapping, not a higher resolution.


#### Syntax 

```
Gamepad.use8bit(true)
```

#### Parameters

* `mode`: true, if values from -127/127 are used. False to use a range from 0 to 1023.

#### Returns

None.

#### Example

```
#include <Gamepad.h>

void setup() {
  pinMode(2, INPUT_PULLUP);
  Gamepad.begin();
}

void loop() {
  //send middle position in default 10bit mode
  Gamepad.positionLeft(512,512);
  delay(250);
  //enable 8bit mode
  Gamepad.use8bit(true);
  //send middle position in 8bit mode
  Gamepad.positionLeft(0,0);
  delay(250);
  
  //send maximum left in 10bit mode
  Gamepad.use8bit(false);
  Gamepad.positionLeft(0,0);
  delay(250);
  //enable 8bit mode
  Gamepad.use8bit(true);
  //send left position in 8bit mode
  Gamepad.positionLeft(-127,-127);
}
```

#### See also

* [Gamepad.positionLeft()](#gamepadpositionleft)
* [Gamepad.LX()](#gamepadlx)
* [Gamepad.LY()](#gamepadly)
* [Gamepad.RX()](#gamepadrx)
* [Gamepad.RY()](#gamepadry)
* [Gamepad.TriggerLeft()](#gamepadtriggerleft)
* [Gamepad.TriggerRight()](#gamepadtriggerright)


### `Gamepad.useManualSend()`

To fully control transmitting the USB-HID reports, enable manual sending.
If disabled, each call to a function updating the Gamepad status (buttons, all axis, hat)
will send a HID report. If you update in a loop, the time between updates (at least 1ms) is too short and something might be not transmitted correctly.
If enabled, update all your axis values, buttons, hat and then send one report via `Gamepad.send_now()`.

#### Syntax 

```
Gamepad.useManualSend(true)
```

#### Parameters

* `mode`: false is sending report each Gamepad update, true enables manual sending via send_now().

#### Returns

None.

#### Example

```
#include <Gamepad.h>

void setup() {
  pinMode(2, INPUT_PULLUP);
  Gamepad.begin();
}

void loop() {
  if (digitalRead(2) == LOW) {
      // send data in 4 different reports
	  Gamepad.button(1,true);
	  Gamepad.button(2,true);
	  Gamepad.button(3,true);
	  Gamepad.button(4,true);

	  //enable manual send
	  Gamepad.useManualSend(true);

	  //send same data in one report
	  Gamepad.button(1,false);
	  Gamepad.button(2,false);
	  Gamepad.button(3,false);
	  Gamepad.button(4,false);
	  Gamepad.send_now();
  }
}
```

#### See also

* [Gamepad.send_now()](#gamepadsend_now)



### `Gamepad.send_now()`

Send a HID report now. Used together with manual sending, see `Gamepad.useManualSend()`.

#### Syntax 

```
Gamepad.send_now()
```

#### Parameters

None.

#### Returns

None.

#### Example

```
#include <Gamepad.h>

void setup() {
  pinMode(2, INPUT_PULLUP);
  Gamepad.begin();
  //enable manual sending in setup
  Gamepad.useManualSend(true);
}

void loop() {
  if (digitalRead(2) == LOW) {
      // update all buttons, but nothing is sent to the host
	  for(uint8_t i = 1; i<=32; i++) Gamepad.button(i,true);
	  Gamepad.LX(256);
	  Gamepad.triggerLeft(0);
	  
	  //now send in one HID report
	  Gamepad.send_now();
  }
}
```

#### See also

* [Gamepad.useManualSend()](#gamepadusemanualsend)


### `Gamepad.LX()`

Update left Thumbstick X axis.
__Note:__ If [manual send](#gamepadusemanualsend) is active, the value is sent to the host only after calling [send_now](#gamepadsend_now).
__Note:__ If in 10bit mode (default), the parameter is interpreted from 0 to 1023.
In 8bit mode from -127 to 127. The internal resolution is always 8bit. Change setting with [use8bit](#gamepaduse8bit).

#### Syntax 

```
Gamepad.LX(0)
```

#### Parameters

* `val`: value from 0 to 1023 (default) or -127 to 127 (8bit mode)

#### Returns

None.

#### Example

```
#include <Gamepad.h>

void setup() {
  pinMode(2, INPUT_PULLUP);
  Gamepad.begin();
}

void loop() {
  if (digitalRead(2) == LOW) {
	  Gamepad.LX(256);
	  delay(500);
	  Gamepad.LX(512);
  }
}
```

#### See also

* [Gamepad.LY()](#gamepadly)
* [Gamepad.RX()](#gamepadrx)
* [Gamepad.RY()](#gamepadry)
* [Gamepad.triggerLeft()](#gamepadtriggerleft)
* [Gamepad.triggerRight()](#gamepadtriggerright)
* [Gamepad.send_now()](#gamepadsend_now)
* [Gamepad.positionL()](#gamepadpositionl)
* [Gamepad.positionR()](#gamepadpositionr)
* [Gamepad.dpad()](#gamepaddpad)
* [Gamepad.use8bit()](#gamepaduse8bit)
* [Gamepad.useManualSend()](#gamepaduseManualSend)

### `Gamepad.LY()`

Update left thumbstick Y axis. Please refer to [Gamepad.LX()](#gamepadlx).

### `Gamepad.RX()`

Update right thumbstick X axis. Please refer to [Gamepad.LX()](#gamepadlx).

### `Gamepad.RY()`

Update right thumbstick Y axis. Please refer to [Gamepad.LX()](#gamepadlx).

### `Gamepad.triggerLeft()`

Left analog trigger value. Please refer to [Gamepad.LX()](#gamepadlx).

### `Gamepad.triggerRight()`

Right analog trigger value. Please refer to [Gamepad.LX()](#gamepadlx).

### `Gamepad.trigger()`

Same as [Gamepad.triggerLeft()](#gamepadtriggerleft).

### `Gamepad.positionL()`

Sets left thumbstick X and Y axis in one call. If autosending is active, one report is generated. Please refer to [Gamepad.LX()](#gamepadlx).


#### Syntax 

```
Gamepad.positionL(512,512)
```

#### Parameters

* `X`: value from 0 to 1023 (default) or -127 to 127 (8bit mode); for X axis
* `Y`: value from 0 to 1023 (default) or -127 to 127 (8bit mode); for Y axis

#### See also

* [Gamepad.LX()](#gamepadlx)
* [Gamepad.LY()](#gamepadly)
* [Gamepad.RX()](#gamepadrx)
* [Gamepad.RY()](#gamepadry)
* [Gamepad.positionR()](#gamepadpositionr)

### `Gamepad.positionR()`

Sets right thumbstick X and Y axis in one call. If autosending is active, one report is generated. Please refer to [Gamepad.positionL()](#gamepadpositionl).

### `Gamepad.dpad()`

Set the dpad value to selection angle or released. The dpad buttons can also be set with the [Gamepad.setBUtton()](#gamepadsetbutton) or [Gamepad.button()](#gamepadbutton)
functions just like normal single-button presses. As they are internally handled as an 8-directional switch, this function can also be used to map to those 8 directions directly.
Possible values are: [UP, UPPERRIGHT, RIGHT, LOWERRIGHT, DOWN, LOWERLEFT, LEFT, UPPERLEFT, RELEASE] or numerical values  -1 to 7.

#### Syntax 

```
Gamepad.dpad(-1), // released / no button pressed
Gamepad.dpad(UP), // up-button pressed
Gamepad.dpad(RELEASE), // released / no button pressed
Gamepad.dpad(3), // left and down buttons pressed
```

#### Parameters

* `direction` direction as enum [UP, UPPERRIGHT, RIGHT, LOWERRIGHT, DOWN, LOWERLEFT, LEFT, UPPERLEFT, RELEASE] 
* `direction` direction as number [-1..7] with -1 representing button released
*  
### `Gamepad.getReport()`

Get the last sent Gamepad HID report. Useful when sending this report to another HID device.

#### Syntax 

```
hid_gamepad_report_t data;
Gamepad.getReport(&data), // get data to our struct
```

#### Parameters

* `*report` Pointer to a hid_gamepad_report_t variable.
