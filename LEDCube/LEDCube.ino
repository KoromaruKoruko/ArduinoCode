PROGMEM const int SleepTime = 1000;

enum COLOR
{
  red = 0,
  green = 1,
  blue = 2,
  yellow = 3,
  purple = 4,
  cyan = 5,
  white = 6,
  black = 7,
};

static void Light(int R, int G, int B)
{
  digitalWrite(12, R);
  digitalWrite(11, G);
  digitalWrite(10, B);
};
  
void LedTo(COLOR col)
{
  switch(col)
  {
    case(COLOR::red):
      Light(HIGH,LOW,LOW);
    break;
    
    case(COLOR::green):
      Light(LOW,HIGH,LOW);
    break;
    
    case(COLOR::yellow):
      Light(HIGH,HIGH,LOW);
    break;
    
    case(COLOR::blue):
      Light(LOW,LOW,HIGH);
    break;
    
    case(COLOR::purple):
      Light(HIGH,LOW,HIGH);
    break;
    
    case(COLOR::cyan):
      Light(LOW,HIGH,HIGH);
    break;
    
    case(COLOR::white):
      Light(HIGH,HIGH,HIGH);
    break;
    
    case(COLOR::black):
      Light(LOW,LOW,LOW);
    break;
  }
}

void setup()
{
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
}

int output;
void loop() {
  LedTo(output);
  output++;
  if(output > 7)
  output = 0;
  delay(SleepTime);
}

