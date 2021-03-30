#include <Servo.h>
#include<Keypad.h>
#include<LiquidCrystal.h>
#include <SPI.h>

#define RS_PIN 13
#define EN_PIN 12
#define D4_PIN 8
#define D5_PIN 9
#define D6_PIN 10
#define D7_PIN 11

LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'7', '8', '9', '/'},
  {'4', '5', '6', '*'},
  {'1', '2', '3', '-'},
  {'c', '0', '=', '+'}
};
byte rowPins[ROWS] = {40, 41, 42, 43}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {25, 24, 23, 22}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

Servo myservo;

volatile boolean danger;

int openLEDPin = A0, closedLEDPin = A1, buzzerPin = A15;

String lcdFirst = "", lcdSecond = "";
bool isOpen = false;
unsigned long time;
unsigned long maxOpenTime = 5000;

ISR (SPI_STC_vect) {
  danger = SPDR;

  if (danger) {
    closeSafe();
    lcdFirst = "Attention";
    lcdSecond = "High Temp!!";
    printOnLCD();
    turnLightsOn();
  } else {
    turnLightsOff();
  }
}

void setup() {
  pinMode(A8, OUTPUT);
  pinMode(A9, OUTPUT);

  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);
  pinMode(MISO, OUTPUT);
  danger = false;
  SPI.attachInterrupt();

  pinMode(openLEDPin, OUTPUT);
  pinMode(closedLEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  lcd.begin(16, 2);
  myservo.attach(2, 1000, 2000);
  Serial2.begin(9600);
  Serial2.setTimeout(maxOpenTime * 2);
  myservo.write(0);
  initSystem();
}

String password = "", pass = "";
short mode = 0;

void loop() {
  if (!danger) {
    char key = keypad.getKey();

    if (isOpen) {
      if (millis() % 1000 == 0) {
        lcdFirst = "will close in:";
        lcdSecond = String((maxOpenTime - millis() + time) / 1000);
        printOnLCD();
      }
      if (millis() - time > maxOpenTime)
        closeSafe();
    }

    if (key) {
      buzz(50);
      switch (key) {
        case 'c':
          if (isOpen)
            closeSafe();
          else
            pass = "";
          break;
        case '/':
          if (isOpen)
            settings();
          break;
        case '=':
          if (pass == password) {
            lcdFirst = "Correct Password";
            lcdSecond = "Safe Opened";
            printOnLCD();
            pass = "";
            openSafe();
          }
          else {
            lcdFirst = "Incorrect";
            lcdSecond = "Password :((";
            printOnLCD();
            pass = "";
          }
          break;
        default:
          pass += key;
          break;
      }
    }
  }
  else if (millis() % 500 == 0) {
    turnLightsOn();
  }
}

void settings() {
  lcdFirst = "Settings";
  lcdSecond = "Enter Command";
  printOnLCD();

  Serial2.flush();
  String cmd = Serial2.readStringUntil(':');
  buzz(80);
  lcdSecond = cmd;
  printOnLCD();
  if (cmd == "setpass")
    setPass();
  else if (cmd == "settimer")
    setTimer();
  else {
    lcdFirst = "Safe Closed";
    lcdSecond = "Enter Password";
    printOnLCD();
  }
}

void initSystem() {
  openSafe();
  setPass();
  closeSafe();
}

void setTimer() {
  maxOpenTime = 0;

  while (true) {
    char key = keypad.getKey();

    if (key) {
      buzz(50);

      if (key >= 48 && key <= 57) {
        maxOpenTime *= 10;
        maxOpenTime += key - 48;
      }
      else
      {
        lcdFirst = "Timer changed";
        lcdSecond = "to: " + String(maxOpenTime);
        printOnLCD();
        maxOpenTime *= 1000;
        return;
      }
    }
  }
}

void setPass() {
  lcdFirst = "Please Enter";
  lcdSecond = "New Password";
  printOnLCD();
  String newPassword = "";
  while (true) {
    char key = keypad.waitForKey();
    buzz(50);

    if (key == '/') {
      lcdFirst = "Operation";
      lcdSecond = "Canceled";
      printOnLCD();
      return;
    }
    else if (key == '=') {
      password = newPassword;
      break;
    }
    else if (key == 'c')
      newPassword = "";
    else
      newPassword += key;
  }
  lcdFirst = "Password Has";
  lcdSecond = "Been Set!!!";
  printOnLCD();
}

int pos = 0;
void openSafe() {
  buzz(1000);
  openLED();
  for (; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(5);
  }
  time = millis();
  isOpen = true;
}

void closeSafe() {
  buzz(1000);
  closedLED();
  for (; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(5);
  }

  isOpen = false;
  lcdFirst = "Safe Closed";
  lcdSecond = "Enter Password";
  printOnLCD();
}

void openLED() {
  digitalWrite(openLEDPin, HIGH);
  digitalWrite(closedLEDPin, LOW);
}

void closedLED() {
  digitalWrite(openLEDPin, LOW);
  digitalWrite(closedLEDPin, HIGH);
}

void buzz(int t) {
  digitalWrite(buzzerPin, HIGH);
  tone(buzzerPin, 1000, t);
}

void printOnLCD() {
  lcd.clear();
  lcd.setCursor((16 - lcdFirst.length()) / 2, 0);
  lcd.print(lcdFirst);
  lcd.setCursor((16 - lcdSecond.length()) / 2, 1);
  lcd.print(lcdSecond);
}

bool firstLight = true;

void turnLightsOff() {
  digitalWrite(A8, LOW);
  digitalWrite(A9, LOW);
}

void turnLightsOn() {
  turnLightsOff();

  if (firstLight) {
    digitalWrite(A8, HIGH);
    buzz(400);
  }
  else
    digitalWrite(A9, HIGH);

  firstLight = !firstLight;
}
