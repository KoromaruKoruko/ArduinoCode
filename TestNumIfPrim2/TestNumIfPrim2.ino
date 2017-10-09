String Serial_ReadLine()//basic SerialShortCut
{
  String o;
  while (true)
  {
    if (Serial.available())
    {
      char newchar = Serial.read();
      if (newchar == '\n')
        break;
      else
      {
        o += newchar;
      }
    }
  }
  return o;
}

bool Speed(String num, bool output = false)//Logical lookup
{
  long NUM = atol(num.c_str());
  long Curr = 1;
  long EndPt = (NUM / 2) + 2;
  long tmr = millis();
  if (output) {
    Serial.println("NUM  =" + String(NUM));
    Serial.println("EndPt=" + String(EndPt));
    Serial.println("Working on it!");
  }

  if (!num.endsWith("1") && !(num).endsWith("3") && !(num).endsWith("7") && !(num).endsWith("9") && NUM > 10)
  {
    if (output)
      Serial.println(num + " cant be a prime number because it doesn't end in 1,3,7,9");
    return false;
  }
  else
  {
    while (true)
    {
      //every 10 sec report where i am
      if (millis() - tmr > 10000 && output) {
        Serial.println("AT " + String(Curr) + "/" + String(EndPt));
        tmr = millis();
      }

      if (Curr > 12)
      {
        if (((String)Curr).endsWith("1") || ((String)Curr).endsWith("7") || ((String)Curr).endsWith("9"))
          Curr += 2; //to 3, 9
        else if (((String)Curr).endsWith("3"))
          Curr += 4; //to 7
      }
      else
        Curr++;

      if ((NUM % Curr) == 0)
      {
        if (output) {
          Serial.println(num + " is not a prime number ");
          Serial.println("because its a factor of " + ((String)Curr));
        }
        return false;
      }
      else {
        if (Curr > EndPt)
        {
          if (output)
            Serial.println(num + " is a prime number");
          return true;
        }
      }
    }
  }
}

bool DataCrunch(String num)//Pure lookup
{
  long NUM = atol(num.c_str());
  long Curr = 1;
  long EndPt = (NUM / 2) + 2;
  long tmr = millis();
  Serial.println("NUM  =" + String(NUM));
  Serial.println("EndPt=" + String(EndPt));
  Serial.println("Working on it!");



  while (true)
  {
    //every 10 sec report where i am
    if (millis() - tmr > 10000) {
      Serial.println("AT " + String(Curr) + "/" + String(EndPt));
      tmr = millis();
    }
    Curr++;
    if ((NUM % Curr) == 0)
    {
      Serial.println(num + " is not a prime number ");
      Serial.println("because its a factor of " + ((String)Curr));
      return false;
    }
    else {
      if (Curr > EndPt)
      {
        Serial.println(num + " is a prime number");
        return true;
      }
    }
  }
}

void List()//Discovery
{
  unsigned int Curr = 2;
  Serial.println("Starting List Search");
  Serial.println("using Speed(S) for Validation");
  Serial.println("Printing Found Prime Numbers");
  while (true)
  {
    if (Speed(String(Curr), false))
      Serial.println(String(Curr));

    //jump to next posable prime number
    if (((String)Curr).endsWith("1") || ((String)Curr).endsWith("7") || ((String)Curr).endsWith("9"))
      Curr += 2; //to 3, 9
    else if (((String)Curr).endsWith("3"))
      Curr += 4; //to 7
    else Curr++;
  }
}

void CMD(String Input)
{
  String num = "";
  String m = "";

  if (Input.indexOf(" ") != -1)
  {
    num = Input.substring(Input.indexOf(" ") + 1);
    m = Input.substring(0, Input.indexOf(" "));
  }
  else
  {
    m = Input;
  }

  m.toLowerCase();

  if (m == "s" || m == "speed") {
    if (num != "") {

      long Start = millis();
      bool rt = Speed(num, true);
      Serial.println("exited in " + String(millis() - Start) + "ms");
      Serial.println("with result " + (String)((rt) ? "True" : "False"));
    }
    else {
      Serial.println("you need to add a number");
      Serial.println("E.g:S 101");
    }
  }
  else if (m == "d" || m == "datacrunch")
  {
    if (num != "") {

      long Start = millis();
      bool rt = DataCrunch(num);
      Serial.println("exited in " + String(millis() - Start) + "ms");
      Serial.println("with result " + (String)((rt) ? "True" : "False"));
    }
    else {
      Serial.println("you need to add a number");
      Serial.println("E.g:D 101");
    }
  }
  else if (m == "l" || m == "listoutput")
  {
    List();
  }
  else
  {
    Serial.println("unknown mode please use:");
    Serial.println("S-Speed,");
    Serial.println("D-DataCrunch,");
    Serial.println("L-ListOutput,");
    Serial.println("Example:");
    Serial.println("S 101");
  }
}

//---------------------------------------------------------------------------------------------------------1.2
void setup() {
  Serial.begin(115200);
}

void loop() {
  String i =  Serial_ReadLine();
  CMD(i);
}

//use Speed(String) to test a number the fastest way posable
//use DataCrunch(String) to test the proper way
//use List() to just find some prime numbers





























