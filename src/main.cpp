//Arduino Nano 7 Segment 6x gruen
#include <Arduino.h>

int latchPin = 7;  // RCLK (Register Clock / Latch) Pin des 74HC595 ist verbunden mit dem digitalen Pin 5
int clockPin = 8; // SRCLK (Shit Register Clock) Pin des 74HC595 ist verbunden mit dem digitalen Pin 6
int dataPin = 5;  // SER (Serial input) Pin des 74HC595 ist verbunden mit dem digitalen Pin 4
int enablePin = 6;
//                    a,b,c,d,e,f,g,p,0,0,6,5,4,3,2,1
int testArr[8][16]= {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
                     {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                     {0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                     {0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0},
                     {0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0},
                     {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
                     {1,1,1,1,1,1,0,1,0,0,1,0,0,0,0,0}};

int targetArr[8][16];

int digitArr[7][8]={{0,0,0,0,0,0,0,0}, //keine Stelle
                    {0,0,0,0,0,0,0,1}, //Stelle 1 von rechts
                    {0,0,0,0,0,0,1,0}, //Stelle 2 von rechts
                    {0,0,0,0,0,1,0,0}, //Stelle 3 von rechts
                    {0,0,0,0,1,0,0,0}, //Stelle 4 von rechts
                    {0,0,0,1,0,0,0,0}, //Stelle 5 von rechts
                    {0,0,1,0,0,0,0,0}};//Stelle 6 von rechts

int numArr[11][8]={{0,0,0,0,0,0,0,0}, //keine Ziffer
                   {1,0,0,1,1,1,1,1}, //Ziffer 1
                   {0,0,1,0,0,1,0,1},
                   {0,0,0,0,0,0,0,1},
                   {0,0,0,0,0,0,0,1},
                   {0,0,0,0,0,0,0,1},
                   {0,0,0,0,0,0,0,1},
                   {0,0,0,0,0,0,0,1},
                   {0,0,0,0,0,0,0,1},
                   {0,0,0,0,0,0,0,1}
                   {0,0,0,0,0,0,0,1}} //

int delTime = 800;

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
  for (int i = 0; i<8; i++) {
    //targetArr[i,j]=
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