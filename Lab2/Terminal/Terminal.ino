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
  int key = Serial2.parseInt();
  
  if (key){
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
      case 1:
        digitalWrite(A0, HIGH);
        break;
      case 2:
        digitalWrite(A1, HIGH);
        break;
      case 3:
        digitalWrite(A2, HIGH);
        break;
      case 4:
        digitalWrite(A3, HIGH);
        break;
      case 5:
        digitalWrite(A7, HIGH);
        break;
      case 6:
        digitalWrite(A6, HIGH);
        break;
      case 7:
        digitalWrite(A5, HIGH);
        break;
      case 8:
        digitalWrite(A4, HIGH);
        break;
      case 9:
        digitalWrite(A8, HIGH);
        break;
    }
  }
}
