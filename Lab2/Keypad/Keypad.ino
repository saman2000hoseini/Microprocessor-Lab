#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {49, 50, 51, 52}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {24, 23, 22}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A7, OUTPUT);
  pinMode(A6, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A8, OUTPUT);
  Serial2.begin(9600);
}
  
void loop(){
  char key = keypad.getKey();
  
  if (key){
    Serial2.write(key);
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A3, LOW);
    digitalWrite(A4, LOW);
    digitalWrite(A5, LOW);
    digitalWrite(A6, LOW);
    digitalWrite(A7, LOW);
    digitalWrite(A8, LOW);
    
    switch(key){
      case '1':
        digitalWrite(A0, HIGH);
        break;
      case '2':
        digitalWrite(A1, HIGH);
        break;
      case '3':
        digitalWrite(A2, HIGH);
        break;
      case '4':
        digitalWrite(A3, HIGH);
        break;
      case '5':
        digitalWrite(A7, HIGH);
        break;
      case '6':
        digitalWrite(A6, HIGH);
        break;
      case '7':
        digitalWrite(A5, HIGH);
        break;
      case '8':
        digitalWrite(A4, HIGH);
        break;
      case '9':
        digitalWrite(A8, HIGH);
        break;
    }
  }
}
