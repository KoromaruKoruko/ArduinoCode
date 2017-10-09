PROGMEM const bool CryptoType = true;
PROGMEM const int SleepTime = 1000;
const int Pins[2][4] = 
{
  {11, 10, 9, 8},
  {7, 6, 5, 4},
};
void Encrypt();
void Decrypt();

void setup() {
  Serial.begin(9600);
  
  if (CryptoType == true) {
    Serial.println("Encoding mode");
    for(int x =0 ;x < 4; x++)
    {
      pinMode(Pins[0][x], OUTPUT);
      digitalWrite(Pins[0][x], LOW);
    }

    for(int x =0 ;x < 4; x++)
    {
      pinMode(Pins[1][x], INPUT);
      digitalWrite(Pins[1][x], LOW);//turn on pulldown resistors
    }
  }
  else
  {
    Serial.println("Decoding mode");
    for(int x =0 ;x < 4; x++)
    {
      pinMode(Pins[0][x], INPUT);
      digitalWrite(Pins[0][x], LOW);//turn on pulldown resistors
    }

    for(int x =0 ;x < 4; x++)
    {
      pinMode(Pins[1][x], OUTPUT);
      digitalWrite(Pins[0][x], LOW);
    }
  }
}
void PrintBoolArray(bool a[])
{
  for(int x = 0; x < 8; x++)
  {
    if(a[x])
    Serial.print("1");
    else
    Serial.print("0");
  }
  Serial.println("");
}

void loop() {
  if(CryptoType)
  Encrypt();
  else
  Decrypt();
}

void Encrypt()
{
  for(int x =0 ;x < 4; x++)
  {
    digitalWrite(Pins[0][x], LOW);
  }

  
  Serial.println("Waitting For Input");
  while(!Serial.available()){ delay(10); }//wait for input
  
  bool Input[8] = {false,false,false,false,false,false,false,false}; 
  bool Output[8] = {false,false,false,false,false,false,false,false};

  for(int x =0; x< 8; x++)
  {
    Input[x] = false;
    Output[x] = false;
  }
  
  
  byte i = Serial.read();

  for(byte x = 0; x < 8; x++){
    Input[x] = (i & (1 << x));//returns if bit is on
  }
  
  PrintBoolArray(Input);
  for(byte x = 0; x < 4; x++)
  {
    if(Input[x])
      digitalWrite(Pins[0][x], HIGH);
      else
      digitalWrite(Pins[0][x], LOW);
  }
  delay(SleepTime); // wait for input to be 100% ready

  for(byte x = 0; x < 4; x++)
  {
    if(digitalRead(Pins[1][x]) == HIGH)//read pin output
    Output[x] = true;
    else
    Output[x] = false;
    
  }
  for(byte x = 0; x < 4; x++)
  {
    if(Input[x+4])
      digitalWrite(Pins[0][x], HIGH);
    else
      digitalWrite(Pins[0][x], LOW);
  }
  delay(SleepTime); // wait for input to be 100% ready

  for(byte x = 0; x < 4; x++)
  {
    if(digitalRead(Pins[1][x]) == HIGH)//read pin output
      Output[x+4] = true;
    else
      Output[x+4] = false;
  }
  PrintBoolArray(Output);
  Serial.println("clean up");
  for(int x = 0; x < 4; x++)
  {
    digitalWrite(Pins[0][x], LOW);
  }
  
  byte o = 0;
  for(byte x = 0; x < 8; x++)
  {
    if(Output[x]){
      o += ((x+1)^2);
    }
  }
  Serial.write(o);
  Serial.println();
}

void Decrypt()
{
  //comming soon
}



















