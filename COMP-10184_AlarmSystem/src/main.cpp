// I Petar Kirkov, 000800914 certify that this material is my original work. No other person's work has been used without due acknowledgement. I have not made my work available to anyone else.
//
// COMP-10184 – Mohawk College
// Project 1 - Alarm System
// Completed up to stage 5
//
// @author  Petar Kirkov
// @id      000800914
//
// I created this work and I have not shared it with anyone else.
//

#include <Arduino.h>

// digital input pin definitions
#define PIN_PIR D5
#define PIN_BUTTON D6

// LED blink delay, timer and button press delay definitions
#define BLINK_DELAY 125
#define BUTTON_DELAY 300
#define TEN_SECOND_DELAY 10000

bool ledON = false;         // Variable used to indicate whether the LED is ON or OFF
bool alarmDisabled = false; // Variable used to indicate whether the alarm is disabled or enabled

// Variables used to control LED blinking
unsigned long startBlink;
unsigned long endBlink;

int printMessage = 0; // Flag used for printing a message telling the user that the alarm is activated ( used as a conditional to print the message once )

// Variable which contains button input
int iButton;

// ******************************************************************************************
void setup()
{
  // Print project info
  Serial.println("COMP-10184 - Alarm System\nName: Petar Kirkov\nStudent ID: 000800914");

  // configure the USB serial monitor
  Serial.begin(115200);

  // configure the LED output
  pinMode(LED_BUILTIN, OUTPUT);

  // PIR sensor is an INPUT
  pinMode(PIN_PIR, INPUT);

  // Button is an INPUT
  pinMode(PIN_BUTTON, INPUT_PULLUP);
}

// ******************************************************************************************
// Function which disables the alarm when the button is pressed
void disableAlarm()
{
  // Turn the D1 mini LED off
  ledON = false;

  // Read button input
  iButton = digitalRead(PIN_BUTTON);

  // If button is pressed, re-enable the alarm
  if (iButton == 0)
  {
    Serial.println("Alarm enabled!");
    alarmDisabled = false;
    delay(BUTTON_DELAY);
  }
}

// ******************************************************************************************
// Function which will activate the alarm 10 seconds after motion is detected by the sensor
void setAlarm()
{

  Serial.println("Motion Detected! - Alarm will sound in 10 seconds!");

  // Start the 10 second timer
  startBlink = millis();

  // End the timer when it reaches 10 seconds
  endBlink = startBlink + TEN_SECOND_DELAY;

  // While loop used to control the LED blinking
  while (startBlink < endBlink) 
  {

    // Read button input
    iButton = digitalRead(PIN_BUTTON);

    // If the button is pressed disable the alarm and do not read motion sensor input
    if (iButton == 0)
    {
      alarmDisabled = true;
      Serial.println("Alarm disabled!");
      delay(BUTTON_DELAY);
      break;
    }

    // Update blink timer
    startBlink = millis();

    // Controls LED blinking
    digitalWrite(LED_BUILTIN, LOW);
    delay(BLINK_DELAY);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(BLINK_DELAY);
  }

  // Turns on the LED solid and does not read further input (used to indicate that the alarm is active)
  ledON = true;
}

// ******************************************************************************************

void loop()
{
  // Variable which holds sensor input
  bool bPIR;

  // read PIR sensor (true = Motion detected!).  As long as there
  // is motion, this signal will be true.  About 2.5 seconds after
  // motion stops, the PIR signal will become false.
  bPIR = digitalRead(PIN_PIR);

  // If the alarm is disabled
  // Call function to disable the alarm
  if (alarmDisabled)
    disableAlarm();

  // If the LED is turned on solid
  if (ledON)
  {
    // Print message to monitor indicating that the alarm is activated
    if (printMessage == 0)
    {
      Serial.println("Alarm activated!");
      printMessage = 1;
    }

    // Set the LED light on solid
    digitalWrite(LED_BUILTIN, LOW);
  }

  // If no motion is detected, turn the LED off
  else if (!bPIR)
    digitalWrite(LED_BUILTIN, HIGH);

  // If the alarm is not disabled and motion is detected
  // Call function to set the alarm off
  else if (bPIR && !alarmDisabled)
    setAlarm();
}
