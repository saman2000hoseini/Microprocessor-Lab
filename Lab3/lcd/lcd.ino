#include<LiquidCrystal.h>
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

#define RS_PIN 13
#define EN_PIN 12
#define D4_PIN 8
#define D5_PIN 9
#define D6_PIN 10
#define D7_PIN 11

LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

const String name = "Saman";

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  printName();
}

void printName(){
  for (int j = 0; j < 2; j++) 
    for (int i = 0; i < 32; i++) {
      lcd.clear();
      lcd.setCursor(i, 0);
      lcd.print(name);
      lcd.setCursor((i-16)%32, 1);
      lcd.print(name);
      lcd.setCursor(i-32, 0);
      lcd.print(name);
      delay(500);
    }
  lcd.clear();
}

const int password = "1234";
static int passCounter ,operand1, operand2, op;
static bool logedin = false, firstTime = true;
static String pass = "";

void loop() {
  char key = keypad.getKey();
  
  if (key){
    if (!logedin){
      if(firstTime){
        firstTime = false;
        lcd.noAutoscroll();
        lcd.clear();
        lcd.setCursor(6, 0);
      }
      if(key == '*')
        if(pass != password){
          lcd.setCursor(0, 1);
          pass = "";
          lcd.print("Wrong password!!");
          delay(3000);
          lcd.clear();
          lcd.setCursor(6, 0);
        }
        else{
          logedin = true;
          lcd.setCursor(0, 1);
          lcd.print("Correct password");
          delay(3000);
          lcd.clear();
          lcd.setCursor(0, 0);
        }
      else{
        pass += key;
        lcd.write(key);
      }
    }
    else
      calculator(key);
  }
}

short findLen(int num){
  int res = 0;
  while(num > 0){
    num/=10;
    res++;
  }

  return res;
}

void calculator(char key){
  if(key == 'c'){
    operand1 = operand2 = op = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
  }
  else{
    if(key != '=')
      lcd.print(key);
    switch(key){
      case '+':
        op = 1;
        break;
      case '-':
        op = 2;
        break;
      case '*':
        op = 3;
        break;
      case '/':
        op = 4;
        break;
      case '=':
        int res;
        switch(op){
          case 0:
            return;
          case 1:
            res = operand1 + operand2;
            break;
          case 2:
            res = operand1 - operand2;
            break;
          case 3:
            res = operand1 * operand2;
            break;
          case 4:
            res = operand1 / operand2;
            break;
          default:
            return;
        }
        lcd.clear();
        lcd.setCursor((16 - findLen(res))/2, 1);
        lcd.print(String(res));
        operand1 = res;
        op = operand2 = 0;
        lcd.setCursor(0, 0);
        break;
      default:
        if(op == 0)
          operand1 = operand1 * 10 + key - (int)'0';
        else
          operand2 = operand2 * 10 + key - (int)'0';
        break;
    }
  }
}
