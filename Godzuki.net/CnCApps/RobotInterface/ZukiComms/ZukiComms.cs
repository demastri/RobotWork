using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

namespace Godzuki
{
    public class ZukiComms
    {
        public static List<ZukiComms> portMaps = new List<ZukiComms>();
        public static string[] GetPortNames() { return SerialPort.GetPortNames(); }
        
        SerialPort serial;

        public bool hasData { get { return curData != null && curData.Count > 0; } }
        public List<string> curData;
        private string localBfr;


        public ZukiComms()
        {
            serial = null;
            curData = new List<string>();
            localBfr = "";
            curData.Add("ZukiBot local object alive and well\n");
        }
        public bool isConnected { get { return serial.IsOpen;  } }
        public bool SelectPort(string portName)
        {
            return SelectPort(portName, 9600, Parity.None, StopBits.One, 8, Handshake.None, false);
        }
        public bool SelectPort(string portName, int rate )
        {
            return SelectPort(portName, rate, Parity.None, StopBits.One, 8, Handshake.None, false);
        }
        public bool SelectPort(string portName, int baud, Parity parity, StopBits sb, int bits, Handshake hs, bool setdtr)
        {
            ShutDown();
            curData.Add("ZukiBot - opening serial port\n");
            serial = new SerialPort(portName);

            serial.BaudRate = baud;
            serial.Parity = parity;
            serial.StopBits = sb;
            serial.DataBits = bits;
            serial.Handshake = hs;
            serial.DtrEnable = setdtr;

            try
            {
                serial.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);

                serial.Open();
                portMaps.Add(this);
                return serial.IsOpen;
            }
            catch
            {
                ShutDown();
                return false;
            }
        }
        public void ShutDown()
        {
            if (serial != null && serial.IsOpen)
            {
                curData.Add("ZukiBot - shutting down serial port\n");
                if (portMaps.Contains(this))
                    portMaps.Remove(this);
                serial.Close();
                serial = null;
            }
        }

        public bool PostCommand(gCommandObject cmdObj)
        {
            return PostCommand( cmdObj.ToString() );
        }
        public bool PostCommand(string cmdString)
        {
            curData.Add(">> "+cmdString);

            if (serial != null && serial.IsOpen)
            {
                if (cmdString.Length >= serial.WriteBufferSize - serial.BytesToWrite)
                    curData.Add("Overflow Here!!");
                serial.Write(cmdString);
                return true;
            }
            else
            {
                curData.Add("ZukiBot - could not post data - no port\n");
                return false;
            }
        }

        /// <summary>
        ///  intent is to collect characters here until we have a complete command/response
        ///  once that's true, write it to the curData list
        ///  whoever's listening is responsible for clearing this buffer...
        /// </summary>

        private static void DataReceivedHandler(
                    object sender,
                    SerialDataReceivedEventArgs e)
        {
            bool displayOnlyCommandObjects = false;
            bool displayOnlyResponseStrings = false;

            SerialPort sp = (SerialPort)sender;
            string indata = sp.ReadExisting();

            ZukiComms r = null;
            foreach( ZukiComms z in portMaps )
                if( z.serial == sp ) {
                    r = z;
                    break;
                }
            if (r != null)
            {
                r.localBfr += indata;

                bool done = false;
                while (!done)
                {
                    done = true;
                    // if some part of this signifies the end of the command, parse out everything else and add to list
                    int cmdStart = r.localBfr.IndexOfAny(new char[] { '!', '&' });
                    int cmdEnd = r.localBfr.IndexOfAny(new char[] { '#', '\n' }, (cmdStart < 0 ? 0 : cmdStart));
                    if (cmdEnd >= 0)
                    {
                        done = false;
                        //displayOnlyCommandObjects             f = display anything that comes through, t = display only valid command objects !& -> #\n
                        //displayOnlyResponseStrings            if above is f, doesn't matter, if t, then only display response objects  & -> #
                        if (!displayOnlyCommandObjects)
                        {
                            if (cmdStart >= 0)
                            {
                                if (cmdStart > 0)
                                    r.curData.Add(r.localBfr.Substring(0, cmdStart));
                                r.curData.Add(r.localBfr.Substring(cmdStart, (cmdEnd - cmdStart) + 1));
                            }
                            else
                                r.curData.Add(r.localBfr);
                        }
                        else
                            if (cmdStart >= 0 && (!displayOnlyResponseStrings || r.localBfr[cmdStart] == '&'))
                                r.curData.Add(r.localBfr.Substring(cmdStart, (cmdEnd - cmdStart) + 1));

                        if (cmdEnd + 1 == r.localBfr.Length)
                            r.localBfr = "";
                        else
                            r.localBfr = r.localBfr.Substring(cmdEnd + 1);
                    }
                }
            }
        }

    }
}
