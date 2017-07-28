/*MIT License

Copyright (c) 2017 Christian Goertz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <Arduino.h>

#include <SDisplay.h>
#include <DCF77.h>
#include <Time.h>
/*makros & symbols*/
//SEGMENTBUS
#define DCF77_PIN 2
#define DCF77_INTERRUPT 0
#define SEG_A_PIN 3
#define SEG_B_PIN 4
#define SEG_C_PIN 5
#define SEG_D_PIN 6
#define SEG_E_PIN 7
#define SEG_F_PIN 8
#define SEG_G_PIN 9
#define LATCH_DIGIT1 A0
#define LATCH_DIGIT2 A1
#define LATCH_DIGIT3 A2
#define LATCH_DIGIT4 A3
#define DIGIT_COUNT 4
#define BAUD_RATE 115200
/*objects & global vars*/
//DCF77 Object that handles decoding the DCF77 signal
DCF77 DCF(DCF77_PIN,DCF77_INTERRUPT,true);
//SDisplay objects
//Digit 1 minutes lower digit
SDisplay Digit1(SEG_A_PIN, SEG_B_PIN,
                SEG_C_PIN, SEG_D_PIN,
                SEG_E_PIN, SEG_F_PIN,
                SEG_G_PIN, LATCH_DIGIT1, COM_ANODE);
//Digit 2 minutes upper digit
SDisplay Digit2(SEG_A_PIN, SEG_B_PIN,
                SEG_C_PIN, SEG_D_PIN,
                SEG_E_PIN, SEG_F_PIN,
                SEG_G_PIN, LATCH_DIGIT2, COM_ANODE);
//Digit 3 hours lower digit
SDisplay Digit3(SEG_A_PIN, SEG_B_PIN,
                SEG_C_PIN, SEG_D_PIN,
                SEG_E_PIN, SEG_F_PIN,
                SEG_G_PIN, LATCH_DIGIT3, COM_ANODE);
//Digit 4 hours upper digit
SDisplay Digit4(SEG_A_PIN, SEG_B_PIN,
                SEG_C_PIN, SEG_D_PIN,
                SEG_E_PIN, SEG_F_PIN,
                SEG_G_PIN, LATCH_DIGIT4, COM_ANODE);
//array of digits for fast iterate
SDisplay *digits[DIGIT_COUNT] = {&Digit1,&Digit2,&Digit3,&Digit4};
time_t time;
/* MCU init*/
void setup(){
  pinMode(DCF77_PIN,INPUT);
  Serial.begin(BAUD_RATE);
  Serial.println("DCF -> init");
  DCF.Start();
  delay(5000);

  for(uint8_t i = 0; i<DIGIT_COUNT;i++){
    digits[i]->clear();
    digits[i]->setOrientation(NOT_FLIPPED);
  }

}
void loop(){
  time_t DCFtime = DCF.getTime(); //Check if DCF-Time is available
  if(DCFtime !=0){
    Serial.println("Time is updated");
    setTime(DCFtime);
  }
  else{
    Serial.print("Zeit vergangen: ");
    Serial.print(millis() /1000);
    Serial.println("Sekunden");
  }
  Serial.print(hour());
  Serial.print(":");
  Serial.println(minute());
  Digit1.num(minute() %10);
  Digit2.num(minute() /10);
  Digit3.num(hour() %10);
  Digit4.num(hour() /10);
  delay(1000);
}
