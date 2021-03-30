#include <SPI.h>

#define SS0 48
#define SS1 49

int thermometer = A0, photocell = A15;

void setup (void)
{
  pinMode(SS0, OUTPUT);
  pinMode(SS1, OUTPUT);
  digitalWrite(SS0, HIGH);
  digitalWrite(SS1, HIGH);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);

  Serial.begin(9600);
  hello();
}


void loop (void)
{
  Serial.println("Reporting Temperature");
  String tempReport = "Temp is: ";
  tempReport += String(getTemp());
  tempReport += "\n";
  writeSlave(tempReport, true);
  delay(900);

  Serial.println("Reporting Light");
  String lightReport = "Light value is: ";
  lightReport += String(getLight());
  lightReport += "\n";
  writeSlave(lightReport, false);
  delay(900);
}

void hello() {
  Serial.println("sending hello to slave 0");
  writeSlave("Hello, World!\n", true);
  delay(400);

  Serial.println("sending hello to slave 1");
  writeSlave("Hi\n", false);
  delay(400);
}

void writeSlave(String output, bool s0) {
  if (s0)
    digitalWrite(SS0, LOW);
  else
    digitalWrite(SS1, LOW);

  int n = output.length();
  char char_array[n + 1];
  strcpy(char_array, output.c_str());

  char c;
  for (const char * p = char_array; c = *p; p++)
    SPI.transfer(c);

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

int getLight() {
  return analogRead(photocell);
}
