using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Godzuki
{
    public class gCommandObject
    {
        public gCommandObject(int srcdev, int srcinst, int dev, int inst, int cmd, int param, long paySize, char[] payData)
        {
            Init(srcdev, srcinst, dev, inst, cmd, param, paySize, payData);
        }

        public gCommandObject(int srcdev, int srcinst, int dev, int inst, int cmd, int param)
        {
            Init(srcdev, srcinst, dev, inst, cmd, param, 0, null);
        }
        public gCommandObject(int srcdev, int srcinst, int dev, int inst, int cmd)
        {
            Init(srcdev, srcinst, dev, inst, cmd, -1, 0, null);
        }

        public gCommandObject(int dev, int inst, int cmd, int param)
        {
            Init(-1, -1, dev, inst, cmd, param, 0, null);
        }

        public gCommandObject(int dev, int inst, int cmd)
        {
            Init(-1, -1, dev, inst, cmd, -1, 0, null);
        }

        public gCommandObject()
        {
            Init(-1, -1, -1, -1, -1, -1, 0, null);
        }

        private void Init(int srcdev, int srcinst, int dev, int inst, int cmd, int param, long paySize, char[] payData)
        {
            sourceDeviceID = srcdev;
            sourceInstanceID = srcinst;
            targetDeviceID = dev;
            targetInstanceID = inst;
            commandID = cmd;
            parameter = param;
            payloadSize = paySize;
            payloadData = payData;
            isReply = false;
            isLocal = true;

        }

        public static gCommandObject IncomingCommand(string s)
        {
            if (s.Length == 18 && s[0] == '!' && s[17] == '#')
            {
                gCommandObject possCmd = new gCommandObject();
                // ###
            }
            return null;
        }

        public int targetDeviceID;
        public int targetInstanceID;
        public int commandID;
        public int parameter;

        public int sourceDeviceID;
        public int sourceInstanceID;
        public bool isReply;
        public bool isLocal;

        //gCommandObject *InitReply( unsigned char status, long paySize, void *payData );
        public void print() { Console.WriteLine( ToString() ); }

        public uint rtnStatus;
        public long payloadSize;
        public char[] payloadData;

        public string ToString()
        {
            if( !isReply )
                return "!" +
                    sourceDeviceID.ToString("D2") +
                    sourceInstanceID.ToString("D2") +
                    targetDeviceID.ToString("D2") +
                    targetInstanceID.ToString("D2") +
                    commandID.ToString("D2") +
                    parameter.ToString("D6") +
                    "#";
            return "&" +
                sourceDeviceID.ToString("D2") +
                sourceInstanceID.ToString("D2") +
                targetDeviceID.ToString("D2") +
                targetInstanceID.ToString("D2") +
                commandID.ToString("D2") +
                rtnStatus.ToString("D2") +
                payloadSize.ToString("D4") +
                payloadData +
                "#";
        }

        //virtual char *Serialize();
    }
}
