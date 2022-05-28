#include "ChatterButton.h"

#define BUTTON_PIN 2 //pin 2 for interrupt 0

ChatterINTButton button(CButtonWire::GND); /* if button connect to GND */

//Helper for prevent first interrupt fire after INPUT_PULLUP init
volatile boolean _int_guard = true;

// --------------------------------

void setup() {
    Serial.begin(9600);
    while (!Serial);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Button interrupts
    pinMode(BUTTON_PIN, INPUT_PULLUP); /* set 'INPUT' if button connect to Vdd */
    attachInterrupt(0, buttonInterrupt, CHANGE);

    Serial.println("<Startup>");
    _int_guard = false;
}

// --------------------------------

void loop() {
    if (button.didChange()) {
        digitalWrite(LED_BUILTIN, static_cast<bool>(button));
        Serial.print("Previous state duration: " + String(button.getPreviousStateDuration()) +" ms" + "     ");
        Serial.println("button " + String(button ? "pressed" : "released"));
    }
}

//===================== Interrupt =====================
void buttonInterrupt() {
    if (_int_guard) return;

    button.update((PIND >> 2) & 1);
}
