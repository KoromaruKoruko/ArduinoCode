using System.IO.Ports;
using System;
using System.Threading;

namespace BDWiredCMD
{
    public static class ConsoleHelpers
    {
        public static readonly string[] ConsoleColors = new string[]
        {
            "Black", "Blue", "Cyan", "DarkBlue", "DarkCyan",
            "DarkGray", "DarkGreen", "DarkMagenta", "DarkRed",
            "DarkYellow", "Gray", "Green", "Magenta", "Red",
            "White", "Yellow"
        };

        public static ConsoleColor Str2CC(string color)
        {
            switch (color.ToLower())
            {
                case ("black"):
                    return ConsoleColor.Black;

                case ("blue"):
                    return ConsoleColor.Blue;

                case ("cyan"):
                    return ConsoleColor.Cyan;

                case ("darkblue"):
                    return ConsoleColor.DarkBlue;

                case ("darkcyan"):
                    return ConsoleColor.DarkCyan;

                case ("darkgray"):
                    return ConsoleColor.DarkGray;

                case ("darkgreen"):
                    return ConsoleColor.DarkGreen;

                case ("darkmagenta"):
                    return ConsoleColor.DarkMagenta;

                case ("darkred"):
                    return ConsoleColor.DarkRed;

                case ("darkyellow"):
                    return ConsoleColor.DarkYellow;

                case ("green"):
                    return ConsoleColor.Green;

                case ("magenta"):
                    return ConsoleColor.Magenta;

                case ("red"):
                    return ConsoleColor.Red;

                case ("white"):
                    return ConsoleColor.White;

                case ("yellow"):
                    return ConsoleColor.Yellow;

                default:
                    throw new ArgumentException("no translation");
            }
        }
        public static string CC2Str(ConsoleColor x)
        {
            switch (x)
            {
                case (ConsoleColor.Black):
                    return "Black";

                case (ConsoleColor.Blue):
                    return "Blue";

                case (ConsoleColor.Cyan):
                    return "Cyan";

                case (ConsoleColor.DarkBlue):
                    return "DarkBlue";

                case (ConsoleColor.DarkCyan):
                    return "DarkCyan";

                case (ConsoleColor.DarkGray):
                    return "DarkGray";

                case (ConsoleColor.DarkGreen):
                    return "DarkGreen";

                case (ConsoleColor.DarkMagenta):
                    return "DarkMagenta";

                case (ConsoleColor.DarkRed):
                    return "DarkRed";

                case (ConsoleColor.DarkYellow):
                    return "DarkYellow";

                case (ConsoleColor.Green):
                    return "Green";

                case (ConsoleColor.Magenta):
                    return "Magenta";

                case (ConsoleColor.Red):
                    return "Red";

                case (ConsoleColor.White):
                    return "White";

                case (ConsoleColor.Yellow):
                    return "Yellow";

                default:
                    throw new ArgumentException("no translation");
            }
        }
    }//ripped from BDlib
    class Program
    {
        private static string PORT = "COM3";
        private static int ReConnectTimer = 100;

        private static SerialPort Arduino;

        static void Main(string[] args)
        {
            while (true)
            {
                Connect();
                CMD();
                Console.WriteLine("Connection Lost!");
                Console.WriteLine($"Retrying in {ReConnectTimer}");
                if (ReConnectTimer != -1)
                    Thread.Sleep(ReConnectTimer);
                else EXIT();
            }
        }

        private static void EXIT()
        {
            Arduino.BaseStream.WriteByte(68);//write disconnect
            Environment.Exit(0);//0 = EXIT_SUCCESS
        }

        private static void CMD()
        {
            while (Arduino.IsOpen)
            {
                string Command = Console.ReadLine();
                string com = "";
                string[] args;
                if (Command.Contains(" "))
                {
                    com = Command.Substring(0, Command.IndexOf(' ')).ToLower();
                    string[] tmp = Command.Substring(Command.IndexOf(' ') + 1).Split(new char[] { ' ' });
                    if (tmp.Length > 10)
                        args = new string[tmp.Length];
                    else
                        args = new string[10];
                    Array.Copy(tmp, args, tmp.Length);
                }
                else
                {
                    com = Command;
                    args = new string[10];
                }

                //remove cap sensitivity
                for (int x = 0; x < args.Length; x++)
                {
                    if (args[x] != null)
                        args[x] = args[x].ToLower();
                }

                switch (com)
                {
                    case ("loop"):
                        switch(args[0])
                        {
                            case ("create"):
                            case ("startlog"):
                                StartLoopLog();
                                break;

                            case ("done"):
                            case ("endlog"):
                                EndLoopLog();
                                break;

                            case ("startloop"):
                                StartLoop();
                                break;

                            case ("stoploop"):
                                StopLoop();
                                break;

                            case ("max"):
                                getMaxLogEntrys();
                                break;

                            default:
                                Console.WriteLine($"invalud arg, '{args[0]}' is not a part of loop");
                                break;
                        }
                        break;


                    case ("wait"):
                        if (args[0] == "100" || args[0] == "100ms")
                            SendWait100();
                        else if (args[0] == "1000" || args[0] == "1000ms")
                            SendWait1000();
                        else
                            Console.WriteLine("invalud val,values ate 1000ms or 100ms");
                        break;

                    case ("set"):
                        switch (args[0])
                        {
                            case ("pin"):
                            case ("pinstate"):
                                int pin = -1;
                                int.TryParse(args[1], out pin);

                                if (pin > 0 && pin <= 11)
                                {
                                    if (args[2] == "on" || args[2] == "true")
                                        SetPinStateTo(pin, true);
                                    else if (args[2] == "off" || args[2] == "false")
                                        SetPinStateTo(pin, false);
                                    else
                                        Console.WriteLine($"Unknown value '{args[2]}'");

                                }
                                else Console.WriteLine("Invlaid pin num, Pins are 1-11");
                                break;

                            case ("pinmode"):
                                int _pin = -1;
                                int.TryParse(args[1], out _pin);

                                if (_pin > 0 && _pin <= 11)
                                {
                                    if (args[2] == "input" || args[2] == "true")
                                        SetPinMode(_pin, true);
                                    else if (args[2] == "output" || args[2] == "false")
                                        SetPinMode(_pin, false);
                                    else
                                        Console.WriteLine($"Unknown value '{args[2]}'");

                                }
                                else Console.WriteLine("Invlaid pin num, Pins are 1-11");
                                break;

                            case ("recontime"):
                                int newtime = 0;
                                int.TryParse(args[1], out newtime);
                                if (newtime > -2 || newtime < 20001)
                                    ReConnectTimer = newtime;
                                else
                                    Console.WriteLine("unknown value, values are -1-20000. -1 is off no reconnect");
                                break;

                            case ("consolecolor"):
                                switch (args[1])
                                {
                                    case ("fore"):
                                        try
                                        {
                                            Console.ForegroundColor = ConsoleHelpers.Str2CC(args[2]);//throw invalidargs eception when invalid color str
                                        }
                                        catch
                                        {
                                            Console.Write("Invalid value, values are");
                                            for (int x = 0; x < ConsoleHelpers.ConsoleColors.Length; x++)
                                                Console.Write($"{ConsoleHelpers.ConsoleColors[x]}, ");
                                        }
                                        break;
                                    case ("back"):
                                        try
                                        {
                                            Console.BackgroundColor = ConsoleHelpers.Str2CC(args[2]);//throw invalidargs eception when invalid color str
                                        }
                                        catch
                                        {
                                            Console.Write("Invalid value, values are");
                                            for (int x = 0; x < ConsoleHelpers.ConsoleColors.Length; x++)
                                                Console.Write($"{ConsoleHelpers.ConsoleColors[x]}, ");
                                        }
                                        break;
                                    default:
                                        Console.WriteLine("invaluid option, options are fore back");
                                        break;
                                }
                                break;

                            case ("help"):
                                //help text goes here
                                Console.WriteLine("this is a PreRelaseBuild help text will be added in the ReleaseBuild");
                                break;

                            default:
                                Console.WriteLine($"invalid var, '{args[0]}' is not a part of set");
                                break;
                        }
                        break;

                    case ("get"):
                        switch (args[0])
                        {
                            case ("pin"):
                            case ("pinstate"):
                                int pin = -1;
                                int.TryParse(args[1], out pin);

                                if (pin > 0 && pin <= 11)
                                {
                                    if (RequestPinState(pin))
                                        Console.WriteLine("pin is active");
                                    else
                                        Console.WriteLine("pin is inactive");
                                }
                                else Console.WriteLine("Invlaid pin num, Pins are 1-11");
                                break;

                            case ("pinmode"):
                                int _pin = -1;
                                int.TryParse(args[1], out _pin);

                                if (_pin > 0 && _pin <= 11)
                                {
                                    if (RequestPinMode(_pin))
                                        Console.WriteLine("pin is set to input mode");
                                    else
                                        Console.WriteLine("pin is set to output mode");
                                }
                                else Console.WriteLine("Invlaid pin num, Pins are 1-11");
                                break;

                            case ("consolecolor"):
                                switch (args[1])
                                {
                                    case ("fore"):
                                        Console.WriteLine($"ForegroundColor={ConsoleHelpers.CC2Str(Console.ForegroundColor)}");
                                        break;
                                    case ("back"):
                                        Console.WriteLine($"BackgroundColor={ConsoleHelpers.CC2Str(Console.BackgroundColor)}");
                                        break;
                                    default:
                                        Console.WriteLine("invaluid option, options are fore back");
                                        break;
                                }
                                break;

                            case ("recontime"):
                                Console.WriteLine($"reconnectTime={ReConnectTimer}");
                                break;

                            case ("maxloopentrys"):
                            case ("maxloop"):
                                getMaxLogEntrys();
                                break;

                            default:
                                Console.WriteLine($"invalid var, '{args[0]}' is not a part of get");
                                break;
                        }
                        break;

                    case ("reconn"):
                    case ("reconnect"):
                        Connect();
                        break;

                    case ("reset"):
                    case ("reboot"):
                        SendReset();
                        break;

                    case ("exit"):
                    case ("quit"):
                        EXIT();
                        break;

                    default:
                        Console.WriteLine("Command NotFound - No Translation found");
                        break;
                }
            }
        }

        private static void Connect()
        {
            Console.WriteLine($"Atempting to connect to Arduino on {PORT}");
            try
            {
                Arduino = new SerialPort(PORT, 9600);
                Arduino.Open();
                Arduino.BaseStream.WriteByte(69);//on connect
                SendPing();
                Console.WriteLine("Connected");
            }
            catch
            {
                Console.WriteLine("Arduino not Found/not active");
                Console.Write("Port:");
                PORT = Console.ReadLine();
                Connect();
            }
        }

        static bool RequestPinState(int Pin)
        {
            //21 offset due to SetPinState
            byte o = byte.Parse((Pin+21).ToString());
            Arduino.Write(new byte[1] { o }, 0, 1);
            Thread.Sleep(10);
            if (Arduino.BaseStream.ReadByte() > 1)
                return true;
            else
                return false;
        }

        static void SetPinMode(int Pin, bool Mode)
        {
            byte BTS = byte.Parse(((Pin*2) + 31).ToString());
            if (Mode)
                BTS++;
            Arduino.BaseStream.WriteByte(BTS);
        }

        static bool RequestPinMode(int Pin)
        {
            byte o = byte.Parse((Pin + 54).ToString());
            Arduino.Write(new byte[1] { o }, 0, 1);
            Thread.Sleep(10);
            if (Arduino.BaseStream.ReadByte() > 1)
                return true;
            else
                return false;
        }

        static void SetPinStateTo(int Pin, bool value)
        {
            //pin is 1-11 max num is 20
            byte BTS = byte.Parse(((Pin * 2) - 2).ToString());
            if (value)
                BTS++;
            Arduino.BaseStream.WriteByte(BTS);
        }

        static void StartLoopLog()
        {
            Arduino.BaseStream.WriteByte(75);
        }

        static void EndLoopLog()
        {
            Arduino.BaseStream.WriteByte(76);
        }

        static void StopLoop()
        {
            Arduino.BaseStream.WriteByte(77);
        }

        static void StartLoop()
        {
            Arduino.BaseStream.WriteByte(78);
        }

        static void getMaxLogEntrys()
        {
            Arduino.BaseStream.WriteByte(81);
            Thread.Sleep(10);
            int amount = Arduino.BaseStream.ReadByte();
            if (amount != -1)
            {
                Console.WriteLine($"Max number of loop Entrys is {amount}");
            }else
            {
                Console.WriteLine($"Failed try ping to make sure the arduino is still there");
            }
        }

        static void SendWait100()
        {
            Arduino.BaseStream.WriteByte(79);
        }

        static void SendWait1000()
        {
            Arduino.BaseStream.WriteByte(80);
        }

        static void SendReset()
        {
            Arduino.BaseStream.WriteByte(70);
        }

        static void SendPing()
        {
            Arduino.BaseStream.WriteByte(66);
            Thread.Sleep(10);
            if(Arduino.ReadByte() != 67)
            {
                Console.WriteLine("ping failed try to reconnect to the arduino");
            }
            else
            {
                Console.WriteLine("Ping came back just fine");
            }
        }
    }
}
