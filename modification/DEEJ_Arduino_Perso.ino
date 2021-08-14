//Require "HID-Project" librarie available into the libraries manager
#include "HID-Project.h"

#define breakoutPin 2

#define next_button 6
#define previous_button 7
#define pp_button 5

#define mute_button 8
#define led_mute 3
boolean mute_state = false;

const int NUM_SLIDERS = 5;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3, A6};

int analogSliderValues[NUM_SLIDERS];

void setup() {
  pinMode(breakoutPin, INPUT_PULLUP);
  if (digitalRead(breakoutPin) == 1) {
    while(1) {}                             // STOPPER LE PROGRAMME
  }

  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }

  pinMode(next_button, INPUT_PULLUP);
  pinMode(previous_button, INPUT_PULLUP);
  pinMode(mute_button, INPUT_PULLUP);
  pinMode(pp_button, INPUT_PULLUP);
  pinMode(led_mute, OUTPUT);

  Consumer.begin();
  BootKeyboard.begin();

  Serial.begin(9600);
}

void loop() {
  check_media_button();
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  // printSliderValues(); // For debug
  delay(10);
  

}


void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}

void check_media_button() {
  //NEXT TRACK
  if (digitalRead(next_button) == 0) {
    Consumer.write(MEDIA_NEXT);
    delay(500);
  }

  //PREVIOUS TRACK
  if (digitalRead(previous_button) == 0) {
    Consumer.write(MEDIA_PREVIOUS);
    delay(500);
  }

  //MUTE
  if (digitalRead(mute_button) == 0) {
    mute_state = !mute_state;
    Consumer.write(MEDIA_VOLUME_MUTE);
    digitalWrite(led_mute, mute_state);
    delay(500);
  }

  //PLAY/PAUSE
  if (digitalRead(pp_button) == 0) {
    Consumer.write(MEDIA_PLAY_PAUSE);
    delay(500);
  }
}