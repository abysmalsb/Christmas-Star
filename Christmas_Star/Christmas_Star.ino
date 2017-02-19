/**************************************************************
 * This code controls a star shaped DIY display using an
 * Arduino MKR1000 and Blynk. More info:
 * https://www.hackster.io/Abysmal/iot-christmas-tree-topper-7ad7e8
 * 
 * This project is made for "The Arduino Internet of Holiday Things"
 * contest on hackster.io. More info:
 * https://www.hackster.io/contests/arduino-holiday-contest
 * 
 * author: Balázs Simon
 * e-mail: simon.balazs.1992@gmail.com
 *
 **************************************************************/

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


#define P4 2    //D4
#define P3 14   //D5
#define P2 12   //D6
#define P1 13   //D7
#define N4 16   //D0
#define N3 5    //D1
#define N2 4    //D2
#define N1 0    //D3
#define PLOT_LENGTH 256

char auth[] = "your auth";
char ssid[] = "your ssid";
char password[] = "your password";

int lightingMode = 1;
int changeSpeed = 1;

char plot[16][PLOT_LENGTH];
int currentStep = 0;

int i;
int j;

unsigned long lastLEDUpdateTime = 0;

void setup() {
  Blynk.begin(auth, ssid, password);

  pinMode(P1, OUTPUT);
  pinMode(P2, OUTPUT);
  pinMode(P3, OUTPUT);
  pinMode(P4, OUTPUT);
  pinMode(N1, OUTPUT);
  pinMode(N2, OUTPUT);
  pinMode(N3, OUTPUT);
  pinMode(N4, OUTPUT);

  for (i = 0; i < PLOT_LENGTH; i++) {
    for (j = 0; j < 16; j++) {
      plot[j][i] = 255;
    }
  }
}

void loop() {

  //Star screen updating based on the "plot"
  for (i = 0; i < 256; i++) {
    j = 0;
    digitalWrite(N4, HIGH);
    plot[j++][currentStep] > i ? digitalWrite(P1, HIGH) : digitalWrite(P1, LOW);
    plot[j++][currentStep] > i ? digitalWrite(P2, HIGH) : digitalWrite(P2, LOW);
    plot[j++][currentStep] > i ? digitalWrite(P3, HIGH) : digitalWrite(P3, LOW);
    plot[j++][currentStep] > i ? digitalWrite(P4, HIGH) : digitalWrite(P4, LOW);
    digitalWrite(N1, LOW);

    delayMicroseconds(20);

    digitalWrite(N1, HIGH);
    plot[j++][currentStep] > i ? digitalWrite(P1, HIGH) : digitalWrite(P1, LOW);
    plot[j++][currentStep] > i ? digitalWrite(P2, HIGH) : digitalWrite(P2, LOW);
    plot[j++][currentStep] > i ? digitalWrite(P3, HIGH) : digitalWrite(P3, LOW);
    plot[j++][currentStep] > i ? digitalWrite(P4, HIGH) : digitalWrite(P4, LOW);
    digitalWrite(N2, LOW);

    delayMicroseconds(20);

    digitalWrite(N2, HIGH);
    plot[j++][currentStep] > i ? digitalWrite(P1, HIGH) : digitalWrite(P1, LOW);
    plot[j++][currentStep] > i ? digitalWrite(P2, HIGH) : digitalWrite(P2, LOW);
    plot[j++][currentStep] > i ? digitalWrite(P3, HIGH) : digitalWrite(P3, LOW);
    plot[j++][currentStep] > i ? digitalWrite(P4, HIGH) : digitalWrite(P4, LOW);
    digitalWrite(N3, LOW);

    delayMicroseconds(20);

    digitalWrite(N3, HIGH);
    plot[j++][currentStep] > i ? digitalWrite(P1, HIGH) : digitalWrite(P1, LOW);
    plot[j++][currentStep] > i ? digitalWrite(P2, HIGH) : digitalWrite(P2, LOW);
    plot[j++][currentStep] > i ? digitalWrite(P3, HIGH) : digitalWrite(P3, LOW);
    plot[j++][currentStep] > i ? digitalWrite(P4, HIGH) : digitalWrite(P4, LOW);
    digitalWrite(N4, LOW);

    delayMicroseconds(20);
  }

  if (millis() - lastLEDUpdateTime > changeSpeed) {
    
    //Moving to the next image in the plot
    
    lastLEDUpdateTime = millis();
    currentStep++;

    if (currentStep == PLOT_LENGTH) {
      currentStep = 0;
    }
  }
  Blynk.run();
}

//Speed slider
BLYNK_WRITE(V1)
{
  changeSpeed = param.asInt();
}

//Creating "Simple on" plot
BLYNK_WRITE(V2)
{
  if (lightingMode != 1) {
    for (i = 0; i < PLOT_LENGTH; i++) {
      for (j = 0; j < 16; j++) {
        plot[j][i] = 255;
      }
    }

    lightingMode = 1;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}

//Creating "Flashing" plot
BLYNK_WRITE(V3)
{
  if (lightingMode != 2) {
    for (i = 0; i < PLOT_LENGTH / 2; i++) {
      for (j = 0; j < 16; j++) {
        plot[j][i] = 255;
      }
    }

    for (i = PLOT_LENGTH / 2; i < PLOT_LENGTH; i++) {
      for (j = 0; j < 16; j++) {
        plot[j][i] = 0;
      }
    }

    lightingMode = 2;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}

//Creating "Fading" plot
BLYNK_WRITE(V4)
{
  if (lightingMode != 3) {
    for (i = 0; i < PLOT_LENGTH; i++) {
      for (j = 0; j < 16; j++) {
        if (i == 0) {
          plot[j][i] = 255;
        }
        else if (i * 2 < PLOT_LENGTH) {
          plot[j][i] = PLOT_LENGTH - 2 * i;
        }
        else {
          plot[j][i] = 2 * i - PLOT_LENGTH;
        }
      }
    }
    lightingMode = 3;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}

//Creating "Off" plot
BLYNK_WRITE(V5)
{
  if (lightingMode != 4) {
    for (i = 0; i < PLOT_LENGTH; i++) {
      for (j = 0; j < 16; j++) {
        plot[j][i] = 0;
      }
    }
    lightingMode = 4;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}

//Creating "Running in" plot
BLYNK_WRITE(V6)
{
  if (lightingMode != 5) {
    for (i = 0; i < PLOT_LENGTH; i++) {
      if (i < PLOT_LENGTH / 2) {
        plot[0][i] = 255;
        plot[2][i] = 255;
        plot[7][i] = 255;
        plot[8][i] = 255;
        plot[11][i] = 255;
      }
      else {
        plot[0][i] = 0;
        plot[2][i] = 0;
        plot[7][i] = 0;
        plot[8][i] = 0;
        plot[11][i] = 0;
      }

      if (i - 32 < PLOT_LENGTH / 2 && i >= 32) {
        plot[3][i] = 255;
        plot[4][i] = 255;
        plot[6][i] = 255;
        plot[12][i] = 255;
        plot[15][i] = 255;
      }
      else {
        plot[3][i] = 0;
        plot[4][i] = 0;
        plot[6][i] = 0;
        plot[12][i] = 0;
        plot[15][i] = 0;
      }
      
      if (i - 64 < PLOT_LENGTH / 2 && i >= 64) {
        plot[1][i] = 255;
        plot[5][i] = 255;
        plot[9][i] = 255;
        plot[10][i] = 255;
        plot[13][i] = 255;
      }
      else {
        plot[1][i] = 0;
        plot[5][i] = 0;
        plot[9][i] = 0;
        plot[10][i] = 0;
        plot[13][i] = 0;
      }

      if (i - 96 < PLOT_LENGTH / 2 && i >= 96) {
        plot[14][i] = 255;
      }
      else {
        plot[14][i] = 0;
      }
    }
    lightingMode = 5;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}

//Creating "Running out" plot
BLYNK_WRITE(V7)
{
  if (lightingMode != 6) {
    for (i = 0; i < PLOT_LENGTH; i++) {
      if (i < PLOT_LENGTH / 2) {
        plot[14][i] = 255;
      }
      else {
        plot[14][i] = 0;
      }

      if (i - 32 < PLOT_LENGTH / 2 && i >= 32) {
        plot[1][i] = 255;
        plot[5][i] = 255;
        plot[9][i] = 255;
        plot[10][i] = 255;
        plot[13][i] = 255;
      }
      else {
        plot[1][i] = 0;
        plot[5][i] = 0;
        plot[9][i] = 0;
        plot[10][i] = 0;
        plot[13][i] = 0;
      }

      if (i - 64 < PLOT_LENGTH / 2 && i >= 64) {
        plot[3][i] = 255;
        plot[4][i] = 255;
        plot[6][i] = 255;
        plot[12][i] = 255;
        plot[15][i] = 255;
      }
      else {
        plot[3][i] = 0;
        plot[4][i] = 0;
        plot[6][i] = 0;
        plot[12][i] = 0;
        plot[15][i] = 0;
      }

      if (i - 96 < PLOT_LENGTH / 2 && i >= 96) {
        plot[0][i] = 255;
        plot[2][i] = 255;
        plot[7][i] = 255;
        plot[8][i] = 255;
        plot[11][i] = 255;
      }
      else {
        plot[0][i] = 0;
        plot[2][i] = 0;
        plot[7][i] = 0;
        plot[8][i] = 0;
        plot[11][i] = 0;
      }
    }
    lightingMode = 6;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}

//Creating "Fading in" plot
BLYNK_WRITE(V8)
{
  if (lightingMode != 7) {
    for (i = 0; i < PLOT_LENGTH; i++) {
      if (i < PLOT_LENGTH / 4) {
        plot[0][i] = 4 * i;
        plot[2][i] = 4 * i;
        plot[7][i] = 4 * i;
        plot[8][i] = 4 * i;
        plot[11][i] = 4 * i;
      }
      else if (i - 64 < PLOT_LENGTH / 4) {
        plot[0][i] = 255 - 4 * (i - 64);
        plot[2][i] = 255 - 4 * (i - 64);
        plot[7][i] = 255 - 4 * (i - 64);
        plot[8][i] = 255 - 4 * (i - 64);
        plot[11][i] = 255 - 4 * (i - 64);
      }
      else {
        plot[0][i] = 0;
        plot[2][i] = 0;
        plot[7][i] = 0;
        plot[8][i] = 0;
        plot[11][i] = 0;
      }

      if (i - 32 < PLOT_LENGTH / 4 && i >= 32) {
        plot[3][i] = 4 * (i - 32);
        plot[4][i] = 4 * (i - 32);
        plot[6][i] = 4 * (i - 32);
        plot[12][i] = 4 * (i - 32);
        plot[15][i] = 4 * (i - 32);
      }
      else if (i - 96 < PLOT_LENGTH / 4 && i >= 32) {
        plot[3][i] = 255 - 4 * (i - 96);
        plot[4][i] = 255 - 4 * (i - 96);
        plot[6][i] = 255 - 4 * (i - 96);
        plot[12][i] = 255 - 4 * (i - 96);
        plot[15][i] = 255 - 4 * (i - 96);
      }
      else {
        plot[3][i] = 0;
        plot[4][i] = 0;
        plot[6][i] = 0;
        plot[12][i] = 0;
        plot[15][i] = 0;
      }

      if (i - 64 < PLOT_LENGTH / 4 && i > 64) {
        plot[1][i] = 4 * (i - 64);
        plot[5][i] = 4 * (i - 64);
        plot[9][i] = 4 * (i - 64);
        plot[10][i] = 4 * (i - 64);
        plot[13][i] = 4 * (i - 64);
      }
      else if (i - 128 < PLOT_LENGTH / 4 && i > 64) {
        plot[1][i] = 255 - 4 * (i - 128);
        plot[5][i] = 255 - 4 * (i - 128);
        plot[9][i] = 255 - 4 * (i - 128);
        plot[10][i] = 255 - 4 * (i - 128);
        plot[13][i] = 255 - 4 * (i - 128);
      }
      else {
        plot[1][i] = 0;
        plot[5][i] = 0;
        plot[9][i] = 0;
        plot[10][i] = 0;
        plot[13][i] = 0;
      }

      if (i - 96 < PLOT_LENGTH / 4 && i >= 96) {
        plot[14][i] = 4 * (i - 96);
      }
      else if (i - 160 < PLOT_LENGTH / 4 && i >= 96) {
        plot[14][i] = 255 - 4 * (i - 160);
      }
      else {
        plot[14][i] = 0;
      }
    }
    lightingMode = 7;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}

//Creating "Fading out" plot
BLYNK_WRITE(V9)
{
  if (lightingMode != 8) {
    for (i = 0; i < PLOT_LENGTH; i++) {
      if (i < PLOT_LENGTH / 4) {
        plot[14][i] = 4 * i;
      }
      else if (i - 64 < PLOT_LENGTH / 4) {
        plot[14][i] = 255 - 4 * (i - 64);
      }
      else {
        plot[14][i] = 0;
      }

      if (i - 32 < PLOT_LENGTH / 4 && i >= 32) {
        plot[1][i] = 4 * (i - 32);
        plot[5][i] = 4 * (i - 32);
        plot[9][i] = 4 * (i - 32);
        plot[10][i] = 4 * (i - 32);
        plot[13][i] = 4 * (i - 32);
      }
      else if (i - 96 < PLOT_LENGTH / 4 && i >= 32) {
        plot[1][i] = 255 - 4 * (i - 96);
        plot[5][i] = 255 - 4 * (i - 96);
        plot[9][i] = 255 - 4 * (i - 96);
        plot[10][i] = 255 - 4 * (i - 96);
        plot[13][i] = 255 - 4 * (i - 96);
      }
      else {
        plot[1][i] = 0;
        plot[5][i] = 0;
        plot[9][i] = 0;
        plot[10][i] = 0;
        plot[13][i] = 0;
      }

      if (i - 64 < PLOT_LENGTH / 4 && i >= 64) {
        plot[3][i] = 4 * (i - 64);
        plot[4][i] = 4 * (i - 64);
        plot[6][i] = 4 * (i - 64);
        plot[12][i] = 4 * (i - 64);
        plot[15][i] = 4 * (i - 64);
      }
      else if (i - 128 < PLOT_LENGTH / 4 && i >= 64) {
        plot[3][i] = 255 - 4 * (i - 128);
        plot[4][i] = 255 - 4 * (i - 128);
        plot[6][i] = 255 - 4 * (i - 128);
        plot[12][i] = 255 - 4 * (i - 128);
        plot[15][i] = 255 - 4 * (i - 128);
      }
      else {
        plot[3][i] = 0;
        plot[4][i] = 0;
        plot[6][i] = 0;
        plot[12][i] = 0;
        plot[15][i] = 0;
      }

      if (i - 96 < PLOT_LENGTH / 4 && i >= 96) {
        plot[0][i] = 4 * (i - 96);
        plot[2][i] = 4 * (i - 96);
        plot[7][i] = 4 * (i - 96);
        plot[8][i] = 4 * (i - 96);
        plot[11][i] = 4 * (i - 96);
      }
      else if (i - 160 < PLOT_LENGTH / 4 && i >= 96) {
        plot[0][i] = 255 - 4 * (i - 160);
        plot[2][i] = 255 - 4 * (i - 160);
        plot[7][i] = 255 - 4 * (i - 160);
        plot[8][i] = 255 - 4 * (i - 160);
        plot[11][i] = 255 - 4 * (i - 160);
      }
      else {
        plot[0][i] = 0;
        plot[2][i] = 0;
        plot[7][i] = 0;
        plot[8][i] = 0;
        plot[11][i] = 0;
      }
    }
    lightingMode = 8;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}

//Creating "Inverse fading in" plot
BLYNK_WRITE(V10)
{
  if (lightingMode != 9) {
    for (i = 0; i < PLOT_LENGTH; i++) {
      if (i < PLOT_LENGTH / 4) {
        plot[0][i] = 255 - 4 * i;
        plot[2][i] = 255 - 4 * i;
        plot[7][i] = 255 - 4 * i;
        plot[8][i] = 255 - 4 * i;
        plot[11][i] = 255 - 4 * i;
      }
      else if (i - 64 < PLOT_LENGTH / 4) {
        plot[0][i] = 4 * (i - 64);
        plot[2][i] = 4 * (i - 64);
        plot[7][i] = 4 * (i - 64);
        plot[8][i] = 4 * (i - 64);
        plot[11][i] = 4 * (i - 64);
      }
      else {
        plot[0][i] = 255;
        plot[2][i] = 255;
        plot[7][i] = 255;
        plot[8][i] = 255;
        plot[11][i] = 255;
      }

      if (i - 32 < PLOT_LENGTH / 4 && i >= 32) {
        plot[3][i] = 255 - 4 * (i - 32);
        plot[4][i] = 255 - 4 * (i - 32);
        plot[6][i] = 255 - 4 * (i - 32);
        plot[12][i] = 255 - 4 * (i - 32);
        plot[15][i] = 255 - 4 * (i - 32);
      }
      else if (i - 96 < PLOT_LENGTH / 4 && i >= 32) {
        plot[3][i] = 4 * (i - 96);
        plot[4][i] = 4 * (i - 96);
        plot[6][i] = 4 * (i - 96);
        plot[12][i] = 4 * (i - 96);
        plot[15][i] = 4 * (i - 96);
      }
      else {
        plot[3][i] = 255;
        plot[4][i] = 255;
        plot[6][i] = 255;
        plot[12][i] = 255;
        plot[15][i] = 255;
      }

      if (i - 64 < PLOT_LENGTH / 4 && i >= 64) {
        plot[1][i] = 255 - 4 * (i - 64);
        plot[5][i] = 255 - 4 * (i - 64);
        plot[9][i] = 255 - 4 * (i - 64);
        plot[10][i] = 255 - 4 * (i - 64);
        plot[13][i] = 255 - 4 * (i - 64);
      }
      else if (i - 128 < PLOT_LENGTH / 4 && i >= 64) {
        plot[1][i] = 4 * (i - 128);
        plot[5][i] = 4 * (i - 128);
        plot[9][i] = 4 * (i - 128);
        plot[10][i] = 4 * (i - 128);
        plot[13][i] = 4 * (i - 128);
      }
      else {
        plot[1][i] = 255;
        plot[5][i] = 255;
        plot[9][i] = 255;
        plot[10][i] = 255;
        plot[13][i] = 255;
      }

      if (i - 96 < PLOT_LENGTH / 4 && i >= 96) {
        plot[14][i] = 255 - 4 * (i - 96);
      }
      else if (i - 160 < PLOT_LENGTH / 4 && i >= 96) {
        plot[14][i] = 4 * (i - 160);
      }
      else {
        plot[14][i] = 255;
      }
    }
    lightingMode = 9;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}

//Creating "Inverse fading out" plot
BLYNK_WRITE(V11)
{
  if (lightingMode != 10) {
    for (i = 0; i < PLOT_LENGTH; i++) {
      if (i < PLOT_LENGTH / 4) {
        plot[14][i] = 255 - 4 * i;
      }
      else if (i - 64 < PLOT_LENGTH / 4) {
        plot[14][i] = 4 * (i - 64);
      }
      else {
        plot[14][i] = 255;
      }

      if (i - 32 < PLOT_LENGTH / 4 && i >= 32) {
        plot[1][i] = 255 - 4 * (i - 32);
        plot[5][i] = 255 - 4 * (i - 32);
        plot[9][i] = 255 - 4 * (i - 32);
        plot[10][i] = 255 - 4 * (i - 32);
        plot[13][i] = 255 - 4 * (i - 32);
      }
      else if (i - 96 < PLOT_LENGTH / 4 && i >= 32) {
        plot[1][i] = 4 * (i - 96);
        plot[5][i] = 4 * (i - 96);
        plot[9][i] = 4 * (i - 96);
        plot[10][i] = 4 * (i - 96);
        plot[13][i] = 4 * (i - 96);
      }
      else {
        plot[1][i] = 255;
        plot[5][i] = 255;
        plot[9][i] = 255;
        plot[10][i] = 255;
        plot[13][i] = 255;
      }

      if (i - 64 < PLOT_LENGTH / 4 && i >= 64) {
        plot[3][i] = 255 - 4 * (i - 64);
        plot[4][i] = 255 - 4 * (i - 64);
        plot[6][i] = 255 - 4 * (i - 64);
        plot[12][i] = 255 - 4 * (i - 64);
        plot[15][i] = 255 - 4 * (i - 64);
      }
      else if (i - 128 < PLOT_LENGTH / 4 && i >= 64) {
        plot[3][i] = 4 * (i - 128);
        plot[4][i] = 4 * (i - 128);
        plot[6][i] = 4 * (i - 128);
        plot[12][i] = 4 * (i - 128);
        plot[15][i] = 4 * (i - 128);
      }
      else {
        plot[3][i] = 255;
        plot[4][i] = 255;
        plot[6][i] = 255;
        plot[12][i] = 255;
        plot[15][i] = 255;
      }

      if (i - 96 < PLOT_LENGTH / 4 && i >= 96) {
        plot[0][i] = 255 - 4 * (i - 96);
        plot[2][i] = 255 - 4 * (i - 96);
        plot[7][i] = 255 - 4 * (i - 96);
        plot[8][i] = 255 - 4 * (i - 96);
        plot[11][i] = 255 - 4 * (i - 96);
      }
      else if (i - 160 < PLOT_LENGTH / 4 && i >= 96) {
        plot[0][i] = 4 * (i - 160);
        plot[2][i] = 4 * (i - 160);
        plot[7][i] = 4 * (i - 160);
        plot[8][i] = 4 * (i - 160);
        plot[11][i] = 4 * (i - 160);
      }
      else {
        plot[0][i] = 255;
        plot[2][i] = 255;
        plot[7][i] = 255;
        plot[8][i] = 255;
        plot[11][i] = 255;
      }
    }
    lightingMode = 10;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}

//Creating "Run around CW" plot
BLYNK_WRITE(V12)
{
  if (lightingMode != 11) {
    for (i = 0; i < PLOT_LENGTH; i++) {
      for (j = 0; j < 16; j++) {
        plot[j][i] = 0;
      }

      plot[11][i] = 255;
      plot[8][i] = 255;
      plot[7][i] = 255;
      plot[0][i] = 255;
      plot[2][i] = 255;
      plot[14][i] = 255;

      if (i < 25) {
        plot[6][i] = 255;
        plot[10][i] = 255;
      }
      else if (i >= 25 && i < 51) {
        plot[1][i] = 255;
        plot[15][i] = 255;
      }
      else if (i >= 51 && i < 76) {
        plot[4][i] = 255;
        plot[13][i] = 255;
      }
      else if (i >= 76 && i < 102) {
        plot[3][i] = 255;
        plot[9][i] = 255;
      }
      else if (i >= 102 && i < 127) {
        plot[5][i] = 255;
        plot[12][i] = 255;
      }
      else if (i >= 127 && i < 153) {
        plot[6][i] = 255;
        plot[10][i] = 255;
      }
      else if (i >= 153 && i < 178) {
        plot[1][i] = 255;
        plot[15][i] = 255;
      }
      else if (i >= 178 && i < 204) {
        plot[4][i] = 255;
        plot[13][i] = 255;
      }
      else if (i >= 204 && i <= 229) {
        plot[3][i] = 255;
        plot[9][i] = 255;
      }
      else if (i >= 229) {
        plot[5][i] = 255;
        plot[12][i] = 255;
      }
    }
    lightingMode = 11;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}

//Creating "Run around CCW" plot
BLYNK_WRITE(V13)
{
  if (lightingMode != 12) {
    for (i = 0; i < PLOT_LENGTH; i++) {
      for (j = 0; j < 16; j++) {
        plot[j][i] = 0;
      }

      plot[11][i] = 255;
      plot[8][i] = 255;
      plot[7][i] = 255;
      plot[0][i] = 255;
      plot[2][i] = 255;
      plot[14][i] = 255;

      if (i < 25) {
        plot[5][i] = 255;
        plot[12][i] = 255;
      }
      else if (i >= 25 && i < 51) {
        plot[3][i] = 255;
        plot[9][i] = 255;
      }
      else if (i >= 51 && i < 76) {
        plot[4][i] = 255;
        plot[13][i] = 255;
      }
      else if (i >= 76 && i < 102) {
        plot[1][i] = 255;
        plot[15][i] = 255;
      }
      else if (i >= 102 && i < 127) {
        plot[6][i] = 255;
        plot[10][i] = 255;
      }
      else if (i >= 127 && i < 153) {
        plot[5][i] = 255;
        plot[12][i] = 255;
      }
      else if (i >= 153 && i < 178) {
        plot[3][i] = 255;
        plot[9][i] = 255;
      }
      else if (i >= 178 && i < 204) {
        plot[4][i] = 255;
        plot[13][i] = 255;
      }
      else if (i >= 204 && i <= 229) {
        plot[1][i] = 255;
        plot[15][i] = 255;
      }
      else if (i >= 229) {
        plot[6][i] = 255;
        plot[10][i] = 255;
      }
    }
    lightingMode = 12;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}

//Creating "Fade around CW 1" plot
BLYNK_WRITE(V14)
{
  if (lightingMode != 13) {
    for (i = 0; i < PLOT_LENGTH; i++) {
      for (j = 0; j < 16; j++) {
        plot[j][i] = 0;
      }

      plot[14][i] = 255;

      if (i < 25) {
        plot[10][i] = 255 - i * 10;
        plot[15][i] = i * 10;
        plot[11][i] = plot[15][i];
        plot[1][i] = plot[15][i];
      }
      else if (i >= 25 && i < 51) {
        plot[15][i] = 255 - (i - 25) * 10;
        plot[11][i] = plot[15][i];
        plot[1][i] = plot[15][i];
        plot[13][i] = (i - 25) * 10;
      }
      else if (i >= 51 && i < 76) {
        plot[13][i] = 255 - (i - 51) * 10;
        plot[9][i] = (i - 51) * 10;
        plot[7][i] = plot[9][i];
        plot[3][i] = plot[9][i];
      }
      else if (i >= 76 && i < 102) {
        plot[9][i] = 255 - (i - 76) * 10;
        plot[7][i] = plot[9][i];
        plot[3][i] = plot[9][i];
        plot[5][i] = (i - 76) * 10;
      }
      else if (i >= 102 && i < 127) {
        plot[5][i] = 255 - (i - 102) * 10;
        plot[10][i] = (i - 102) * 10;
        plot[6][i] = plot[10][i];
        plot[2][i] = plot[10][i];
      }
      else if (i >= 127 && i < 153) {
        plot[10][i] = 255 - (i - 127) * 10;
        plot[6][i] = plot[10][i];
        plot[2][i] = plot[10][i];
        plot[1][i] = (i - 127) * 10;
      }
      else if (i >= 153 && i < 178) {
        plot[1][i] = 255 - (i - 153) * 10;
        plot[13][i] = (i - 153) * 10;
        plot[4][i] = plot[13][i];
        plot[0][i] = plot[13][i];
      }
      else if (i >= 178 && i < 204) {
        plot[13][i] = 255 - (i - 178) * 10;
        plot[4][i] = plot[13][i];
        plot[9][i] = plot[13][i];
        plot[9][i] = (i - 178) * 10;
      }
      else if (i >= 204 && i <= 229) {
        plot[9][i] = 255 - (i - 204) * 10;
        plot[12][i] = (i - 204) * 10;
        plot[8][i] = plot[12][i];
        plot[5][i] = plot[12][i];
      }
      else if (i >= 229 && i < 255) {
        plot[12][i] = 255 - (i - 229) * 10;
        plot[8][i] = plot[12][i];
        plot[5][i] = plot[12][i];
        plot[10][i] = (i - 229) * 10;
      }
      else{
        plot[12][i] = 0;
        plot[8][i] = plot[1][i];
        plot[5][i] = plot[1][i];
        plot[10][i] = 255;
      }
    }
    lightingMode = 13;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}

//Creating "Fade around CCW 1" plot
BLYNK_WRITE(V15)
{
  if (lightingMode != 14) {
    for (i = 0; i < PLOT_LENGTH; i++) {
      for (j = 0; j < 16; j++) {
        plot[j][i] = 0;
      }

      plot[14][i] = 255;

      if (i < 25) {
        plot[10][i] = 255 - i * 10;
        plot[5][i] = i * 10;
        plot[8][i] = plot[5][i];
        plot[12][i] = plot[5][i];
      }
      else if (i >= 25 && i < 51) {
        plot[5][i] = 255 - (i - 25) * 10;
        plot[8][i] = plot[5][i];
        plot[12][i] = plot[5][i];
        plot[9][i] = (i - 25) * 10;
      }
      else if (i >= 51 && i < 76) {
        plot[9][i] = 255 - (i - 51) * 10;
        plot[0][i] = (i - 51) * 10;
        plot[4][i] = plot[0][i];
        plot[13][i] = plot[0][i];
      }
      else if (i >= 76 && i < 102) {
        plot[0][i] = 255 - (i - 76) * 10;
        plot[4][i] = plot[0][i];
        plot[13][i] = plot[0][i];
        plot[1][i] = (i - 76) * 10;
      }
      else if (i >= 102 && i < 127) {
        plot[1][i] = 255 - (i - 102) * 10;
        plot[2][i] = (i - 102) * 10;
        plot[6][i] = plot[2][i];
        plot[10][i] = plot[2][i];
      }
      else if (i >= 127 && i < 153) {
        plot[2][i] = 255 - (i - 127) * 10;
        plot[6][i] = plot[2][i];
        plot[10][i] = plot[2][i];
        plot[5][i] = (i - 127) * 10;
      }
      else if (i >= 153 && i < 178) {
        plot[5][i] = 255 - (i - 153) * 10;
        plot[3][i] = (i - 153) * 10;
        plot[7][i] = plot[3][i];
        plot[9][i] = plot[3][i];
      }
      else if (i >= 178 && i < 204) {
        plot[3][i] = 255 - (i - 178) * 10;
        plot[7][i] = plot[3][i];
        plot[9][i] = plot[3][i];
        plot[13][i] = (i - 178) * 10;
      }
      else if (i >= 204 && i <= 229) {
        plot[13][i] = 255 - (i - 204) * 10;
        plot[1][i] = (i - 204) * 10;
        plot[11][i] = plot[1][i];
        plot[15][i] = plot[1][i];
      }
      else if (i >= 229 && i < 255) {
        plot[1][i] = 255 - (i - 229) * 10;
        plot[11][i] = plot[1][i];
        plot[15][i] = plot[1][i];
        plot[10][i] = (i - 229) * 10;
      }
      else{
        plot[1][i] = 0;
        plot[11][i] = plot[1][i];
        plot[15][i] = plot[1][i];
        plot[10][i] = 255;
      }
    }
    lightingMode = 14;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}

//Creating "Fade around CW 2" plot
BLYNK_WRITE(V16)
{
  if (lightingMode != 15) {
    for (i = 0; i < PLOT_LENGTH; i++) {
      for (j = 0; j < 16; j++) {
        plot[j][i] = 255;
      }

      plot[14][i] = 0;

      if (i < 25) {
        plot[10][i] = i * 10;
        plot[15][i] = 255 - i * 10;
        plot[11][i] = plot[15][i];
        plot[1][i] = plot[15][i];
      }
      else if (i >= 25 && i < 51) {
        plot[15][i] = (i - 25) * 10;
        plot[11][i] = plot[15][i];
        plot[1][i] = plot[15][i];
        plot[13][i] = 255 -  (i - 25) * 10;
      }
      else if (i >= 51 && i < 76) {
        plot[13][i] = (i - 51) * 10;
        plot[9][i] = 255 - (i - 51) * 10;
        plot[7][i] = plot[9][i];
        plot[3][i] = plot[9][i];
      }
      else if (i >= 76 && i < 102) {
        plot[9][i] = (i - 76) * 10;
        plot[7][i] = plot[9][i];
        plot[3][i] = plot[9][i];
        plot[5][i] = 255 - (i - 76) * 10;
      }
      else if (i >= 102 && i < 127) {
        plot[5][i] = (i - 102) * 10;
        plot[10][i] = 255 - (i - 102) * 10;
        plot[6][i] = plot[10][i];
        plot[2][i] = plot[10][i];
      }
      else if (i >= 127 && i < 153) {
        plot[10][i] = (i - 127) * 10;
        plot[6][i] = plot[10][i];
        plot[2][i] = plot[10][i];
        plot[1][i] = 255 - (i - 127) * 10;
      }
      else if (i >= 153 && i < 178) {
        plot[1][i] = (i - 153) * 10;
        plot[13][i] = 255 - (i - 153) * 10;
        plot[4][i] = plot[13][i];
        plot[0][i] = plot[13][i];
      }
      else if (i >= 178 && i < 204) {
        plot[13][i] = (i - 178) * 10;
        plot[4][i] = plot[13][i];
        plot[9][i] = plot[13][i];
        plot[9][i] = 255 - (i - 178) * 10;
      }
      else if (i >= 204 && i <= 229) {
        plot[9][i] = (i - 204) * 10;
        plot[12][i] = 255 - (i - 204) * 10;
        plot[8][i] = plot[12][i];
        plot[5][i] = plot[12][i];
      }
      else if (i >= 229 && i < 255) {
        plot[12][i] = (i - 229) * 10;
        plot[8][i] = plot[12][i];
        plot[5][i] = plot[12][i];
        plot[10][i] = 255 - (i - 229) * 10;
      }
      else{
        plot[12][i] = 255;
        plot[8][i] = plot[1][i];
        plot[5][i] = plot[1][i];
        plot[10][i] = 0;
      }
    }
    lightingMode = 15;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}

//Creating "Fade around CCW 2" plot
BLYNK_WRITE(V17)
{
  if (lightingMode != 16) {
    for (i = 0; i < PLOT_LENGTH; i++) {
      for (j = 0; j < 16; j++) {
        plot[j][i] = 255;
      }

      plot[14][i] = 0;

      if (i < 25) {
        plot[10][i] = i * 10;
        plot[5][i] = 255 - i * 10;
        plot[8][i] = plot[5][i];
        plot[12][i] = plot[5][i];
      }
      else if (i >= 25 && i < 51) {
        plot[5][i] = (i - 25) * 10;
        plot[8][i] = plot[5][i];
        plot[12][i] = plot[5][i];
        plot[9][i] = 255 - (i - 25) * 10;
      }
      else if (i >= 51 && i < 76) {
        plot[9][i] = (i - 51) * 10;
        plot[0][i] = 255 - (i - 51) * 10;
        plot[4][i] = plot[0][i];
        plot[13][i] = plot[0][i];
      }
      else if (i >= 76 && i < 102) {
        plot[0][i] = (i - 76) * 10;
        plot[4][i] = plot[0][i];
        plot[13][i] = plot[0][i];
        plot[1][i] = 255 - (i - 76) * 10;
      }
      else if (i >= 102 && i < 127) {
        plot[1][i] = (i - 102) * 10;
        plot[2][i] = 255 - (i - 102) * 10;
        plot[6][i] = plot[2][i];
        plot[10][i] = plot[2][i];
      }
      else if (i >= 127 && i < 153) {
        plot[2][i] = (i - 127) * 10;
        plot[6][i] = plot[2][i];
        plot[10][i] = plot[2][i];
        plot[5][i] = 255 - (i - 127) * 10;
      }
      else if (i >= 153 && i < 178) {
        plot[5][i] = (i - 153) * 10;
        plot[3][i] = 255 - (i - 153) * 10;
        plot[7][i] = plot[3][i];
        plot[9][i] = plot[3][i];
      }
      else if (i >= 178 && i < 204) {
        plot[3][i] = (i - 178) * 10;
        plot[7][i] = plot[3][i];
        plot[9][i] = plot[3][i];
        plot[13][i] = 255 - (i - 178) * 10;
      }
      else if (i >= 204 && i <= 229) {
        plot[13][i] = (i - 204) * 10;
        plot[1][i] = 255 - (i - 204) * 10;
        plot[11][i] = plot[1][i];
        plot[15][i] = plot[1][i];
      }
      else if (i >= 229 && i < 255) {
        plot[1][i] = (i - 229) * 10;
        plot[11][i] = plot[1][i];
        plot[15][i] = plot[1][i];
        plot[10][i] = 255 - (i - 229) * 10;
      }
      else{
        plot[1][i] = 255;
        plot[11][i] = plot[1][i];
        plot[15][i] = plot[1][i];
        plot[10][i] = 0;
      }
    }
    lightingMode = 16;
    Blynk.virtualWrite(V1, changeSpeed);
    Blynk.virtualWrite(V0, lightingMode);
  }
}
