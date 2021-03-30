#include <SPI.h>
#include <Servo.h>

Servo servo[3];

#define SS0 48
#define SS1 49

int thermometer = A0;

void setup (void)
{
  pinMode(SS0, OUTPUT);
  pinMode(SS1, OUTPUT);
  digitalWrite(SS0, HIGH);
  digitalWrite(SS1, HIGH);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);

  servo[0].attach(5, 1000, 2000);
  servo[1].attach(6, 1000, 2000);
  servo[2].attach(7, 1000, 2000);
  openBars();
  Serial.begin(9600);
}


void loop (void)
{
  String tempReport = "Temp is: ";
  tempReport += String(getTemp());
  Serial.println(tempReport);

  if (getTemp() > 80) {
    writeSlave(true, true);
    writeSlave(true, false);
    Serial.println("Danger!! Bars are closed");
    closeBars();
  } else {
    writeSlave(false, true);
    writeSlave(false, false);
    openBars();
  }
  delay(200);
}

void writeSlave(bool output, bool s0) {
  if (s0)
    digitalWrite(SS0, LOW);
  else
    digitalWrite(SS1, LOW);

  SPI.transfer(output);

  delay(100);

  if (s0)
    digitalWrite(SS0, HIGH);
  else
    digitalWrite(SS1, HIGH);
}

int getTemp() {
  int difference = analogRead(A0) - analogRead(A1);
  if (difference < 0)
    difference--;
  return difference * 0.48828125;
}


int pos = 180;
void openBars() {
  for (; pos < 181; pos += 1) {
    servo[0].write(pos);
    servo[1].write(pos);
    servo[2].write(pos);
    delay(50);
  }
}

void closeBars() {
  for (; pos > 0; pos -= 1) {
    servo[0].write(pos);
    servo[1].write(pos);
    servo[2].write(pos);
    delay(5);
  }
}
