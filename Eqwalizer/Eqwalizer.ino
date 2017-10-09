const int InputPin = A0;
const int Seperator = 100;
const int OutputPins[11]
{
  2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
};
int OutputPinState[11]
{
  LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW
};


void setup() {
  pinMode(InputPin, INPUT);
  //analogWrite(InputPin,);
  Serial.begin(9600);

  for (int x = 0; x < 11; x++)
    pinMode(x, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop() {
  float in = analogRead(InputPin);
  Serial.println(String(in));
  for(int x = 0,o=0; x < 11;x++,o+=Seperator)
  {
    if(in > o)
      OutputPinState[x] = HIGH;
    else
      OutputPinState[x] = LOW;
  }
  delay(20);

  for (int x = 0; x < 11; x++) {
    digitalWrite(OutputPins[x], OutputPinState[x]);
  }
  delay(100);
}
