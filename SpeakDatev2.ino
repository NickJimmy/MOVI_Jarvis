#include <Time.h>
#include <TimeLib.h>

/****************************************************************************
 * This is an example for the use of Audeme's MOVI(tm) Voice Control Shield *
 * ----> http://www.audeme.com/MOVI/                                        *
 * This code is inspired and maintained by Audeme but open to change        *
 * and organic development on GITHUB:                                       *
 * ----> https://github.com/audeme/MOVIArduinoAPI                           *
 * Written by Gerald Friedland for Audeme LLC.                              *
 * Contact: fractor@audeme.com                                              *
 * BSD license, all text above must be included in any redistribution.      *
 ****************************************************************************
 *
 * This is an alternate version of the lightswitch demo that only uses one 
 * sentence and stores the state of the light in a variable. This technique 
 * can be very useful to cut down on the sentences to be recognized
 * if that's a problem with larger projects. 
 *
 * Circuitry:
 * LED is pin D13 and GND
 * Arduino UNO R3, MEGA2560 R3, or Arduino Leonardo R3.
 * Connect speaker to MOVI.
 * IMPORTANT: Always use external power supply with MOVI. 
 * 
 * Other Arduino-compatible boards:  
 * Consult MOVI's User Manual before connecting MOVI.
 *
 */
#include "MOVIShield.h"     // Include MOVI library, needs to be *before* the next #include

#ifdef ARDUINO_ARCH_AVR 
#include <SoftwareSerial.h> // This is nice and flexible but only supported on AVR architecture, other boards need to use Serial1 
#endif

MOVI recognizer(true);            // Get a MOVI object, true enables serial monitor interface, rx and tx can be passed as parameters for alternate communication pins on AVR architecture
boolean lighton=false;
String TimeString;
String TimeHour;
String TimeMin;
String Date;
String Month;

void setup()  
{
  
  recognizer.init();      // Initialize MOVI (waits for it to boot)

  //*
  // Note: training can only be performed in setup(). 
  // The training functions are "lazy" and only do something if there are changes. 
  // They can be commented out to save memory and startup time once training has been performed.
  Serial.begin(9600);
  setTime(1470252720);  // Enter time in Unix format, seconds elapsed since 1 Jan 1970! - can be calculated from Excel sheet
  recognizer.callSign("Jarvis"); // Train callsign Arduino (may take 20 seconds)
  recognizer.addSentence("Time"); // Add sentence 1
  recognizer.addSentence("Day"); // Add sentence 2
  recognizer.train();              // Train (may take 20seconds) 
  //*/

  //  recognizer.setThreshold(5);			// uncomment and set to a higher value (valid range 2-95) if you have a problems due to a noisy environment.
}

void loop() // run over and over
{
  signed int res=recognizer.poll(); // Get result from MOVI, 0 denotes nothing happened, negative values denote events (see docs)
  if (res==1) {                     // Sentence 1.
    recognizer.say("The Time is");
    if (hour()<10){
      TimeHour = '0' + String(hour());
    }
    else
    {
      TimeHour = String(hour());
    }
    TimeMin = String(minute());
    TimeString = TimeHour + ' ' + TimeMin;
    recognizer.say(TimeString);
    Serial.print("Time Function. ");
    Serial.print("**Time variable output: " + TimeHour + ' ' + TimeMin + "\t");
  } 
  if (res==2) {
    recognizer.say("The day of the month is");
    Date = String(day());
    recognizer.say(Date);
    recognizer.say("The month of the year is");
    Month = String(month());
    recognizer.say(Month);
    
    Serial.print("Date Function. ");
    Serial.print("**Date variable output: " + Date + "\t");
  }
}
