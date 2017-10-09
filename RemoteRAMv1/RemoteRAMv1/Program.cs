using System;
using System.IO.Ports;
using System.Threading;
using System.Text;
namespace RemoteRAMv_1
{
    class Program
    {
        const int ConnectionTimeOutRetryTime = 2000;
        const int ReadTimeout = 2000;
        const int BitRate = 115200;
        const string RRPort = "COM3";
        static readonly Encoding DEV_Encoding = Encoding.ASCII;//Arduino
        static readonly byte[] HELO = new byte[] { 0x48,0x45,0x4c,0x4f };
        static SerialPort DEV;
        static byte[] MEM;
        static bool DEBUG;

        static void Main(string[] args)
        {

            if (args.Length > 0 && args[0] == "-v") DEBUG = true; else DEBUG = false;
            DEBUG = true;

            Console.Title = "RemoteRAM v1 Beta PreRelease";
            Console.WriteLine("Connecting to Port (" + RRPort + ")");
            DEV = new SerialPort(RRPort, BitRate);//open port
            DEV.Encoding = DEV_Encoding;
            DEV.Open();
            Connect();
            WORK();
            Console.Write("Press any Enter to close console:");
            Console.ReadKey();
        }

        //main loop
        static void WORK()
        {
            bool Running = true;
            while(DEV.IsOpen && Running)
            {
                if (DEV.BytesToRead > 0)
                {
                    byte Flag = ReadByte();
                    switch(Flag)
                    {
                        case (255)://END
                            Console.WriteLine("Closing by DEV");
                            DEV.Close();
                            Running = false;
                            //Write(0);
                            break;
                        case (166)://readInt
                            short addr=BitConverter.ToInt16(Read(2), 0);
                            ReadInt(addr);
                            break;
                        case (167)://setInt
                            short Addr, value;
                            byte[] input = Read(4);
                            Addr = BitConverter.ToInt16(input, 0);
                            value = BitConverter.ToInt16(input, 2);
                            SetInt(Addr, value);
                            //Write(0);
                            break;
                        case (11)://Alloc
                            AllocNewInt();
                            //Write(0);
                            break;

                        case (10)://say
                            Console.WriteLine("DEV Said:"+DEV.ReadLine());
                            //Write(0);
                            break;

                        default:
                            Console.WriteLine("unknown Flag " + Flag);
                            break;
                    }
                }
            }
            if (Running)
                Console.WriteLine("DEV Disconnect, Closing");
        }

        static void AllocNewInt()
        {
            if (DEBUG)
                Console.WriteLine("Alloc Creating New Address");
            Array.Resize<byte>(ref MEM, MEM.Length + 2);
        }
        static void SetInt(short Address,short NewValue)
        {
            if (DEBUG)
                Console.WriteLine("Setting " + Address + " to " + NewValue);
            Array.Copy(BitConverter.GetBytes(NewValue), 0, MEM, Address * 2, 2);
            //Set Value
        }
        static void ReadInt(short Address)
        {
            if (DEBUG)
                Console.WriteLine($"Echoing *{Address} = '{BitConverter.ToInt16(MEM, Address * 2)}'");
            byte[] buf = new byte[] { MEM[Address * 2], MEM[(Address * 2) + 1] };
            Write(buf);
        }

        static byte[] Read(int count)
        {
            if(DEBUG)
                Console.WriteLine("Reading " + count + " bytes");
            byte[] buffer = new byte[count];
            DateTime TMR = DateTime.Now;
            while (DEV.BytesToRead < count && DateTime.Now - TMR < TimeSpan.FromMilliseconds(ReadTimeout));
            if (DEV.BytesToRead < count)
            {
                Console.WriteLine("Failed to read");
                Console.WriteLine("BUFFER=" + DEV_Encoding.GetString(buffer));
                return new byte[count];//return 0n
            }
            else
            {
                DEV.Read(buffer, 0, count);
                return buffer;
            }
        }
        static byte ReadByte()
        {
            return Read(1)[0];
        }

        static void Write(byte[] x)
        {
            if (DEBUG)
                Console.WriteLine("Writing "+x.Length+" bytes");
            DEV.Write(x, 0, x.Length);
        }
        static void Write(byte x)
        {
            if (DEBUG)
                Console.WriteLine("Writing 1 byte");
            byte[] buf = new byte[] { x };
            DEV.Write(buf, 0, 1);
        }

        static void Connect()
        {
            try
            {
                Write(HELO);//say hi
                MEM = new byte[0];//de nullify and reset
                byte[] HL = Read(HELO.Length);

                for (int x = 0; x < HL.Length; x++)
                {
                    if (HELO[x] != HL[x])
                    {
                        Console.WriteLine("HELO Failed");
                        Console.WriteLine("Retrying in " + ConnectionTimeOutRetryTime);
                        Thread.Sleep(ConnectionTimeOutRetryTime);
                        Connect();
                        break;
                    }
                }
            }
            catch(Exception e)
            {
                Console.WriteLine("Exception. Retrying in "+ ConnectionTimeOutRetryTime);
                Thread.Sleep(ConnectionTimeOutRetryTime);
                Connect();
            }
        }
    }
}
