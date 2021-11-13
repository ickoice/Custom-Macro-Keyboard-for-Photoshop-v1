#include <Keyboard.h>
#include <EEPROM.h>

#define ENCODER_A 15
#define ENCODER_B 14
#define ENCODER_BUTTON A3

#define HORIZONTAL_MODE 0
#define VERTICAL_MODE 1
#define RESOLUTION 2

int mode = HORIZONTAL_MODE;

const int numMode = 2;
long tempCount = 0;

int encoderButtonState;
int encoderPreviousButtonState = 0;

bool aState;
bool aPreviousState;

const int numButtons = 8;
int buttonPins[numButtons] = {A2, A1, A0, 16, 10, 9, 8, 7};

const int DEBOUNCE_DELAY = 10;
int lastSteadyState[numButtons] = {LOW};
int lastFlickerableState[numButtons] = {LOW};
int buttonsCurrentState[numButtons] = {LOW};
unsigned long lastDebounceTime[numButtons] = {0};

unsigned long loopCount;
unsigned long startTime;
String msg;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  mode = EEPROM.read(0);

  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  pinMode(ENCODER_BUTTON, INPUT_PULLUP);

  for (int i = 0; i < numButtons; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  Keyboard.begin();

  aPreviousState = digitalRead(ENCODER_A);

  loopCount = 0;
  startTime = millis();
  msg = "";
}

void loop() {
  // put your main code here, to run repeatedly:
  loopCount++;
  if ( (millis() - startTime) > 5000 ) {
    startTime = millis();
    loopCount = 0;
  }

  handleEncoder();
  handleEncoderButton();
  handleButtons();
}


void handleButtons() {
  for(int i = 0; i < numButtons; i++) {
    buttonsCurrentState[i] = digitalRead(buttonPins[i]);

    if (buttonsCurrentState[i] != lastFlickerableState[i]) {
      lastDebounceTime[i] = millis();
      lastFlickerableState[i] = buttonsCurrentState[i];
    }

    if ((millis() - lastDebounceTime[i]) > DEBOUNCE_DELAY) {
      if (lastSteadyState[i] == HIGH && buttonsCurrentState[i] == LOW){
        pressButton(i, 0);
      } else if (lastSteadyState[i] == LOW && buttonsCurrentState[i] == HIGH){
        pressButton(i, 1);
      }
      lastSteadyState[i] = buttonsCurrentState[i];
    }
  }
}


void handleEncoder() {
  aState = digitalRead(ENCODER_A);
  if (aState != aPreviousState) {
    if (digitalRead(ENCODER_B) != aState) {
      rotateLeft();
      delay(8);
    } else {
      rotateRight();
      delay(8);
    }
    aPreviousState = aState;
  }
}


void handleEncoderButton() {
  if (digitalRead(ENCODER_BUTTON) == LOW){
    if(encoderPreviousButtonState == LOW){
      
    } else {
      delay(300);
    }
    encoderPreviousButtonState = LOW;
  } else {
    if (encoderPreviousButtonState == LOW){
      changeMode();
    } else {}
    encoderPreviousButtonState = HIGH;
  }
}


void changeMode() {

  mode = ++mode % numMode;
  EEPROM.write(0, mode);
  Serial.print("Button mode changed to ");
  Serial.println(mode);
}

void rotateLeft() {
  if(tempCount++ % RESOLUTION == 0){
    if(mode == HORIZONTAL_MODE){
      Keyboard.press('[');
    } else if (mode == VERTICAL_MODE){
      Keyboard.press(KEY_LEFT_GUI);
      //Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('-');
    }
    Keyboard.releaseAll();
  }
}

void rotateRight() {
  if(tempCount++ % RESOLUTION == 0){
    if(mode == HORIZONTAL_MODE){
      Keyboard.press(']');
    } else if (mode == VERTICAL_MODE){
      Keyboard.press(KEY_LEFT_GUI);
//      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('+');
    }
    Keyboard.releaseAll();
  }
}

void pressButton(int i, int state){
  if (state == 0) {
    switch (i) {
      case 0:
      // REDO - 2
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press(KEY_LEFT_SHIFT);
//        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('z');
        break;
      case 1:
      // UNDO - 1
        Keyboard.press(KEY_LEFT_GUI);
//        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('z');
        break;
      case 2:
      // MOVE TOOL - 3
//        Keyboard.press(KEY_LEFT_GUI);
//        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('v');
        break;
      case 3:
      // PAINT BUCKET TOOL - 6
//        Keyboard.press(KEY_LEFT_GUI);
//        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('g');
        break;
      case 4:
      // BRUSH TOOL - 4
//        Keyboard.press(KEY_LEFT_GUI);
//        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('b');
        break;
      case 5:
      // SPOT HEALING BRUSH TOOL - 7
//        Keyboard.press(KEY_LEFT_GUI);
//        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('j');
        break;
      case 6:
      // ERASER TOOL - 8
//        Keyboard.press(KEY_LEFT_GUI);
//        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('e');
        break;
      case 7:
      // RECTANGLE TOOL - 5
//        Keyboard.press(KEY_LEFT_GUI);
//        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('u');
        break;
      default:
        break;
    }
  } else {
    switch (i) {
      case 0:
      // REDO - 2
        Keyboard.release(KEY_LEFT_GUI);
        Keyboard.release(KEY_LEFT_SHIFT);
//        Keyboard.release(KEY_LEFT_GUI);
//        Keyboard.release(KEY_LEFT_CTRL);
        Keyboard.release('z');
        break;
      case 1:
      // UNDO - 1
        Keyboard.release(KEY_LEFT_GUI);
//        Keyboard.release(KEY_LEFT_CTRL);
        Keyboard.release('z');
        break;
      case 2:
      // MOVE TOOL - 3
//        Keyboard.release(KEY_LEFT_GUI);
//        Keyboard.release(KEY_LEFT_CTRL);
        Keyboard.release('v');
        break;
      case 3:
      // PAINT BUCKET TOOL - 6
//        Keyboard.release(KEY_LEFT_GUI);
//        Keyboard.release(KEY_LEFT_CTRL);
        Keyboard.release('g');
        break;
      case 4:
      // BRUSH TOOL - 4
//        Keyboard.release(KEY_LEFT_GUI);
//        Keyboard.release(KEY_LEFT_CTRL);
        Keyboard.release('b');
        break;
      case 5:
     // SPOT HEALING BRUSH TOOL - 7
//        Keyboard.release(KEY_LEFT_GUI);
//        Keyboard.release(KEY_LEFT_CTRL);
        Keyboard.release('j');
        break;
      case 6:
     // ERASER TOOL - 8
//        Keyboard.release(KEY_LEFT_GUI);
//        Keyboard.release(KEY_LEFT_CTRL);
        Keyboard.release('e');
        break;
      case 7:
      // RECTANGLE TOOL - 5
//        Keyboard.release(KEY_LEFT_GUI);
//        Keyboard.release(KEY_LEFT_CTRL);
        Keyboard.release('u');
        break;
      default:
        break;
    }
  }
}
