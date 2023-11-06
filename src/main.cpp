#include "InertialMeasurementUnit.h"
#include "Lightsaber.h"

#ifndef ARDUINO
    #define A1 (15)
#endif

int howManyLed = 10;
int ledPin = 6;
int buttonPin = A1;

Lightsaber* lightsaber;

void setup() {
    Serial.begin(9600);

    Button* button;
    LedStrip* ledStrip;
    InertialMeasurementUnit* inertialMeasurementUnit;

    inertialMeasurementUnit = new InertialMeasurementUnit();
#ifdef ARDUINO
    button = new MicroButton(buttonPin);
    ledStrip = new MicroLedStrip(howManyLed, ledPin);
#else
    button = new NullButton();
    ledStrip = new NullLedStrip();
#endif
    lightsaber = new Lightsaber(*inertialMeasurementUnit, *ledStrip, *button);
}
void loop() { lightsaber->tick(); }


#ifndef ARDUINO
int main() { setup(); while (1) loop(); }
#endif


