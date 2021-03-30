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
byte rowPins[ROWS] = {41, 42, 43, 44}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {25, 24, 23, 22}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

Servo myservo;

int buzzerPin = A15, thermometer = A0;

volatile boolean danger;

String lcdFirst = "", lcdSecond = "";
bool isOpen = false, finished = true, startTimer = true;
unsigned long time, timer, temperature;

ISR (SPI_STC_vect) {
  danger = SPDR;

  if (danger) {
    openFurnace();
    lcdFirst = "Attention";
    lcdSecond = "High Temp!!";
    printOnLCD();
    trunLightsOn();
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

  pinMode(buzzerPin, OUTPUT);
  lcd.begin(16, 2);
  myservo.attach(2, 1000, 2000);
  openFurnace();
  initSystem();
}

void loop() {
  if (!danger) {
    if (!isOpen && (getTemp() >= temperature) && !finished) {
      if (startTimer) {
        startTimer = false;
        time = millis();
      }
      else if (!finished && millis() % 1000 == 0) {
        lcdFirst = "time left:";
        lcdSecond = String((timer - millis() + time) / 1000);
        printOnLCD();
      }
      else if (!finished && millis() - time > timer) {
        lcdFirst = "Cook Finished";
        lcdSecond = "Ready to open";
        printOnLCD();
        finished = true;
      }
    }

    char key = keypad.getKey();

    if (key) {
      buzz(50);
      if (!isOpen) {
        if (finished) {
          lcdFirst = "Furnace Opened";
          lcdSecond = "settings: /";
          printOnLCD();
          openFurnace();
        }
        else {
          lcdFirst = "Furnace Opened";
          lcdSecond = "Cook Canceled";
          printOnLCD();
          openFurnace();
        }
      }
      else {
        if (key == '/')
          initSystem();
        else
          closeFurnace();
      }
    }
  }
  else if (millis() % 500 == 0) {
    trunLightsOn();
  }
}

void initSystem() {
  lcdFirst = "Furnace Opened";
  lcdSecond = "Enter Temp";
  printOnLCD();

  set(&temperature);
  lcdFirst = "Furnace Opened";
  lcdSecond = "Enter Time";
  printOnLCD();
  set(&timer);
  timer *= 1000;
  closeFurnace();
}

void set(unsigned long* entry) {
  *entry = 0;

  while (true) {
    char key = keypad.getKey();

    if (key) {
      buzz(50);

      if (key >= 48 && key <= 57) {
        *entry *= 10;
        *entry += key - 48;
      }
      else if (*entry > 0)
      {
        return;
      }
    }
  }
}

void openFurnace() {
  buzz(1000);
  for (int pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(5);
  }

  isOpen = true;
}

void closeFurnace() {
  startTimer = true;
  finished = false;
  buzz(1000);
  for (int pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(5);
  }

  isOpen = false;
  lcdFirst = "Furnace Closed";
  lcdSecond = "Started Cooking";
  printOnLCD();
}

void buzz(int t) {
  digitalWrite(buzzerPin, HIGH);
  tone(buzzerPin, 1000, t);
}

int getTemp() {
  return analogRead(thermometer) * 0.48828125;
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

void trunLightsOn() {
  turnLightsOff();

  if (firstLight){
    digitalWrite(A8, HIGH);
    buzz(400);
  }
  else
    digitalWrite(A9, HIGH);
    
  firstLight = !firstLight;
}
