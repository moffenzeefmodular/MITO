/*
    Designed by Ross Fish
    Coded by Ben Davis 3.15.17
    Digital 2 = Clock in
    Digital 3 = Reset in
    Digital 4 = 9 Trigger outputs
    Analog 0 - 5 = beat channels
    Analog 6 = Swing
    Analog 7 = PW
*/

#include <elapsedMillis.h>
int clockDuration = 1000;
boolean clockInput[2];
boolean resetInput[2];
int pw;
int masterCount;
int swing;
boolean gateOut[6];
int modValue[6];
int analogInputs[6];

elapsedMillis sinceClock;
elapsedMillis sinceOut[6];

void setup() {
  pinMode(2, INPUT); // clock in
  pinMode(3, INPUT); // reset in
  pinMode(4, OUTPUT); // out 1
  pinMode(5, OUTPUT); // out 2
  pinMode(6, OUTPUT); // out 3
  pinMode(7, OUTPUT); // out 4
  pinMode(8, OUTPUT); // out 5
  pinMode(9, OUTPUT); // out 6
  pinMode(10, OUTPUT); // SMT LED power indicator
  digitalWrite(10, HIGH);
}

void loop() {

  clockInput[1] = clockInput[0];
  clockInput[0] = digitalRead(2);
  
  if (digitalRead(3) == HIGH) // if high then reset 
  {
    masterCount = 0;
  }

  for (int i = 0; i < 6; i++)
  {
    analogInputs[i] = analogRead(i);
  }
  for (int i = 0; i < 6; i++)
  {
    modValue[i] = map(analogInputs[i], 0, 1013, 16, 1);
  }
  pw = (clockDuration * (analogRead(7) / 2046.)) + 5;
  
  if (clockInput[0] > clockInput[1]) //rising edge clock
  {
    masterCount++;
    for (int i = 0; i < 6; i++)
    {
      if (masterCount % modValue[i] == 0)
        sinceOut[i] = 0;
    }
    clockDuration = sinceClock;
    sinceClock = 0;
  }
  if (masterCount % 2 == 0)
    swing = 0;
  else
    swing = map(analogRead(6), 0, 1023, 0, 100);
  if (sinceClock < pw + swing && sinceClock > swing)
  {
    for (int i = 0; i < 6; i++)
    {
      if (masterCount % (modValue[i]) == 0)
        digitalWrite(i + 4, HIGH);
    }
  }
  else
    for (int i = 0; i < 6; i++)
    {
      if (sinceOut[i] > ((pw * modValue[i]) + swing))
        digitalWrite(i + 4, LOW);
    }
}


