#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3);
#define BLYNK_PRINT DebugSerial
#include <BlynkSimpleStream.h>

char auth[] = "e39d369352544f04868729e961b7b91e";

// Declare and initialize pin variables
const int tempSensor = A14;
const int LDR = A0;
const int moistSensor = A8;
const int LED = 8;
const int motor = 7;
const int button = 3;

// Define virtual pins
#define tempSensorV V14
#define motorVirtualOnLEDV V9
#define motorVirtualOffLEDV V10
#define motorButtonV V3
#define daylightLEDV V8
#define greenlightLEDV V0
#define moistSensorV V18

int buttonState = LOW; /* For the virtual button, motorButtonV,
                          which switches the motor circuit */

// Declare virtual LEDs
WidgetLED motorVirtualOnLED(motorVirtualOnLEDV);
WidgetLED motorVirtualOffLED(motorVirtualOffLEDV);
WidgetLED daylightLED(daylightLEDV);
WidgetLED greenlightLED(greenlightLEDV);

void setup()
{
  // Blynk stuff
  DebugSerial.begin(9600);
  Serial.begin(9600);
  Blynk.begin(auth, Serial);

  // Set digital pin modes
  pinMode(motor, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(button, INPUT);
}

void loop()
{
  Blynk.run();
  // moistSensor
  Blynk.virtualWrite(moistSensorV, analogRead(moistSensor));
  delay(50);
  
  // motor, physical button (PWM), virtual button, virtual LEDs
  if (digitalRead(button) == LOW) // if the physical button is off
  {
    analogWrite(motor, 0);        // stop the motor
    motorVirtualOnLED.off();      // turn a virtual LED off
    motorVirtualOffLED.on();      // turn a virtual LED on
    delay(100);
  }
  else // if the physical button is on
  {
    if (analogRead(moistSensor) > 450)
    {
      analogWrite(motor, 0);        // stop the motor
      motorVirtualOnLED.off();      // turn a virtual LED off
      motorVirtualOffLED.on();      // turn a virtual LED on
      delay(100);
    }
    else
    {
      analogWrite(motor, 255);    // start the motor
      motorVirtualOnLED.on();  // turn a virtual LED on
      motorVirtualOffLED.off();  // turn a virtual LED off
      delay(100);
    }
  }
  
  // LDR, physical LED, virtual LEDs
  if (analogRead(LDR) >= 150) // if light is more than or equal to 150
  {
    analogWrite(LED, 0);   // turn off the physical LED
    daylightLED.on();         // turn a virtual LED on
    greenlightLED.off();      // turn a virtual LED off
    delay(50);
  }
  else                        // if light is less than 150
  {
    analogWrite(LED, 255);  // turn on he physical LED
    daylightLED.off();        // turn a virtual LED off
    greenlightLED.on();       // turn a virtual LED on
    delay(50);
  }
  
  // tempSensor, virtual gauge
  float mV = analogRead(tempSensor) / 1024.0 * 5000; // convert analog output to millivolt 
  float cel = mV / 10;                        // covert millivolt to Celsius
  Blynk.virtualWrite(tempSensorV, (int)cel);  // write the truncated celsius integer to the virtual gauge
  delay(100);
}
