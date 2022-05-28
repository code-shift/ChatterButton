#include "ChatterButton.h"

#define BUTTON_PIN 2

ChatterButton button(CButtonWire::GND); /* if button connect to GND */

// --------------------------------

void setup() {
    Serial.begin(9600);
    while (!Serial);

    pinMode(BUTTON_PIN, INPUT_PULLUP); /* set 'INPUT' if button connect to Vdd */

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    /* method A; check in handler function */
    button.setHandler(buttonState);

    Serial.println("<Startup>");
}

// --------------------------------

void loop() {
    button.update(digitalRead(BUTTON_PIN));

    /* method B; direct button poll */
    if (button.didChange()) {
        Serial.println("<method B> button " + String(button ? "pressed" : "released")); Serial.println();
    }
}

//-========= Button state changed (method A) =========-
void buttonState(bool active) {
    digitalWrite(LED_BUILTIN, active);
    Serial.println("Previous state duration: " + String(button.getPreviousStateDuration()) +" ms");
    Serial.println("<method A> button " + String(active ? "pressed" : "released"));
}
