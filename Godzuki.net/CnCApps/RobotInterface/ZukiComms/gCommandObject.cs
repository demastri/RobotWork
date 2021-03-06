﻿using System;
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

        public gCommandObject(int srcdev, int srcinst, int dev, int inst, int cmd, long param)
        {
            Init(srcdev, srcinst, dev, inst, cmd, param, 0, null);
        }
        public gCommandObject(int srcdev, int srcinst, int dev, int inst, int cmd)
        {
            Init(srcdev, srcinst, dev, inst, cmd, -1, 0, null);
        }

        public gCommandObject(int dev, int inst, int cmd, long param)
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

        private void Init(int srcdev, int srcinst, int dev, int inst, int cmd, long param, long paySize, char[] payData)
        {
            sourceDeviceID = srcdev;
            sourceInstanceID = srcinst;
            targetDeviceID = dev;
            targetInstanceID = inst;
            commandID = cmd;
            parameter = param;
            payloadSize = paySize;
            payloadData = (char[])(payData == null ? null : payData.Clone());
            isReply = false;
            isLocal = true;

        }

        public static gCommandObject FromString(string s)
        {
            if (s.Length == 20 && s[0] == '!' && s[19] == '#')
            {
                try
                {
                    gCommandObject possCmd = new gCommandObject();
                    possCmd.sourceDeviceID = Convert.ToInt16(s.Substring(1, 2));
                    possCmd.sourceInstanceID = Convert.ToInt16(s.Substring(3, 2));
                    possCmd.targetDeviceID = Convert.ToInt16(s.Substring(5, 2));
                    possCmd.targetInstanceID = Convert.ToInt16(s.Substring(7, 2));
                    possCmd.commandID = Convert.ToInt16(s.Substring(9, 2));
                    possCmd.rtnStatus = 0;
                    possCmd.parameter = Convert.ToInt64(s.Substring(11, 8));
                    possCmd.payloadSize = 0;
                    possCmd.payloadData = null;
                    possCmd.isReply = false;
                    possCmd.isLocal = false;
                    return possCmd;
                }
                catch (FormatException)
                {
                    return null;
                }
            }
            if (s.Length > 10 && s[0] == '&')
            {
                gCommandObject possCmd = new gCommandObject();
                possCmd.sourceDeviceID = Convert.ToInt16(s.Substring(1, 2));
                possCmd.sourceInstanceID = Convert.ToInt16(s.Substring(3, 2));
                possCmd.targetDeviceID = Convert.ToInt16(s.Substring(5, 2));
                possCmd.targetInstanceID = Convert.ToInt16(s.Substring(7, 2));
                possCmd.commandID = Convert.ToInt16(s.Substring(9, 2));
                possCmd.parameter = -1;
                possCmd.rtnStatus = Convert.ToUInt16(s.Substring(11, 2));
                possCmd.payloadSize = Convert.ToInt16(s.Substring(13, 4));
                possCmd.payloadData = s.Substring(17, (int)possCmd.payloadSize).ToCharArray();
                possCmd.isReply = true;
                possCmd.isLocal = false;
                if (s.Substring(17 + (int)possCmd.payloadSize, 1) == "#")
                    return possCmd;
            }
            return null;
        }

        public int targetDeviceID;
        public int targetInstanceID;
        public int commandID;
        public long parameter;

        public int sourceDeviceID;
        public int sourceInstanceID;
        public bool isReply;
        public bool isLocal;

        //gCommandObject *InitReply( unsigned char status, long paySize, void *payData );
        public void print() { Console.WriteLine(ToString()); }

        public uint rtnStatus;
        public long payloadSize;
        public char[] payloadData;

        override public string ToString()
        {
            if (!isReply)
                return "!" +
                    sourceDeviceID.ToString("D2") +
                    sourceInstanceID.ToString("D2") +
                    targetDeviceID.ToString("D2") +
                    targetInstanceID.ToString("D2") +
                    commandID.ToString("D2") +
                    parameter.ToString(parameter < 0 ? "D7" : "D8") +
                    "#";
            return "&" +
                sourceDeviceID.ToString("D2") +
                sourceInstanceID.ToString("D2") +
                targetDeviceID.ToString("D2") +
                targetInstanceID.ToString("D2") +
                commandID.ToString("D2") +
                rtnStatus.ToString("D2") +
                payloadSize.ToString("D4") +
                (payloadData == null ? "" : new string(payloadData)) +
                "#";
        }

        //virtual char *Serialize();
    }
}
