/*
*
* This sketch controls the Bescor MP-101 Pan/Tilt system using the Wii Nunchuck
*
* Copyright (c) 2015 Matt Alford, http://protechy.com/bescor-mp-101-hack/
* Date: April 6, 2015
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
 
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses></http:>.
* I am not responsible for any damage that may occur from the use of this software.
* Nunchuck Library http://playground.arduino.cc/Main/WiiChuckClass
*
*/
 
#include "Wire.h"
#include "WiiChuck.h" // The library used by the Nunchuck
 
#define left 11 // Pin 11 controls left pan
#define right 3 // Pin 3 controls right pan
#define up 10 // Pin 10 controls up tilt
#define down 9 // Pin 9 controls down tilt
 
WiiChuck chuck = WiiChuck();
 
int varx, vary, absx, absy, mapx, mapy = 0;
 
void setup() {
 Serial.begin(115200); // Opening the serial port
 
 TCCR2B = TCCR2B & 0b11111000 | 0x07; // Adjusting PWM frequencies for testing pins 11 and 3
 TCCR1B = TCCR1B & 0b11111000 | 0x05; // Pins 9 and 10
 
 nunchuck_setpowerpins(); // use analog pins 2 & 3 as gnd & pwr (uncomment to use WiiChuck)
 
 chuck.begin();
 chuck.update();
}
 
// To power the WiiChuck Adapter
static void nunchuck_setpowerpins() {
#define pwrpin PORTC3
#define gndpin PORTC2
 DDRC |= _BV(pwrpin) | _BV(gndpin);
 PORTC &=~ _BV(gndpin);
 PORTC |= _BV(pwrpin);
 delay(100); // wait for things to stabilize 
}
 
void loop() {
 chuck.update(); // The Nunchuck values update in the loop
 
 // 130 is the distance from 0 on the joystick
 varx = chuck.readJoyX(); // nunchuk.analogX is the value of the x-axis
 vary = chuck.readJoyY(); // nunchuk.analogY is the value of the y-axis
 
 // The values used for speed
 absx = abs(varx); // Convert the x-axis value to an absolute value
 absy = abs(vary);
 
 // Map the x/y value to get the full range
 mapx = map(absx, 0, 100, 0, 115);
 mapy = map(absy, 0, 100, 0, 115);
 
 // Tilt based on the input from the joystick
 if (vary > 18) {
  analogWrite(up, mapy);
  digitalWrite(down, LOW);
 } else if (vary < -18) {
  analogWrite(down, mapy);
  digitalWrite(up, LOW);
 } else { // Stop tilt
  analogWrite(up, LOW);
  analogWrite(down, LOW);
 }
 
 // Pan based on the input from the joystick
 if (varx > 10) {
  analogWrite(right, mapx);
  digitalWrite(left, LOW);
 } else if (varx < -10) {
  analogWrite(left, mapx);
  digitalWrite(right, LOW);
 } else { // Stop pan
  analogWrite(right, LOW);
  analogWrite(left, LOW);
 }
 
 Serial.print(chuck.readJoyX());
 Serial.print("X, "); 
 Serial.print(chuck.readJoyY());
 Serial.print("Y, "); 
 Serial.print(mapx);
 Serial.print("MapX, "); 
 Serial.print(mapy);
 Serial.print("MapY, "); 
 Serial.println();
}
