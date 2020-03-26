#include <Arduino.h>
#include <pitches.h>

int potPin = A0;

int b1 = 5;
int b2 = 6;
int b3 = 7;

int nextButton = 12;

#define numButtons 3
int buttons[] = {b1,b2,b3};

#define scaleLength 8
int CMajorScale[scaleLength] = {NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4};
int AMinorScale[scaleLength] = {NOTE_A3, NOTE_B3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4};
int DMajorScale[scaleLength] = {NOTE_D3, NOTE_E3, NOTE_FS3, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_CS4, NOTE_D4};
int FMajorScale[scaleLength] = {NOTE_F3, NOTE_G3, NOTE_A3, NOTE_AS3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4};

#define chordLength 3

int CMaj[chordLength] = {CMajorScale[0],CMajorScale[2], CMajorScale[4]};
int AMin[chordLength] = {AMinorScale[0],AMinorScale[2], AMinorScale[4]};


#define numChords 4

int chords[numChords][chordLength] = {{CMajorScale[0],CMajorScale[2], CMajorScale[4]},  //Cmaj 
                                      {AMinorScale[0],AMinorScale[2], AMinorScale[4]},  //Aminor
                                      {DMajorScale[0],DMajorScale[2], DMajorScale[4]},  //Dmajor
                                      {FMajorScale[0],FMajorScale[2], FMajorScale[4]}   //Fmajor
                                                                                     };

int speaker = 11;
int sensorValue;
int lastSensorValue;

void setup() {
  
  Serial.begin(9600);
}

int buttonPressed(){
  int pressed = -1;
  for(int i = 0; i < numButtons; i++){
    if(digitalRead(buttons[i])){
      pressed = i;
    }

  }
  return pressed;
}


unsigned long currentTime = 0;
unsigned long lastTime = 0;
int debounceDelay = 50;

bool buttonState = false;
bool lastButtonState = false;


bool nextPressed(){
  bool rv;
  currentTime = millis();
  buttonState = digitalRead(nextButton);


  if (currentTime > (lastTime + debounceDelay) && buttonState != lastButtonState && buttonState){
    rv = true;
  
  }else{
    rv = false;
  }

  if (buttonState){
    lastTime = currentTime;
  }
  lastButtonState = buttonState;

  return rv;
}



int chordIndex = 0;
#define PI 3.1415926535897932384626433832795
int waveFreq = 6;

void loop() {
  int sensorValue = analogRead(potPin);
  int waveFactor = map(sensorValue, 0, 1024, 0, 10);
  float wave = sin(((millis()/1000.0)*waveFreq)*2*PI);
  //Serial.println(sin((millis()/1000.0)*2*PI));
  //Serial.println(wave);


  int pressed = buttonPressed();

  

  if(nextPressed()){
    chordIndex++;
    if(chordIndex >= numChords){
      chordIndex = 0;
    }
    Serial.println(chordIndex);
  }
  
  if (pressed != -1){
    tone(speaker, chords[chordIndex][pressed]+wave*waveFactor,10);
  
  }
  
}