#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BH1750.h>
#include <math.h> 
#include <Servo.h>
#include <Arduino.h>
#include "AdvancedSerial.h"

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define analog_max 740


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
BH1750 lightMeter;

const int iso_tabelle[] = {100, 160, 200, 400, 800, 1600, 3200};
const int verschluss_tabelle[] = {100, 125, 150, 200, 250, 350, 
400, 450, 500, 550, 600, 650, 700, 750, 800, 900, 1000, 
1250, 1500, 1750, 2000, 3000, 4000, 8000, 16000, 32000};
const int blende_tabelle[] = {18, 28};

int winkelalt = 0;
int drehungen = 0;
int isoIndex;
int verschlussIndex;
int blendeIndex;

Servo myservo;
int pos = 0; 

void setup() {

Serial.begin(9600);
lightMeter.begin();

  
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
  Serial.println(F("SSD1306 allocation failed"));
  for(;;);
}

delay(2000);
display.clearDisplay();

pinMode(12, INPUT_PULLUP);
pinMode(11, INPUT_PULLUP);
pinMode(10, INPUT_PULLUP);
pinMode(9, INPUT_PULLUP);

myservo.attach(7);
myservo.write(133);

}

void loop() {

int Verschluss = digitalRead(12);
int Verschluss1 = digitalRead(11);
int iso_minus = digitalRead(10);
int iso_plus = digitalRead(9);

if (iso_minus == 0) { 
  isoIndex  = isoIndex + 6;
  delay(200);
}

if (iso_plus == 0) { 
  isoIndex  = isoIndex + 1;
  delay(200);
}

int iso = iso_tabelle[round(map((isoIndex%7)+0.1, 0, 7, 0, sizeof(iso_tabelle)  / sizeof(int)))];

verschlussIndex  = round(map(analogRead(A0), 0, analog_max, 0, sizeof(verschluss_tabelle)  / sizeof(int)));
int verschluss = verschluss_tabelle[verschlussIndex];

blendeIndex  = round(map(analogRead(A1), 0, analog_max, 0, sizeof(blende_tabelle)  / sizeof(int)));
float blende = blende_tabelle[blendeIndex];

float lux = lightMeter.readLightLevel();

if (analogRead(A3) <= 360){
  winkelalt = analogRead(A2);
}

if (analogRead(A3) > 360) {
  if (winkelalt <= 360){
  drehungen = drehungen + 1;
  winkelalt = analogRead(A3);
  }
}



if (Verschluss1 == 0) { 
    myservo.write(161);              
    delay(15);                      
  }


if (Verschluss == 0) {
    myservo.write(133+((1.8-(blende/10))*(-14)));             
    delay(verschluss);       
    myservo.write(161);           
    delay(5000);
    myservo.write(133);
    delay(100);                
  }



  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println(100000*pow((blende/10),2)/((iso)*(pow(log10(lux)/log10(2),2))));
  display.setTextSize(2);
  display.println(verschluss);
  display.println(blende/10); 
  display.setTextSize(1);
  display.println(iso);
  display.setCursor(100, 10);
  display.setTextSize(2);
  display.println(36-(drehungen));
  display.display(); 
  delay(10); 
  display.clearDisplay();

}
