//Discovery /w Remote RAM
const byte END = 255;
const byte REQInt = 166;
const byte SETInt = 167;
const byte SayByte = 10;
const byte CreateAddress = 11;
const byte HELO[6] {0x48, 0x45, 0x4c, 0x4f};
const int SizeOfHELO = 4;
const int ReadTimeoutTime = 200;

int LargestAddress = 0;

bool array_cmp(byte a[], byte b[], int len_a, int len_b)
{
  int n;

  // if their lengths are different, return false
  if (len_a != len_b) return false;

  // test each element to be the same. if not, return false
  for (n = 0; n < len_a; n++) {
    if (a[n] != b[n]) {
      return false;
    }
  }

  //ok, if we have not returned yet, they are equal :)
  return true;
}

void Connect()
{
  byte HL[SizeOfHELO];

  while (!array_cmp(HELO, HL, SizeOfHELO, SizeOfHELO))
  {
    if (Serial.available() >= SizeOfHELO) {
      Serial.readBytes(HL, SizeOfHELO);
    }
  }
  Serial.print((const char*)HELO);
}

void CreateInt(int value)
{
  Serial.write(CreateAddress);//create new Address
  //while (Serial.available() < 1) ;
  //Serial.read();
  SetInt(LargestAddress, value);
  LargestAddress++;//step up largest address
}
void SetInt(int addr, int value)
{
  Serial.write(SETInt);//set that address
  Serial.write(lowByte(addr));
  Serial.write(highByte(addr));//send address
  Serial.write(lowByte(value));
  Serial.write(highByte(value));//send new value

  //while (Serial.available() < 1) ;
  //Serial.read();
}

union  ByteArrayToint { //converter
  byte array[2];
  int integer;
};

int GetInt(int addr)
{
  Serial.write(REQInt);//set that address
  Serial.write(lowByte(addr));
  Serial.write(highByte(addr));//send address
  byte value[2];
  while (Serial.available() < 2) ;
  Serial.readBytes(value, 2);
  ByteArrayToint Conv = {value[0], value[1]};
  return Conv.integer;
}

void say(String what)
{
  Serial.write(SayByte);
  Serial.println(what); //STD Encoding is ASCII
  //while (Serial.available() < 1) ;
  //Serial.read();
}

//-------------------------------------------------------------------------------------------------------------
int Curr = 1;

void setup() {
  Serial.begin(115200);
  //Get connection
  Connect();
}

void loop() {//do discovery mode;
  if (Curr > 10)
  {
    if (String(Curr).endsWith("1") || String(Curr).endsWith("7") || String(Curr).endsWith("9") )
      Curr += 2;
    else if (String(Curr).endsWith("3"))
      Curr += 4;
    else
      Curr++;
  }
  else
    Curr++;

  bool IsPrime = true;
  for (int x = 0; x < LargestAddress; x++)
  {
    int tmp = GetInt(x);
    //say("On GetInt Recived :" + String(tmp));
    if ((Curr % tmp) == 0)
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
