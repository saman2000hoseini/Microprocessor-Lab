#include<Keypad.h>
#include<Wire.h>
#include<LiquidCrystal.h>

#define RS_PIN 13
#define EN_PIN 12
#define D4_PIN 11
#define D5_PIN 10
#define D6_PIN 9
#define D7_PIN 8

#define DEV_ADDR 0b1010000
#define MODE 10
#define PRE_ADDR 15
#define WASH1_ADDR 20
#define WASH2_ADDR 25
#define DRY_ADDR 30

LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'7', '8', '9', '/'},
  {'4', '5', '6', '*'},
  {'1', '2', '3', '-'},
  {'c', '0', '=', '+'}
};
byte rowPins[ROWS] = {50, 51, 52, 53}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {25, 24, 23, 22}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String lcdFirst = "", lcdSecond = "";
int mode = -1;

int times[4];

void setup() {
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  lcd.begin(16, 2);
  Wire.begin();
  Serial2.begin(9600);
  Serial2.setTimeout(20000 * 2);
  lcdFirst = "Welcome";
  lcdSecond = "Enter Command";
  printOnLCD();
}

unsigned long time;
void loop() {
  if (mode == 0)
    preWash();
  else if (mode == 1)
    wash1();
  else if (mode == 2)
    wash2();
  else if (mode == 3)
    dry();
  else if (mode == 4)
    finished();
  else
    off();

  char key = keypad.getKey();

  if (mode >= 0 && mode < 4) {
    if (millis() % 1000 == 0) {
      lcdSecond = String((times[mode] - millis() + time) / 1000);
      printOnLCD();
    }
    if (millis() - time > times[mode]) {
      mode++;
      time = millis();
    }
  }

  if (key) {
    switch (key) {
      case 'c':
        settings();
        break;
      case'*':
        mode = 0;
        break;
      case '/':
        mode = 1;
        break;
      case'+':
        mode = 2;
        break;
      case '-':
        mode = 3;
        break;
      case '=':
        save();
        exit(0);
        break;
      default:
        mode++;
        break;
    }
    time = millis();
  }
}


void settings() {
  Serial2.flush();
  lcdFirst = "Settings";
  lcdSecond = "Enter Command";
  printOnLCD();

  String cmd = Serial2.readStringUntil(':');
  lcdSecond = cmd;
  printOnLCD();
  if (cmd == "set") {
    setPreWash();
    setWash1();
    setWash2();
    setDry();
  }
  else if (cmd == "pre")
    setPreWash();
  else if (cmd == "wash1")
    setWash1();
  else if (cmd == "wash2")
    setWash2();
  else if (cmd == "dry")
    setDry();
  else if (cmd == "read")
    readAll();
  else {
  }
  lcdFirst = "Normal Mode";
  lcdSecond = "";
  printOnLCD();
}

void save() {
  writeOnDevice(MODE, mode);
}

void setPreWash() {
  lcdFirst = "set";
  lcdSecond = "pre wash";
  printOnLCD();
  times[0] = 0;
  while (true) {
    char key = keypad.getKey();

    if (key) {
      if (key >= 48 && key <= 57) {
        times[0] *= 10;
        times[0] += key - 48;
      }
      else {
        writeOnDevice(PRE_ADDR, times[0]);
        times[0] *= 1000;
        return;
      }
    }
  }
}

void setWash1() {
  lcdFirst = "set";
  lcdSecond = "wash1";
  printOnLCD();
  times[1] = 0;
  while (true) {
    char key = keypad.getKey();

    if (key) {
      if (key >= 48 && key <= 57) {
        times[1] *= 10;
        times[1] += key - 48;
      }
      else {
        writeOnDevice(WASH1_ADDR, times[1]);
        times[1] *= 1000;
        return;
      }
    }
  }
}

void setWash2() {
  lcdFirst = "set";
  lcdSecond = "wash2";
  printOnLCD();
  times[2] = 0;
  while (true) {
    char key = keypad.getKey();

    if (key) {
      if (key >= 48 && key <= 57) {
        times[2] *= 10;
        times[2] += key - 48;
      }
      else {
        writeOnDevice(WASH2_ADDR, times[2]);
        times[2] *= 1000;
        return;
      }
    }
  }
}

void setDry() {
  lcdFirst = "set";
  lcdSecond = "dry";
  printOnLCD();
  times[3] = 0;
  while (true) {
    char key = keypad.getKey();

    if (key) {
      if (key >= 48 && key <= 57) {
        times[3] *= 10;
        times[3] += key - 48;
      }
      else {
        writeOnDevice(DRY_ADDR, times[3]);
        times[3] *= 1000;
        return;
      }
    }
  }
}

void readAll() {
  mode = readFromDevice(MODE);
  for (int i = 0; i < 4; i++) {
    times[i] = readFromDevice((i + 3) * 5);
    times[i] *= 1000;
  }
}

void writeOnDevice(byte addr, byte input) {
  Wire.beginTransmission(DEV_ADDR);
  Wire.write(addr);
  Wire.write(input);
  Wire.endTransmission();

  delay(200);

  lcdFirst = "Writing: " + String(addr);
  lcdSecond = String(input);
  printOnLCD();
}

byte readFromDevice(byte addr) {
  Wire.beginTransmission(DEV_ADDR);
  Wire.write(addr);
  Wire.endTransmission();
  Wire.requestFrom(DEV_ADDR, 1);
  byte output = Wire.read();


  lcdFirst = "Reading: " + String(addr);
  lcdSecond = String(output);
  printOnLCD();

  return output;
}

void preWash() {
  digitalWrite(A0, HIGH);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
}

void wash1() {
  digitalWrite(A0, LOW);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
}

void wash2() {
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, LOW);
}

void dry() {
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, HIGH);
}

void finished() {
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
}

void off() {
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
}

void printOnLCD() {
  lcd.clear();
  lcd.setCursor((16 - lcdFirst.length()) / 2, 0);
  lcd.print(lcdFirst);
  lcd.setCursor((16 - lcdSecond.length()) / 2, 1);
  lcd.print(lcdSecond);
}
