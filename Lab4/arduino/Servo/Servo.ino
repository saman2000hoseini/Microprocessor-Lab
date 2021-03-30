#include <Servo.h>
#include<Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'7','8','9', '/'},
  {'4','5','6', '*'},
  {'1','2','3', '-'},
  {'c','0','=', '+'}
};
byte rowPins[ROWS] = {50, 51, 52, 53}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {25, 24, 23, 22}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position
int potPin = A0;

void setup() {
  myservo.attach(2, 1000, 2000);  // attaches the servo on pin 9 to the servo object
  rotation(1);
  Serial2.begin(9600);
  delay(300);
  myservo.write(90);
}

uint num1 = 0, num2 = 0, terminalCounter = 0;
short mode = 0;

void loop() {
  char key = keypad.getKey();
  
  if (key){
    if(key == '*')
      mode = 1;
    else if(key == '/')
      mode = 2;
    else if(key == '=')
      mode = 0;
    else if (key >= '0' && key <='9'){
      num1 *= 10;
      num1 += key - '0';
      num1 = num1%181;
    }
    else {
      myservo.write(num1);
      num1 = 0;
    }
  }
  
  if(mode == 1){
    terminalCounter++;
    num2 *= 10;
    num2 += Serial2.parseInt();
    if(num2 > 18 || terminalCounter==3){
      myservo.write(num2);
      num2 = 0;
      mode = 0;
      terminalCounter=0;
    }
  }
  else if(mode == 2){
    int val = analogRead(potPin);
    val = map(val, 0, 1023, 0, 180);
    myservo.write(val);
  }
}

void rotation(int n){
  for (int i=0;i<n;i++){
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }
}
