#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
AudioSynthWaveform waveform1;  //xy=352.1999969482422,217.1999969482422
AudioOutputI2S i2s1;           //xy=527.1999969482422,215.1999969482422
AudioConnection patchCord1(waveform1, 0, i2s1, 0);
AudioConnection patchCord2(waveform1, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;  //xy=264.1999969482422,150.1999969482422
// GUItool: end automatically generated code
int buttons[6] = { 24, 25, 27, 29, 39, 40 };
int buttonState[6] = { LOW, LOW, LOW, LOW, LOW, LOW };
int lastButtonState[6] = { LOW, LOW, LOW, LOW, LOW, LOW };
int n;
int notes[5][7] = {
  { 0, 2, 4, 5, 7, 9, 11 },
  { 0, 2, 3, 5, 7, 8, 11 },
  { 0, 2, 4, 7, 9, 12, 14 },
  { 0, 2, 3, 4, 7, 9, 12 },
  { 0, 3, 5, 6, 7, 10, 12 },
};
int frequency;
int scale;
int mappedScale;
int scalePot = A17;
int rannote;
void setup() {
  Serial.begin(9600);
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
  scale = analogRead(scalePot);
  mappedScale = map(scale, 0, 1023, 0, 4);
}
void convert() {
  n = random(1, 88);
  rannote = random(0, 6);
  frequency = 440.0 * pow(2, (n + notes[mappedScale][rannote] - 49) / 12.0);
}
void play() {

  for (int i = 0; 1 < 6; i++) {
    buttonState[i] = digitalRead(buttons[i]);
    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      Serial.println(buttonState[i]);
      waveform1.begin(WAVEFORM_SINE);
      waveform1.frequency(frequency);
      lastButtonState[i] = HIGH;

      Serial.println(frequency);
    } else if (buttonState[i] == LOW && lastButtonState[i] == HIGH) {
      lastButtonState[i] = LOW;
      delay(5);
    }
  }
}
void loop() {
  findScale();
  convert();
  play();
}
