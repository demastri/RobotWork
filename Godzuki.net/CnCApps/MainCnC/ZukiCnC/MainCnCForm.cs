﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ZukiCnC
{
    public partial class MainCnCForm : Form
    {
        Dictionary<DateTime, Godzuki.gCommandObject> openCommands;

        bool isConnected;
        bool isExecutingGoal;
        Godzuki.ZukiComms gz = new Godzuki.ZukiComms();

        public MainCnCForm()
        {
            InitializeComponent();

            isConnected = isExecutingGoal = false;
            openCommands = new Dictionary<DateTime, Godzuki.gCommandObject>();
            InitializeGoals();
            RefreshPortData_Click(null, null);

            MessageLoopTimer.Start();
        }
        private void InitializeGoals()
        {
            goalList.Items.Clear();
            goalList.Items.Add("None");
            goalList.Items.Add("Sweep");
            goalList.Items.Add("Sweep/Map");
            goalList.Items.Add("Turn");
            goalList.Items.Add("Turn/Sweep/Map");
            goalList.SelectedIndex = 0;
        }

        private void RefreshPortData_Click(object sender, EventArgs e)
        {
            string[] ports = Godzuki.ZukiComms.GetPortNames();
            string refText = availablePorts.SelectedText;
            availablePorts.Items.Clear();
            if (ports.Length == 0)
                availablePorts.Items.Add("None");
            else
                foreach( string s in ports )
                    availablePorts.Items.Add(s);
            if (refText != "" && ports.Contains(refText))
                availablePorts.SelectedText = refText;
            else
                availablePorts.SelectedIndex = 0;
            if (availableInstances.SelectedIndex < 0)
                availableInstances.SelectedIndex = 0;
        }

        private void ConnectToRobotButton_Click(object sender, EventArgs e)
        {
            if (!isConnected)
            {
                if (gz.SelectPort((string)availablePorts.SelectedItem))
                {
                    ConnectToRobotButton.Text = "Connecting";
                    isConnected = true;
                    Godzuki.gCommandObject cmdObj = new Godzuki.gCommandObject(
                        Godzuki.ZukiCommands.CNC_APP_DEVICE_ID, 1,
                        Godzuki.ZukiCommands.GODZUKI_SENSOR_PLATFORM_DEVICE_ID, 1,
                        Godzuki.ZukiCommands.COMMAND_ID_GLOBAL_REQUEST_STATUS);
                    gz.PostCommand( cmdObj );
                    // somehow we need to set a timer that expires and revokes the connectedness if we haven't gotten a reply
                    openCommands.Add( DateTime.Now.Add(new TimeSpan(0,0,2)), cmdObj );
                }
                else
                {
                    MessageLoopTimer.Stop();

                    gz.curData.Add("Couldn't open port, please check connections...");
                    MessageBox.Show("Couldn't open port, please check connections...");
                
                    MessageLoopTimer.Start();
                }
            }
            else
            {
                ConnectToRobotButton.Text = "Connect";
                gz.ShutDown();
                isConnected = false;
            }
        }

        private void LogText(string s)
        {
            sessionLog.AppendText(s + Environment.NewLine);
        }
        private void MessageLoopTimer_Tick(object sender, EventArgs e)
        {

            while (gz.hasData)
            {
                // if cmd or response
                Godzuki.gCommandObject cmdObj = Godzuki.gCommandObject.FromString(gz.curData[0]);
                if (cmdObj != null)
                {
                    if (cmdObj.isReply)
                    {
                        DateTime thisKey = DateTime.MinValue;
                        Godzuki.gCommandObject openCmd = null;
                        foreach (DateTime dt in openCommands.Keys)
                        {
                            openCmd = openCommands[dt];
                            if (openCmd.sourceDeviceID == cmdObj.sourceDeviceID &&
                                openCmd.sourceInstanceID == cmdObj.sourceInstanceID &&
                                openCmd.targetDeviceID == cmdObj.targetDeviceID &&
                                openCmd.targetInstanceID == cmdObj.targetInstanceID &&
                                openCmd.commandID == cmdObj.commandID)
                            {
                                thisKey = dt;
                                break;  // ok, got it
                            }
                        }
                        if (thisKey != DateTime.MinValue)
                        {
                            LogText("<< " + gz.curData[0]);
                            openCommands.Remove(thisKey);
                            if (cmdObj.rtnStatus != Godzuki.ZukiCommands.GLOBAL_COMMAND_STATUS_OK)
                            {
                                string outString = cmdObj.rtnStatus.ToString();
                                if (cmdObj.payloadSize > 0) // ok - we got a status message as well;
                                    outString += " " + new string(cmdObj.payloadData);
                                MessageLoopTimer.Stop();
                                MessageBox.Show("Command " + openCmd.ToString() + " failed with status code <" + outString + ">");
                                MessageLoopTimer.Start();
                            }
                            else
                            {
                                // ok - it's a "valid" command
                                switch (cmdObj.targetDeviceID)
                                {
                                    case Godzuki.ZukiCommands.SERVO_DEVICE_ID:
                                        switch (cmdObj.commandID)
                                        {
                                            case Godzuki.ZukiCommands.COMMAND_ID_SERVO_SET_POSITION:
                                                LogText("ACK for Servo Set Position");  // issue read command
                                                GetServoPositionButton_Click(null, null);
                                                break;
                                            case Godzuki.ZukiCommands.COMMAND_ID_SERVO_READ_POSITION:
                                                CurrentServoPosition.Text = Convert.ToUInt16( new string( cmdObj.payloadData) ).ToString();
                                                LogText("ACK for Servo Read Position");
                                                break;
                                        }
                                        break;
                                    case Godzuki.ZukiCommands.GODZUKI_SENSOR_PLATFORM_DEVICE_ID:
                                        switch (cmdObj.commandID)
                                        {
                                            case Godzuki.ZukiCommands.COMMAND_ID_GLOBAL_REQUEST_STATUS:
                                                LogText("ACK for Global Status");  // issue read command
                                                ConnectToRobotButton.Text = "Connected";
                                                break;
                                        }
                                        break;
                                    default:
                                        LogText("don't quite know how to handle this response");
                                        break;
                                }

                            }
                        }
                    }
                }
                else
                {
                    LogText(gz.curData[0]);
                }
                gz.curData.RemoveAt(0);
            }
            // in any event, if there are commands that have timed out, message them here...
            List<DateTime> deadKeys = new List<DateTime>();
            foreach( DateTime dt in openCommands.Keys ) 
                if( DateTime.Now > dt )
                {
                    string cmd = openCommands[dt].ToString();
                    openCommands.Remove(dt);
                    MessageLoopTimer.Stop();
                    MessageBox.Show("Command Timeout: " + cmd);
                    MessageLoopTimer.Start();
                    break;
                }
        }

        private void GetServoPositionButton_Click(object sender, EventArgs e)
        {
            LogText("Request Servo Position");
            
            // at some point the command will return a response 
            // that will cause the display to update...
            Godzuki.gCommandObject cmdObj = new Godzuki.gCommandObject(
                Godzuki.ZukiCommands.CNC_APP_DEVICE_ID, 1,
                Godzuki.ZukiCommands.SERVO_DEVICE_ID, 1,
                Godzuki.ZukiCommands.COMMAND_ID_SERVO_READ_POSITION);

            // stringification is back into the comms object
            if (gz.PostCommand(cmdObj))
                openCommands.Add(DateTime.Now.Add(new TimeSpan(0, 0, 2)), cmdObj);
        }

        private void SetServoButton_Click(object sender, EventArgs e)
        {
            LogText("Setting Servo Position");
            int outVal = ServoTargetPos.Minimum + ServoTargetPos.Maximum - ServoTargetPos.Value;

            // at some point the command will return a response 
            // that will cause the display to update...
            Godzuki.gCommandObject cmdObj = new Godzuki.gCommandObject(
                Godzuki.ZukiCommands.CNC_APP_DEVICE_ID, 1,
                Godzuki.ZukiCommands.SERVO_DEVICE_ID, 1,
                Godzuki.ZukiCommands.COMMAND_ID_SERVO_SET_POSITION,
                outVal);

            // stringification is back into the comms object
            if (gz.PostCommand(cmdObj))
                openCommands.Add(DateTime.Now.Add(new TimeSpan(0, 0, 2)), cmdObj);
        }

        private void ServoTargetPos_DoubleClick(object sender, EventArgs e)
        {
            ServoTargetPos.Value = (ServoTargetPos.Minimum + ServoTargetPos.Maximum) / 2;
        }

        private void MotorSpeed_DoubleClick(object sender, EventArgs e)
        {
            if (MotorSpeed.Text == "Rev Speed")
                MotorSpeed.Text = "Fwd Speed";
            else
                MotorSpeed.Text = "Rev Speed";
        }

        private void SnapPictureButton_Click(object sender, EventArgs e)
        {
            LogText("Temporary - coopt for SD Test" );
            Godzuki.gCommandObject cmdObj = new Godzuki.gCommandObject(
                Godzuki.ZukiCommands.CNC_APP_DEVICE_ID, 1,
                Godzuki.ZukiCommands.SDCARD_DEVICE_ID, 1,
                Convert.ToInt16(SDcommandChoice.SelectedItem));

            // stringification is back into the comms object
            if (gz.PostCommand(cmdObj))
                openCommands.Add(DateTime.Now.Add(new TimeSpan(0, 0, 2)), cmdObj);

            return;
#if false
            LogText("Request Camera Image");
            // the command will return a response that will cause the display to update...
            Godzuki.gCommandObject cmdObj = new Godzuki.gCommandObject(
                Godzuki.ZukiCommands.CNC_APP_DEVICE_ID, 1,
                Godzuki.ZukiCommands.CAMERA_DEVICE_ID, 1,
                Godzuki.ZukiCommands.COMMAND_ID_CAMERA_SNAP_IMAGE );

            // stringification is back into the comms object
            if (gz.PostCommand(cmdObj))
                openCommands.Add(DateTime.Now.Add(new TimeSpan(0, 0, 10)), cmdObj);
#endif
        }

        private void ReadRangerButton_Click(object sender, EventArgs e)
        {
            LogText("Request Ranger Reading");
            // the command will return a response that will cause the display to update...
            Godzuki.gCommandObject cmdObj = new Godzuki.gCommandObject(
                Godzuki.ZukiCommands.CNC_APP_DEVICE_ID, 1,
                Godzuki.ZukiCommands.DISTANCE_SENSOR_DEVICE_ID, 1,
                Godzuki.ZukiCommands.COMMAND_ID_RANGER_READ_DISTANCE);

            // stringification is back into the comms object
            if (gz.PostCommand(cmdObj))
                openCommands.Add(DateTime.Now.Add(new TimeSpan(0, 0, 10)), cmdObj);
        }

        private void ClearCmdLogButton_Click(object sender, EventArgs e)
        {
            sessionLog.Clear();
        }
    }
}
