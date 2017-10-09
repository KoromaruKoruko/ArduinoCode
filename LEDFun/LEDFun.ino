const int SleepTime = 100;
const int Pins[3]
{
  4,5,6
};
const int PinAmont = 3;

void setup() {
  for (int x = 0; x < 3; x++)
  {
    pinMode(Pins[x], OUTPUT);
  }
}
int y = 0;
void loop() {
    digitalWrite(Pins[y], HIGH);
  
  delay(SleepTime);
    digitalWrite(Pins[y], LOW);
    y++;

    if(y == PinAmont)
      y= 0;
}
