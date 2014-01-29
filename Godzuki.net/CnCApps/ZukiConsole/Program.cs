using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ZukiConsole
{
    class Program
    {
        static void receiveResponses() {
            System.Console.WriteLine("\nIn Handler\n");
        }

        static void Main(string[] args)
        {
            Godzuki.ZukiBot gz = new Godzuki.ZukiBot();
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
                    case 'r':
                        System.Console.WriteLine("\nrouting some kind of command...");
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
