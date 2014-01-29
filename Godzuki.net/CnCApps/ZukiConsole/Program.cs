using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ZukiConsole
{
    class Program
    {
        static Godzuki.ZukiBot gz = new Godzuki.ZukiBot();

        static void receiveResponses(int cmd, int status, int size, String data)
        {
            if (cmd == 99)
            { // GLOBAL_COMMAND_BROADCAST )
                System.Console.WriteLine("\nasked to rebroadcast this string - <"+data+">\n");
                gz.PostCommand(data);
            }
            else
                System.Console.WriteLine("\nIn random Handler\n");
        }

        static void Main(string[] args)
        {
            bool done = false;
            char c;
            DateTime shutTime = DateTime.MinValue;

            ZukiProxy.SensorBot.HelloWorld();
            ZukiProxy.SensorBot.setup();
            ZukiProxy.SensorBot.setClientHandler(Program.receiveResponses);

            gz.PostCommand("f\n");
            
            while (!done)
            {
                bool newCmd = false;
                string roughCmds = "crsoxdDkgfLR";

                switch ((c = readCommand()))
                {
                    case 'p': // Initialize a zuki object
                        gz.SelectPort("COM2");
                        newCmd = true;
                        break;
                    case '!':
                        System.Console.WriteLine("\nExiting - shutting down zukibot");
                        gz.ShutDown();
                        shutTime = DateTime.Now;
                        break;
                    case 'q':
                        System.Console.WriteLine("\nrouting some kind of remote command...");
                        ZukiProxy.SensorBot.routeCommand(2);
                        break;
                    case 'z':
                        System.Console.WriteLine("\ntrying to do a remote sweep...");
                        ZukiProxy.SensorBot.routeCommand(3);
                        break;
                    case 'r':
                        System.Console.WriteLine("\nrouting some kind of local command...");
                        ZukiProxy.SensorBot.routeCommand(1);
                        break;
                    default:
                        while(gz.hasData)
                        {
                            string s = gz.curData[0];
                            gz.curData.RemoveAt(0);
                            Console.Write("\n\n"+s);
                            newCmd = true;
                        }
                        break;
                }
                if (roughCmds.IndexOf(c) >= 0)
                {
                    if( c == 'L' )
                        gz.PostCommand("g90");
                    else if (c == 'R')
                        gz.PostCommand("g135");
                    else
                        gz.PostCommand(c + "100");
                    newCmd = true;
                }
                if (newCmd)
                    System.Console.Write("\nPosted Command or Received New Data - Enter Next Command:");
                TimeSpan waitTime = DateTime.Now - shutTime;
                if (shutTime != DateTime.MinValue && waitTime.Seconds >= 2)
                    done = true;

                ZukiProxy.SensorBot.loop();
            }
        }
        static char readCommand()
        {
            if (System.Console.KeyAvailable )
                return System.Console.ReadKey().KeyChar;
            return '\0';
        }
    }
}
