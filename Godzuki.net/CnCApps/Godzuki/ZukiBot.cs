using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

namespace Godzuki
{
    public class ZukiBot
    {
        public static List<ZukiBot> portMaps = new List<ZukiBot>();
        SerialPort serial;

        public bool hasData { get { return curData != null && curData.Count > 0; } }
        public List<string> curData;
        private string localBfr;

        public ZukiBot()
        {
            serial = null;
            curData = new List<string>();
            localBfr = "";
            curData.Add("ZukiBot local object alive and well\n");
        }

        public void SelectPort( string portName ) {
            SelectPort(portName, 9600, Parity.None, StopBits.One, 8, Handshake.None);
        }
        public void SelectPort(string portName, int baud, Parity parity, StopBits sb, int bits, Handshake hs)
        {
            ShutDown();
            curData.Add("ZukiBot - opening serial port\n");
            serial = new SerialPort(portName);

            serial.BaudRate = baud;
            serial.Parity = parity;
            serial.StopBits = sb;
            serial.DataBits = bits;
            serial.Handshake = hs;

            serial.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);

            serial.Open();
            portMaps.Add(this);
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

        public void PostCommand(string cmdString)
        {
            if( serial != null && serial.IsOpen )
                serial.Write(cmdString);
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
            SerialPort sp = (SerialPort)sender;
            string indata = sp.ReadExisting();

            ZukiBot r = null;
            foreach( ZukiBot z in portMaps )
                if( z.serial == sp ) {
                    r = z;
                    break;
                }
            if (r != null )
            {
                r.localBfr += indata;
                // if some part of this signifies the end of the command, parse out everything else and add to list
                int cmdLoc = r.localBfr.IndexOf( "\n" );
                if( cmdLoc >= 0 ) {
                    string outStr = r.localBfr.Substring(0, cmdLoc+1);
                    if( cmdLoc+1 == r.localBfr.Length )
                        r.localBfr = "";
                    else
                        r.localBfr = r.localBfr.Substring(cmdLoc+1);
                    r.curData.Add( outStr );
                }
            }
        }

    }
}
