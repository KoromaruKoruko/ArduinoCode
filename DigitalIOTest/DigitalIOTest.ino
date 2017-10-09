PROGMEM const int SleepTime = 1000;

void setup() {
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);

  pinMode(7, INPUT);
  pinMode(6, INPUT);
  pinMode(5, INPUT);
  pinMode(4, INPUT);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  
  digitalWrite(11,HIGH);
  digitalWrite(10,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(8,HIGH);
}

void loop() {
  if(digitalRead(4) == HIGH)
  digitalWrite(8, LOW);
  else
  digitalWrite(8, HIGH);

  delay(SleepTime);
  
  if(digitalRead(5) == HIGH)
  digitalWrite(9, LOW);
  else
  digitalWrite(9, HIGH);

  delay(SleepTime);
  
  if(digitalRead(6) == HIGH)
  digitalWrite(10, LOW);
  else
  digitalWrite(10, HIGH);

  delay(SleepTime);
  
  if(digitalRead(7) == HIGH)
  digitalWrite(11, LOW);
  else
  digitalWrite(11, HIGH);
  
  delay(SleepTime);
}
