#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

AudioSynthWaveform waveform1;  //xy=352.1999969482422,217.1999969482422
AudioOutputI2S i2s1;           //xy=527.1999969482422,215.1999969482422
AudioConnection patchCord1(waveform1, 0, i2s1, 0);
AudioConnection patchCord2(waveform1, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;  //xy=264.1999969482422,150.1999969482422
// GUItool: end automatically generated code
int buttons[6] = { 26,27,28, 29, 39, 40 };
int buttonState[6] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };
int lastButtonState[6] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };
float n;
float temp;
float hum;
int notes[5][7] = {
  { 0, 2, 4, 5, 7, 9, 11 },
  { 0, 2, 3, 5, 7, 8, 11 },
  { 0, 2, 4, 7, 9, 12, 14 },
  { 0, 2, 3, 4, 7, 9, 12 },
  { 0, 3, 5, 6, 7, 10, 12 },
};
int frequency;
int scale;
int scalePot = A17;
int rannote;
void setup() {
  Serial.begin(9600);
  while (!Serial)
    delay(10); 
    Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  Serial.print("Heater Enabled State: ");
  if (sht31.isHeaterEnabled())
    Serial.println("ENABLED");
  else
    Serial.println("DISABLED");
  AudioMemory(12);  //always include this when using the Teensy Audio Library even if you don't know what it is yet!
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  waveform1.begin(WAVEFORM_SINE);
  waveform1.amplitude(0.2);  //amplitude (volume) can be 0 to 1 and this is usually a good volume for headphones
  for (int i = 0; i < 6; i++) {
    pinMode(buttons[i], INPUT);
  }
}


void findScale() {
  hum = sht31.readHumidity();
  
  if ((hum <30)){
    scale = 2;
  }else if ((hum >= 30) && (hum < 50)){
    scale = 0;
  }else if((hum >= 50) && (hum < 65)){
    scale = 3;
  }else if ((hum >= 65) && (hum < 80)){
    scale = 1;
  }else if ((hum >= 80)){
    scale = 4;
  }
  
}
void convert() {
  temp = sht31.readTemperature();
  n = map(temp, -10, 45, 88, 1);
  rannote = random(0, 7);
  frequency = 440.0 * pow(2, (n + notes[scale][rannote] - 49) / 12.0);
}
void play() {

  for (int i = 0; i < 6; i++) {
    lastButtonState[i]= buttonState[i];
    buttonState[i] = digitalRead(buttons[i]);
    if (buttonState[i] == LOW && lastButtonState[i] == HIGH) {
      waveform1.amplitude(0.2);
      waveform1.begin(WAVEFORM_SINE);
      waveform1.frequency(frequency);
      Serial.println(frequency);
      delay(300);
      waveform1.amplitude(0);
      Serial.println(hum);
      Serial.println(scale);
      
    } else if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      delay(5);
    }
  }
}
void loop() {
  
  findScale();
  convert();
  play();
}
