//Arduino Nano 7 Segment 6x gruen
#include <Arduino.h>

int latchPin = 7;  // RCLK (Register Clock / Latch) Pin des 74HC595 ist verbunden mit dem digitalen Pin 5
int clockPin = 8; // SRCLK (Shit Register Clock) Pin des 74HC595 ist verbunden mit dem digitalen Pin 6
int dataPin = 5;  // SER (Serial input) Pin des 74HC595 ist verbunden mit dem digitalen Pin 4
int enablePin = 6;
/*
//                    a,b,c,d,e,f,g,p,0,0,6,5,4,3,2,1
int testArr[8][16]= {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
                     {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                     {0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                     {0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0},
                     {0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0},
                     {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
                     {1,1,1,1,1,1,0,1,0,0,1,0,0,0,0,0}};
*/
int target = 0b0000000000000000;

int digitArr[7]={0b00000000, //keine Stelle
                 0b10000000, //Stelle 1 von rechts
                 0b01000000, //Stelle 2 von rechts
                 0b00100000, //Stelle 3 von rechts
                 0b00010000, //Stelle 4 von rechts
                 0b00001000, //Stelle 5 von rechts
                 0b00000100};//Stelle 6 von rechts

//                pgfedcba 0=aktiv!
int numArr[12]={0b11000000, //Ziffer 0
                0b11111001, //Ziffer 1
                0b10100100, //Ziffer 2
                0b10110000, //Ziffer 3
                0b10011001, //Ziffer 4
                0b10010010, //Ziffer 5
                0b10000010, //Ziffer 6
                0b11111000, //Ziffer 7
                0b10000000, //Ziffer 8
                0b10010000, //Ziffer 9
                0b11111111, //keine Ziffer 
                0b01111111};//Punkt

int delTime = 400;

int pointFlag=1;

unsigned long vorherMillis;
uint16_t cntInterrupts;
uint16_t cntHunderstelSek;
uint16_t cntZehntelSek;
uint16_t cntSekunden;

//declare functions (functions are written below)
int simplePWMFlag(int cycleOn, int cycleFull);
void simpleBreathe(int pin, int cycleFull);
void putNumDigit(int num, int dig, bool pkt);
void putShiftRegister(int x);

ISR(TIMER2_COMPA_vect) {
  // timer2 interrupt to-do code here
  cntInterrupts = cntInterrupts + 1;
  if (cntInterrupts >= 5) {
    cntInterrupts = 0;
    cntHunderstelSek = cntHunderstelSek + 1;
  }
  if (cntHunderstelSek >= 10) {
    cntHunderstelSek = 0;
    cntZehntelSek = cntZehntelSek + 1;
  }
  if (cntZehntelSek >= 10) {
    cntZehntelSek = 0;
    cntSekunden = cntSekunden + 1;
  }
  OCR2A = 125;        // set compare match register prescaler 256; 125 -> 2ms
}

void setup() {
  // put your setup code here, to run once:
  // Alle Pins fÃ¼r den 74HC595 des Arduino auf OUTPUT (=Ausgang) setzen
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(13, OUTPUT);
  
  // timer2
  
  TCCR2A = 0; // set TCCR2A register to 0
  TCCR2B = 0; // set TCCR2B register to 0
  TCNT2  = 0; // set counter value to 0
  
  OCR2A = 255; // set compare match register
  
  TCCR2B |= (1<<CS02) | (1<<CS01)| (0<<CS00); // Set CS bits for 1:1024 prescaler 1, 1, 1 = 1024
                                              //                                  1, 1, 0 =  256
                                              //                                  1, 0, 1 =  128
                                              //                                  1, 0, 0 =   64
 
  TCCR2A |= (1 << WGM21); // turn off CTC mode
  TIMSK2 |= (1 << OCIE2A); // disable timer compare interrupt

  interrupts(); // Interrupts einschalten

}

void loop() {
  // put your main code here, to run repeatedly:
  
  //if (millis() - vorherMillis > 3) {
    //vorherMillis = millis();
    unsigned long zwiA;
    zwiA = cntZehntelSek;    //zehntel
    putNumDigit(zwiA, 1, 0); 
    zwiA = cntSekunden % 10; //einer
    putNumDigit(zwiA, 2, 1);
    zwiA = cntSekunden / 10; //einer herausrechnen
    zwiA = zwiA % 10;        //zehner
    putNumDigit(zwiA, 3, 0);   
    zwiA = cntSekunden / 100; //zehner herausrechnen
    zwiA = zwiA % 10;           //hunderter
    putNumDigit(zwiA, 4, 0);
    zwiA = cntSekunden / 1000; //hunderter herausrechnen
    zwiA = zwiA % 10;          //tausender
    putNumDigit(zwiA, 5, 0);
    zwiA = cntSekunden / 10000; //tausender herausrechnen
    zwiA = zwiA % 10;          //zehntausender
    putNumDigit(zwiA, 6, 0);
  //}
  //pointFlag = simplePWMFlag(1000,2000);
  //digitalWrite(13, pointFlag);
  
  simpleBreathe(13, 1000);
  pointFlag = digitalRead(13);
}

void simpleBreathe(int pin, int cycleFull){
  int pulsingPin = pin;
  static unsigned long previousMicros;
  static bool runUp;
  static uint16_t cycleOn;
  static uint16_t counter;
  if ((micros() - previousMicros) < cycleFull) {
    if ((micros() - previousMicros) < cycleOn) {
      digitalWrite(pulsingPin, HIGH);
    }
    else {
      digitalWrite(pulsingPin, LOW);
    }
  }
  else {
    previousMicros = micros();
    counter = counter + 1; 
    if (counter >= 120) { //circa Millisekunden
      counter = 0;
      if (runUp == 0) { //Von Dunkel zu Hell
        cycleOn = cycleOn + 100;
        if (cycleOn >= (cycleFull - 1)) {
          runUp = 1;
        }
      }
      else { //Von Hell zu Dunkel
        cycleOn = cycleOn - 100;
        if ((cycleOn) <= 1) {
          runUp = 0;
        }
      }
    }
  } 
}

int simplePWMFlag(int cycleOn, int cycleFull){
  //int pulsingPin = pin;
  static unsigned long previousMillis;
  if ((millis() - previousMillis) < cycleFull) {
    if ((millis() - previousMillis) < cycleOn) {
      //digitalWrite(pulsingPin, LOW);
      return 0;
    }
    else {
      //digitalWrite(pulsingPin, HIGH);
      return 1;
    }
  }
  else {
    previousMillis = millis();
  }
}

void putNumDigit(int num, int dig, bool pkt){
  if (pkt == 1) {
    target |= (digitArr[dig] << 8) | (numArr[num] & numArr[11]);
  }
  else {
    target |= (digitArr[dig] << 8) | numArr[num];
  }
  putShiftRegister(target); 
  target = 0;
}

void putShiftRegister(int x) {
  digitalWrite(enablePin, HIGH); //deactivate; Low active !
  digitalWrite(latchPin, LOW);
  for (int k=0; k < 16; k++) {
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, (x&1));
    digitalWrite(clockPin, HIGH);
    x = x >> 1;    
  }
  digitalWrite(latchPin, HIGH);
  digitalWrite(enablePin, LOW); //Low active !
}