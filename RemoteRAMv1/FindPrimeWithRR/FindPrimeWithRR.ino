//Discovery /w Remote RAM
PROGMEM const byte END = 255;
PROGMEM const byte REQInt = 166;
PROGMEM const byte SETInt = 167;
PROGMEM const byte SayByte = 10;
PROGMEM const byte CreateAddress = 11;
PROGMEM const byte HELO[6] {12, 12, 44, 51, 52, 112};
PROGMEM const int SizeOfHELO = 6;
PROGMEM const int ReadTimeoutTime = 200;

int LargestAddress = 0;

bool ReadErrOut = false;
void Read(byte Buffer[], int Count)
{
  int start = millis();
  int pos = 0;
  while (millis() - start < ReadTimeoutTime && pos != Count - 1)
  {
    if (Serial.available() > 0) {
      Buffer[pos] = Serial.read();
      pos++;
      start = millis();
    }
  }
  if (millis() - start > ReadTimeoutTime)
    ReadErrOut = true;
    else
    ReadErrOut = false;
}

void Connect()
{
  byte HL[SizeOfHELO];
  Read(HL, SizeOfHELO);
  bool Write = ReadErrOut;
  if (Write)
  {
    for (int x = 0; x < SizeOfHELO; x++)
    {
      if (HELO[x] != HL[x])
      {
        Write = false;
        Connect();
        break;
      }
    }
    if (Write)
    {
      Serial.write((const char*)HELO);
    }
  }
  else
  {
    Connect();
  }
}

void CreateInt(int value)
{
  Serial.write(CreateAddress);//create new Address
  SetInt(LargestAddress, value);
  LargestAddress++;//step up largest address
}
void SetInt(int addr, int value)
{
  Serial.write(SETInt);//set that address
  Serial.write(highByte(addr));//send address
  Serial.write(lowByte(addr));
  Serial.write(highByte(value));//send new value
  Serial.write(lowByte(value));
}

union  ByteArrayToint { //converter
  byte array[2];
  int16_t integer;
};

int GetInt(int addr)
{
  Serial.write(REQInt);//set that address
  Serial.write(highByte(addr));//send address
  Serial.write(lowByte(addr));
  byte value[2];
  Read(value, 2);
  ByteArrayToint Conv = {value[0], value[1]};
  return Conv.integer;
}

void say(String what)
{
  Serial.write(SayByte);
  Serial.print(what + "\n"); //STD Encoding is ASCII
}

//-------------------------------------------------------------------------------------------------------------
int Curr = 1;

void setup() {
  Serial.begin(9600);
  //Get connection
  Connect();
}

void loop() {//do discovery mode;
  if (Curr > 10) {
    if (((String)Curr).endsWith("1") || ((String)Curr).endsWith("7") || ((String)Curr).endsWith("9"))
      Curr += 2; //to 3, 9
    else if (((String)Curr).endsWith("3"))
      Curr += 4; //to 7
    else
      Curr++;
  }
  bool IsPrime = true;
  for (int x = 0; x < LargestAddress; x++)
  {
    int tmp = GetInt(x);
    if (tmp % Curr == 0)
    {
      IsPrime = false;
      break;
    }
  }
  if (IsPrime)
  {
    CreateInt(Curr);
    String what = "Found ";
    what += String(Curr);
    say(what);
  }
}
