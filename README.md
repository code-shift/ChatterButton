## ChatterButton

**Contacts debounce assistant for stable operation of buttons and other hardware triggers.**
 
 * works with buttons on GND or Vdd connect
 * time tracking of button's states
 * can check button's state directly or with callback function
 * can button update from main loop or with hardware interrupt
 * compatible with Arduino and some many other platforms
 

### Usage (see examples folder)
1. Create instance like:
`ChatterButton button(CButtonWire::GND);`
2. Set handler if needed:
`button.setHandler(buttonState);`

handler:
```
void buttonState(bool active) {
	digitalWrite(LED_BUILTIN, active);
    Serial.println("Previous state duration: " + String(button.getPreviousStateDuration()) +" ms");
    Serial.println("<method A> button " + String(active ? "pressed" : "released"));
}
```
3. Organize update button state in runloop:
```
void loop() {
    button.update(digitalRead(BUTTON_PIN));
}
```
4. ***Bingo!***

*You can check button state in hadler function or direct button poll:*
```
if (button.didChange()) {
	Serial.println("String(button ? "pressed" : "released")); 
}
```

### Some features
* Select button's connection closed on GND or Vdd (positive) also it use to correct connect inverted (normal close) button in constructor
`ChatterButton(CButtonWire) // CButtonWire has ::GND and ::Vdd options`

* Request current button state from instance; true - active; once call will reset 'didChange()' flag
`if (button) { /*do something*/ }`

* Once check button state change; will reset after call 'operator bool()'
`if (button.didChange()) {...}`

* Force set button state; didChange() doesn't triggered; 
  (use for forced init etc)
`button.set(bool state);`

* Request duration of current button state; milliseconds
`button.getStateDuration();`

* Request duration of previous button state; milliseconds
`button.getPreviousStateDuration();`

*The MIT License (MIT)
Copyright* (c) 2022 0xdeb <0xdeb@mail.ru>
