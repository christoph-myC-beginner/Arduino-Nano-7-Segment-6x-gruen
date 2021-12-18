//Arduino Nano 7 Segment 6x gruen
#include <Arduino.h>

int latchPin = 7;  // RCLK (Register Clock / Latch) Pin des 74HC595 ist verbunden mit dem digitalen Pin 5
int clockPin = 8; // SRCLK (Shit Register Clock) Pin des 74HC595 ist verbunden mit dem digitalen Pin 6
int dataPin = 5;  // SER (Serial input) Pin des 74HC595 ist verbunden mit dem digitalen Pin 4
int enablePin = 6;

int testArr[2][16]= {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1}};

int delTime = 200;

//declare functions (functions are written below)
void putShiftRegister(int x);

void setup() {
  // put your setup code here, to run once:
  // Alle Pins fÃ¼r den 74HC595 des Arduino auf OUTPUT (=Ausgang) setzen
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i<2; i++) {
    putShiftRegister(i); 
    delay(delTime);
  }
}

void putShiftRegister(int x) {
  digitalWrite(enablePin, HIGH); //Low active !
  digitalWrite(latchPin, LOW);
  for (int k=0; k < 16; k++) {
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, testArr[x][k]);
    digitalWrite(clockPin, HIGH);
  }
  digitalWrite(latchPin, HIGH);
  digitalWrite(enablePin, LOW); //Low active !
}