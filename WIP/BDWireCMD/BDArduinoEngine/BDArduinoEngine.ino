PROGMEM const byte LoopSize     = 20;
bool FirstStartup = true;//to add resart func
bool LoopLog      = false;
bool InLoop       = false;
byte Loop[LoopSize];
int  looppos      = 0;
int  Pins[11]     = {  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12};
bool PinState[11] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
bool PinMode[11]  = {OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT};

//User Func's
void UserCodeStartUp();//compiler help
void UserCodeLoop();//says that the fnction is some where in the code
void OnUserLeave();//and stops un-wanted errors
void OnUserJoin();
void OnPong();

void setup() {
  if (FirstStartup)
  {
    Serial.begin(9600);
    FirstStartup = false;
  }
  for (int x = 0; x < 11; x++)
  {
    Pins[x] = 2 + x;
    PinState[x] = LOW;
    PinMode[x] = OUTPUT;
    digitalWrite(Pins[x], LOW);
  }
  clearLoop();
  UserCodeStartUp();
}

//Engine Functions--------------------------------------------------------------
void clearLoop()
{
  for (int x = 0; x < LoopSize; x++)
    Loop[x] = 255;//set loop to null
}

void SetPin(int pin, bool state)
{
  if (state)
  {
    digitalWrite(Pins[pin], HIGH);
    PinState[pin] = HIGH;
  }
  else
  {
    digitalWrite(Pins[pin], LOW);
    PinState[pin] = LOW;
  }
}

void EchoPinState(int Pin)
{
  if (PinMode[Pin] == OUTPUT) {
    if (PinState[Pin] == LOW)
      Serial.write(0);
    else
      Serial.write(10);
  } else
  {
    if (digitalRead(Pins[Pin]) == HIGH)
      Serial.write(10);
    else
      Serial.write(0);
  }
}

void SetPinMode(int pin, bool mode)
{
  if (mode) //input
  {
    PinMode[pin] = INPUT;
    pinMode(Pins[pin], INPUT);
  }
  else//output
  {
    PinMode[pin] = OUTPUT;
    pinMode(Pins[pin], OUTPUT);
  }
}

void EchoPinMode(int Pin)
{
  if (PinMode[Pin] == OUTPUT)
    Serial.write(0);
  else
    Serial.write(10);
}

//low---------------------------------------------------------------------------

void Exec(byte x)
{
  switch (x)
  {
    //SetPinState
    case (0):
      SetPin(0, false);
      break;
    case (1):
      SetPin(0, true);
      break;
    case (2):
      SetPin(1, false);
      break;
    case (3):
      SetPin(1, true);
      break;
    case (4):
      SetPin(2, false);
      break;
    case (5):
      SetPin(2, true);
      break;
    case (6):
      SetPin(3, false);
      break;
    case (7):
      SetPin(3, true);
      break;
    case (8):
      SetPin(4, false);
      break;
    case (9):
      SetPin(4, true);
      break;
    case (10):
      SetPin(5, false);
      break;
    case (11):
      SetPin(5, true);
      break;
    case (12):
      SetPin(6, false);
      break;
    case (13):
      SetPin(6, true);
      break;
    case (14):
      SetPin(7, false);
      break;
    case (15):
      SetPin(7, true);
      break;
    case (16):
      SetPin(8, false);
      break;
    case (17):
      SetPin(8, true);
      break;
    case (18):
      SetPin(9, false);
      break;
    case (19):
      SetPin(9, true);
      break;
    case (20):
      SetPin(10, false);
      break;
    case (21):
      SetPin(10, true);
      break;

    //REQ Pin State
    case (22):
      EchoPinState(0);
      break;
    case (23):
      EchoPinState(1);
      break;
    case (24):
      EchoPinState(2);
      break;
    case (25):
      EchoPinState(3);
      break;
    case (26):
      EchoPinState(4);
      break;
    case (27):
      EchoPinState(5);
      break;
    case (28):
      EchoPinState(6);
      break;
    case (29):
      EchoPinState(7);
      break;
    case (30):
      EchoPinState(8);
      break;
    case (31):
      EchoPinState(9);
      break;
    case (32):
      EchoPinState(10);
      break;

    //set pin mode
    case (33):
      SetPinMode(0, false);
      break;
    case (34):
      SetPinMode(0, true);
      break;
    case (35):
      SetPinMode(1, false);
      break;
    case (36):
      SetPinMode(1, true);
      break;
    case (37):
      SetPinMode(2, false);
      break;
    case (38):
      SetPinMode(2, true);
      break;
    case (39):
      SetPinMode(3, false);
      break;
    case (40):
      SetPinMode(3, true);
      break;
    case (41):
      SetPinMode(4, false);
      break;
    case (42):
      SetPinMode(4, true);
      break;
    case (43):
      SetPinMode(5, false);
      break;
    case (44):
      SetPinMode(5, true);
      break;
    case (45):
      SetPinMode(6, false);
      break;
    case (46):
      SetPinMode(6, true);
      break;
    case (47):
      SetPinMode(7, false);
      break;
    case (48):
      SetPinMode(7, true);
      break;
    case (49):
      SetPinMode(8, false);
      break;
    case (50):
      SetPinMode(8, true);
      break;
    case (51):
      SetPinMode(9, false);
      break;
    case (52):
      SetPinMode(9, true);
      break;
    case (53):
      SetPinMode(10, false);
      break;
    case (54):
      SetPinMode(10, true);
      break;

    //REQ pin mode
    case (55):
      EchoPinMode(0);
      break;
    case (56):
      EchoPinMode(1);
      break;
    case (57):
      EchoPinMode(2);
      break;
    case (58):
      EchoPinMode(3);
      break;
    case (59):
      EchoPinMode(4);
      break;
    case (60):
      EchoPinMode(5);
      break;
    case (61):
      EchoPinMode(6);
      break;
    case (62):
      EchoPinMode(7);
      break;
    case (63):
      EchoPinMode(8);
      break;
    case (64):
      EchoPinMode(9);
      break;
    case (65):
      EchoPinMode(10);
      break;

    //MISC
    case (66): //ping
      Serial.write(67);
      break;

    case (67): //need to add some sort of ping pong system
    OnPong();
      break;

    case (68): //on disconnect
      OnUserLeave();
      break;

    case (69): //on connect
      OnUserJoin();
      break;

    case (70):
      setup();//restart
      break;

    case (71): //remote ram enable
      Serial.write(255);
      break;

    case (72): //req remote ram alloc space
      Serial.write(255);
      break;

    case (73): //req remote ram address
      Serial.write(255);
      break;

    case (74): //req removal of alloc space
      Serial.write(255);
      break;

    case (75)://loop start
      if (InLoop)
        InLoop = false;
      clearLoop();
      LoopLog = true;
      looppos = 0;
      break;

    case (76)://loop end
      LoopLog = false;
      looppos = 0;
      break;

    case (77)://stop looping
      InLoop = false;
      looppos = 0;
      break;

    case (78)://start looping
      if (!LoopLog) {
        InLoop = true;
        looppos = 0;
      }
      break;

    case (79):
      delay(100);
      break;

    case (80):
      delay(1000);
      break;

    case (81):
      Serial.write(LoopSize);
      break;
  }
  if (LoopLog && looppos < LoopSize && (x < 75 || x > 78))
  {
    Loop[looppos] = x;
    looppos++;
  }
}

void loop() {
  UserCodeLoop();
  if (InLoop) {
    Exec(Loop[looppos]);
    looppos++;
    if (looppos + 1 == LoopSize)
      looppos = 0;
  }
  if (Serial.available()) {
    byte action = Serial.read();
    Exec(action);
  }
}

//OWNER     : Bailey. Drahoss
//NAME      : BDArduinoEngine

//engine functions
//clearLoop      clears all items in the loop
//SetPin         sets the pin state HIGH - LOW . digital only
//EchoPinState   # DONT USE # it only tells the serial port
//SetPinMode     sets the pin mode INPUT - OUTPUT . digital only
//EchoPinMode    # DONT USE # it only tells the serial port

//to get the pin state use
//PinState[(int)Pin NUM]
//for mode use
//PinMode[(int)Pin NUM]

//if you need to see if the user has enterd a loop check this
//InLoop

//Your code here

//Tip dont remove any of the pre made functions

void OnPong()//when the debugging client has sent a pong
{
  //WIP
}

void OnUserJoin()//when the debugging client has joined
{
  
}

void OnUserLeave()//when the debugging client has left
{
  
}

void UserCodeStartUp()//Startup
{

}

void UserCodeLoop()//Loop
{

}

