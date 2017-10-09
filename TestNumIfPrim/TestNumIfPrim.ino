#include "BigNumber.h"//Extreamly Large Numbers
//#include <avr/pgmspace.h>// PROGMEM


//ment to work but sim dont ack BigNumbers lib

BigNumber  _PtNum = 0;
const int ST_BufferSize = 45;//300 max prim numbers so that we run stabily
//const int MaxDecPlace = 25;
BigNumber  Endpt = 0; //a Factor can never be more then Half the Number

BigNumber Prim[ST_BufferSize] = {};//must be same as ST_BufferSize
int alloc_d = 0;
BigNumber CurrNum = 0;

String GetNumber() {
  //add number by digit
  String output = "";
  while (Serial.available())
  {
    char t = Serial.read();//read one byte
    if (isDigit(t))
    {
      output += t;
    }
    else {
      break;
    }
  }
  return output;
}

void InitNew()
{
  // put your setup code here, to run once:
  CurrNum = "2";//skip 1 and 0

  alloc_d = 1;//dont need to set Prim array agen
  //code will only look at all Numbers in array under alloc_d
  Prim[0] = "2";//must have a begining number

  //get number to check
  //Serial.println("pick a number between 0 and the " + String(MaxDecPlace) + "decimal place");
  Serial.print("Number:");
  while (!Serial.available()) {
    delay(25); // wait of input
  }
  _PtNum = GetNumber().c_str();
  BigNumber two = "2";
  Endpt = _PtNum / two;
  Serial.println("");
  Serial.println("Testing");

  //Send Working out
  Serial.println("Finding PrimeNumbers under " + (String)Endpt.toString());
  Serial.println("...to test " + (String)_PtNum.toString());
}

void Exit()
{
  InitNew();
}

void loop() {
  //if half way to end To PtCheck
  if (CurrNum > Endpt)
  {
    Serial.println("Checking input number");
    //retract Working
    bool isPrim = true;
    for (int x = 0; x < alloc_d; x++)
    {
      if ((_PtNum % Prim[x]) == 0)
      {
        isPrim = false;
        break;
      }
    }
    Serial.println("Done!");
    if (isPrim)
    {
      //Send Working out
      Serial.println("Its a Prime Number");
    }
    else
    {
      Serial.println("Its not a Prime Number");
    }
    Exit();
  }
  else
  {
    bool Priv = true;
    for (int x = 0; x < alloc_d; x++)
    {
      //if rem div of CurN . Prim Then Not Prim else Check Next
      if ((CurrNum % Prim[x]) == 0)
      {
        Priv = false;
        break;
      }
      //if no more Prim to check then CurN is new Prim
    }

    if (Priv)
    {
      //is Prim
      Serial.println(CurrNum);
      Prim[alloc_d] = CurrNum;
      alloc_d ++;
      if (alloc_d >= ST_BufferSize)
      {
        Serial.println("BufferOverFlow");
        Exit();
      }

    }
    CurrNum++;
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("booting");
  BigNumber::begin();//init bignumbers
  InitNew();
}
