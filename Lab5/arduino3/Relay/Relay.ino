#define PIN 52
#define DELAY 2000
void setup() {
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
}

void loop() {
  digitalWrite(PIN, LOW);
  delay(DELAY);
  digitalWrite(PIN, HIGH);
  delay(DELAY);
}
