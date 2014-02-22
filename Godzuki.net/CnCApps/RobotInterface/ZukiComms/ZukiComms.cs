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
        static DateTime lastSendTime;
        static System.Threading.Timer tmrThreadingTimer = null;

        SerialPort serial;

        public bool hasData { get { return curData != null && curData.Count > 0; } }
        public List<string> curData = new List<string>();
        private string localBfr;

        List<string> openCommands = new List<string>();
        public List<string> replyData = new List<string>();
        int curRetry = -1;
        int maxRetry = 3;
        TimeSpan maxAckWait = new TimeSpan(0, 0, 0, 0, 500);


        public ZukiComms()
        {
            serial = null;
            localBfr = "";
            curData.Add("ZukiBot local object alive and well\n");
            if (tmrThreadingTimer == null)
            {
                //Initialize the timer to not start automatically... 
                tmrThreadingTimer = new System.Threading.Timer(new System.Threading.TimerCallback(MonitorCommands), null, System.Threading.Timeout.Infinite, 100);
                //Manually start the timer... 
                tmrThreadingTimer.Change(0, 100);
                //Manually stop the timer... 
                //tmrThreadingTimer.Change(Timeout.Infinite, Timeout.Infinite);
            }
        }
        public bool isConnected { get { return serial.IsOpen; } }
        public bool SelectPort(string portName)
        {
            return SelectPort(portName, 9600, Parity.None, StopBits.One, 8, Handshake.None, false);
        }
        public bool SelectPort(string portName, int rate)
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
                {
                    portMaps.Remove(this);
                    if (portMaps.Count == 0)
                    {
                        tmrThreadingTimer.Change(System.Threading.Timeout.Infinite, System.Threading.Timeout.Infinite);
                        tmrThreadingTimer.Dispose();
                        tmrThreadingTimer = null;
                    }
                }
                serial.Close();
                serial = null;
            }
        }

        public bool PostCommand(gCommandObject cmdObj)
        {
            return PostCommand(cmdObj.ToString());
        }
        public bool PostCommand(string cmdString)
        {
            openCommands.Add(cmdString);
            return true;
        }

        // do this every 100 millis or so...
        static public void MonitorCommands(object state)
        {
            foreach (ZukiComms gz in portMaps)
            {
                gz.MonitorLocalCommands();
            }
        }

        public void MonitorLocalCommands()
        {
            bool retry = false;
            while (replyData.Count > 0) // see if i have an ack to an existing command
            {
                string s = replyData[0];
                Godzuki.gCommandObject cmdObj = Godzuki.gCommandObject.FromString(s);
                replyData.RemoveAt(0);
                if (cmdObj != null) // not a command object
                {
                    curData.Add(s);
                }
                else
                {
                    if (curRetry >= 0)
                    {
                        Godzuki.gCommandObject cmdRef = Godzuki.gCommandObject.FromString(openCommands[0]);

                        if (cmdObj.isReply && cmdObj.commandID == cmdRef.commandID)  // ok - it's a reply
                        {
                            if (cmdObj.rtnStatus == Godzuki.ZukiCommands.GLOBAL_COMMAND_STATUS_OK)
                            {
                                // ok - this command is done, pass it back
                                curRetry = -1;
                                openCommands.RemoveAt(0);
                                curData.Add(s);
                            }
                            else
                            {
                                // by definition it must be a failure
                                retry = true; // mark it to be sent again
                            }
                        }
                    }
                }
            }

            // ok - dealt with replies, now let's see about outgoing

            // turn a timeout into a retry request here
            if (DateTime.Now - lastSendTime > maxAckWait)
                retry = true;

            if (curRetry < 0 && openCommands.Count == 0)    // nothing to do
                return;

            if ((curRetry < 0 || retry) && openCommands.Count > 0)    // command to send
            {
                if (serial != null && serial.IsOpen)
                {
                    if (++curRetry >= maxRetry)
                    {
                        //curData.Add("XX-Max retries exceeded!!" + openCommands[0]);
                        Godzuki.gCommandObject failReply = Godzuki.gCommandObject.FromString(openCommands[0]);
                        failReply.isReply = true;
                        failReply.rtnStatus = Godzuki.ZukiCommands.GLOBAL_COMMAND_STATUS_FAIL;
                        failReply.payloadData = "Timeout".ToCharArray();
                        failReply.payloadSize = "Timeout".Length;

                        curData.Add(failReply.ToString());
                        curRetry = -1;
                        openCommands.RemoveAt(0);
                        return;
                    }
                    curData.Add(">> " + openCommands[0]);
                    if (openCommands[0].Length >= serial.WriteBufferSize - serial.BytesToWrite)
                        curData.Add("Overflow Here!!");
                    serial.Write(openCommands[0]);
                    lastSendTime = DateTime.Now;
                    curRetry++;
                    retry = false;
                    return;
                }
                else
                {
                    curData.Add("ZukiBot - could not post data - no port\n");
                    return;
                }
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
            foreach (ZukiComms z in portMaps)
                if (z.serial == sp)
                {
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
