#region references
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
#endregion

namespace ZukiCnC
{
    public partial class MainCnCForm : Form
    {
        #region variable initialization
        Dictionary<DateTime, Godzuki.gCommandObject> openCommands;

        bool isConnected;

        bool isExecutingGoal;
        Godzuki.ZukiComms gz = new Godzuki.ZukiComms();
        System.Xml.XmlDocument goalDoc = null;
        System.Xml.XmlNodeList allGoalNodes = null;
        System.Xml.XmlNode currentGoalNode = null;
        int currentGoalIndex = -1;
        bool currentGoalStepMet;
        DateTime currentStepTimeout = DateTime.MinValue;
        Dictionary<string, string> goalParamTokens;
        string currentGoalCommand = "";
        #endregion

        #region initialization

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
            System.IO.DirectoryInfo rootDir = new System.IO.DirectoryInfo("../../../Goals");
            BuildGoalTree(goalList, rootDir, "");
            goalList.SelectedIndex = 0;
        }
        private void BuildGoalTree(ComboBox l, System.IO.DirectoryInfo thisDir, string curBase)
        {
            IEnumerable<System.IO.DirectoryInfo> kids = thisDir.EnumerateDirectories();
            foreach (System.IO.DirectoryInfo kidDir in kids)
            {
                BuildGoalTree(goalList, kidDir, curBase + kidDir.Name + "-");
            }
            IEnumerable<System.IO.FileInfo> kidf = thisDir.EnumerateFiles();
            foreach (System.IO.FileInfo kidFile in kidf)
            {
                string s = kidFile.Name;
                s = s.Substring(0, s.LastIndexOf('.'));
                goalList.Items.Add(curBase + s);
            }
        }
        #endregion

        #region Timer Response - needs to be cleaned up
        int tickCount = 0;

        private void MessageLoopTimer_Tick(object sender, EventArgs e)
        {
            // deal with incoming messages
            while (gz.hasData)
            {
                // if cmd or response
                Godzuki.gCommandObject cmdObj = Godzuki.gCommandObject.FromString(gz.curData[0]);
                if (cmdObj != null)
                {
                    if (cmdObj.isReply)
                    {
                        UpdateGoalState(cmdObj);

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
                                    case Godzuki.ZukiCommands.DISTANCE_SENSOR_DEVICE_ID:
                                        switch (cmdObj.commandID)
                                        {
                                            case Godzuki.ZukiCommands.COMMAND_ID_RANGER_READ_DISTANCE:
                                                Convert.ToUInt16(new string(cmdObj.payloadData)).ToString();    // distance in cm...
                                                if (currentGoalCommand == "Ranger/Read")
                                                    currentGoalStepMet = true;
                                                break;
                                        }
                                        break;
                                    case Godzuki.ZukiCommands.SERVO_DEVICE_ID:
                                        switch (cmdObj.commandID)
                                        {
                                            case Godzuki.ZukiCommands.COMMAND_ID_SERVO_SET_POSITION:
                                                LogText("ACK for Servo Set Position");  // issue read command
                                                GetServoPositionButton_Click(null, null);
                                                break;
                                            case Godzuki.ZukiCommands.COMMAND_ID_SERVO_READ_POSITION:
                                                CurrentServoPosition.Text = Convert.ToUInt16(new string(cmdObj.payloadData)).ToString();
                                                LogText("ACK for Servo Read Position");
                                                break;
                                        }
                                        break;
                                    case Godzuki.ZukiCommands.GODZUKI_SENSOR_PLATFORM_DEVICE_ID:
                                        switch (cmdObj.commandID)
                                        {
                                            case Godzuki.ZukiCommands.COMMAND_ID_GLOBAL_REQUEST_STATUS:
                                                LogText("ACK for Global Status");
                                                ConnectToRobotButton.Text = "Connected";
                                                if (currentGoalCommand == "Platform/Connect")
                                                    currentGoalStepMet = true;
                                                break;
                                        }
                                        break;
                                    case Godzuki.ZukiCommands.MOTOR_CONTROL_DEVICE_ID:
                                        switch (cmdObj.commandID)
                                        {
                                            case Godzuki.ZukiCommands.COMMAND_ID_MOTORCONTROL_PULL_SPEEDS:
                                                LogText("ACK for GetSpeeds");
                                                LeftMotorSpeed.Text = new string(cmdObj.payloadData).Substring(0, 4) + " mm/s";
                                                RightMotorSpeed.Text = new string(cmdObj.payloadData).Substring(4, 4) + " mm/s";
                                                break;
                                        }
                                        break;
                                    default:
                                        // don't have to do anything, just consume the ack...
                                        LogText("ACK for " + openCmd.ToString());  // issue read command
                                        //LogText("don't quite know how to handle this response");
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
            foreach (DateTime dt in openCommands.Keys)
                if (DateTime.Now > dt)
                {
                    string cmd = openCommands[dt].ToString();
                    openCommands.Remove(dt);
                    MessageLoopTimer.Stop();
                    MessageBox.Show("Command Timeout: " + cmd);
                    MessageLoopTimer.Start();
                    break;
                }
            if (isConnected && (++tickCount % 4) == 3)
            {
                Godzuki.gCommandObject newCmdObj = new Godzuki.gCommandObject(
                    Godzuki.ZukiCommands.CNC_APP_DEVICE_ID, 1,
                    Godzuki.ZukiCommands.MOTOR_CONTROL_DEVICE_ID, 1,
                    Godzuki.ZukiCommands.COMMAND_ID_MOTORCONTROL_PULL_SPEEDS);

                // stringification is back into the comms object
                if (gz.PostCommand(newCmdObj))
                    openCommands.Add(DateTime.Now.Add(new TimeSpan(0, 0, 2)), newCmdObj);
            }
            // and, finally, manage goal state here
            ManageGoalState();
        }
        #endregion

        #region hardware control - camera button is coopted for now

        #region robot control
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
                    gz.PostCommand(cmdObj);
                    // somehow we need to set a timer that expires and revokes the connectedness if we haven't gotten a reply
                    openCommands.Add(DateTime.Now.Add(new TimeSpan(0, 0, 2)), cmdObj);
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
                if (currentGoalCommand == "Platform/Disconnect")
                    currentGoalStepMet = true;
            }
        }
        #endregion

        #region servo control
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
        #endregion

        #region camera control
        private void SnapPictureButton_Click(object sender, EventArgs e)
        {
            LogText("Temporary - coopt for SD Test");
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
        #endregion

        #region ranger control
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
        #endregion

        #region motor commands
        private void SendMotorCommand(int cmd, int speed)
        {
            // read the motor select checks
            int motorMask = 0;
            if (motorCheckFL.Checked)
                motorMask += Godzuki.ZukiCommands.COMMAND_CONST_MOTORCONTROL_MOTOR1;
            if (motorCheckRL.Checked)
                motorMask += Godzuki.ZukiCommands.COMMAND_CONST_MOTORCONTROL_MOTOR2;
            if (motorCheckFR.Checked)
                motorMask += Godzuki.ZukiCommands.COMMAND_CONST_MOTORCONTROL_MOTOR4;
            if (motorCheckRR.Checked)
                motorMask += Godzuki.ZukiCommands.COMMAND_CONST_MOTORCONTROL_MOTOR3;
            // build the right command 
            long parameter = cmd * 100000 + (motorMask * 1000) + speed;

            Godzuki.gCommandObject cmdObj = new Godzuki.gCommandObject(
                Godzuki.ZukiCommands.CNC_APP_DEVICE_ID, 1,
                Godzuki.ZukiCommands.MOTOR_CONTROL_DEVICE_ID, 1,
                Godzuki.ZukiCommands.COMMAND_ID_MOTORCONTROL_START, parameter);
            // go
            if (gz.PostCommand(cmdObj))
                openCommands.Add(DateTime.Now.Add(new TimeSpan(0, 0, 5)), cmdObj);
        }
        private void MotorStop_Click(object sender, EventArgs e)
        {
            SendMotorCommand(Godzuki.ZukiCommands.COMMAND_CONST_MOTORCONTROL_STOP, 0);
        }
        private void MotorFwd_Click(object sender, EventArgs e)
        {
            SendMotorCommand(Godzuki.ZukiCommands.COMMAND_CONST_MOTORCONTROL_FORWARD, MotorSpeed.Value);
        }
        private void MotorBack_Click(object sender, EventArgs e)
        {
            SendMotorCommand(Godzuki.ZukiCommands.COMMAND_CONST_MOTORCONTROL_BACKWARD, MotorSpeed.Value);
        }
        private void MotorLeft_Click(object sender, EventArgs e)
        {
            SendMotorCommand(Godzuki.ZukiCommands.COMMAND_CONST_MOTORCONTROL_TURNLEFT, TurningSpeed.Value);
        }
        private void MotorRight_Click(object sender, EventArgs e)
        {
            SendMotorCommand(Godzuki.ZukiCommands.COMMAND_CONST_MOTORCONTROL_TURNRIGHT, TurningSpeed.Value);
        }
        private void GoButton_Click(object sender, EventArgs e)
        {
            Godzuki.gCommandObject cmdObj = new Godzuki.gCommandObject(
                Godzuki.ZukiCommands.CNC_APP_DEVICE_ID, 1,
                Godzuki.ZukiCommands.MOTOR_CONTROL_DEVICE_ID, 1,
                Godzuki.ZukiCommands.COMMAND_ID_MOTORCONTROL_SET_SPEED_SLOW, -1);
            if (gz.PostCommand(cmdObj))
                openCommands.Add(DateTime.Now.Add(new TimeSpan(0, 0, 5)), cmdObj);

            cmdObj = new Godzuki.gCommandObject(
                Godzuki.ZukiCommands.CNC_APP_DEVICE_ID, 1,
                Godzuki.ZukiCommands.MOTOR_CONTROL_DEVICE_ID, 1,
                Godzuki.ZukiCommands.COMMAND_ID_MOTORCONTROL_SET_DIR_FWD, -1);
            if (gz.PostCommand(cmdObj))
                openCommands.Add(DateTime.Now.Add(new TimeSpan(0, 0, 5, 1)), cmdObj);

            cmdObj = new Godzuki.gCommandObject(
                Godzuki.ZukiCommands.CNC_APP_DEVICE_ID, 1,
                Godzuki.ZukiCommands.MOTOR_CONTROL_DEVICE_ID, 1,
                Godzuki.ZukiCommands.COMMAND_ID_MOTORCONTROL_START, -1);
            if (gz.PostCommand(cmdObj))
                openCommands.Add(DateTime.Now.Add(new TimeSpan(0, 0, 5, 2)), cmdObj);

        }

        #endregion

        #endregion

        #region Goal Management

        private void ExecuteGoalButton_Click(object sender, EventArgs e)
        {
            goalDoc = new System.Xml.XmlDocument();
            goalDoc.Load(FindGoalFileName((string)goalList.SelectedItem));
            currentGoalStepMet = false;
            currentGoalNode = goalDoc.SelectNodes("Goal")[0];
            GoalCurrentStep.Text = "Running " + currentGoalNode.Attributes["Name"].Value;
            LogText("Running " + currentGoalNode.Attributes["Name"].Value);
        }
        private void ManageGoalState()  // called in the timer loop - update goal state if ready
        {
            if (currentGoalNode == null)
                return;
            CheckTimedSteps();

            if (currentGoalNode.Name == "Goal") // check for scripting parameters, then mark as met
            {
                InitGoalParamList();
                allGoalNodes = goalDoc.SelectNodes("Goal/Step");
                currentGoalIndex = -1;
                currentGoalStepMet = true;
            }

            if (currentGoalStepMet)
            {
                // if i'm done, advance the state of the goal
                if (++currentGoalIndex < allGoalNodes.Count)
                {
                    currentGoalNode = allGoalNodes[currentGoalIndex];
                    ExecuteGoalStep();
                }
                else
                {
                    LogText("Goal Completed");
                    currentGoalNode = null;
                }
            }
            else
            {
                // if i'm not done, have I timed out?
                if (DateTime.Now > currentStepTimeout)
                {
                    bool failthrough = IgnoreStepFails.Checked;
                    LogText("Goal Failed");
                    if (!failthrough)
                    {
                        currentGoalNode = null;
                        currentGoalStepMet = false;
                    }
                    else
                    {
                        currentGoalStepMet = true;
                    }
                }
            }
        }
        private void CheckTimedSteps()
        {
            if (currentGoalNode.Name != "Step")
                return;
            string curCommand = currentGoalNode.Attributes["Device"].Value + "/" + currentGoalNode.Attributes["Command"].Value;
            if (curCommand != "Platform/Delay")
                return;
            if (DateTime.Now > currentStepTimeout)
                currentGoalStepMet = true;
        }
        private void ExecuteGoalStep()
        {
            string newStepName = currentGoalNode.Attributes["Name"].Value;
            LogText("Goal Step: " + newStepName);
            currentGoalStepMet = false;
            long timeoutMS = Convert.ToInt64(currentGoalNode.Attributes["Timeout"].Value);
            currentStepTimeout = DateTime.Now.AddMilliseconds(timeoutMS);
            List<long> param = new List<long>();
            currentGoalCommand = currentGoalNode.Attributes["Device"].Value + "/" + currentGoalNode.Attributes["Command"].Value;
            switch (currentGoalCommand)
            {
                case "Platform/Connect":  // first parameter is the COM port...
                    availablePorts.SelectedItem = getStringGoalParam(0);
                    if (!isConnected)
                        ConnectToRobotButton_Click(null, null);
                    else
                    {
                        LogText("...already connected");
                        currentGoalStepMet = true;
                    }
                    break;
                case "Platform/Disconnect":
                    if (isConnected)
                        ConnectToRobotButton_Click(null, null);
                    else
                    {
                        LogText("...already disconnected");
                        currentGoalStepMet = true;
                    }
                    break;
                case "Platform/Delay":  // first parameter is the delay amount...
                    currentStepTimeout = DateTime.Now.AddMilliseconds( getLongGoalParam(0) );
                    break;
                case "Platform/UpdateMap":
                    LogText("...ok - start mapping");
                    break;
                case "Ranger/Read":
                    ReadRangerButton_Click(null, null);
                    break;
                default:
                    LogText("...no idea what this is...skipping");
                    currentGoalStepMet = true;
                    break;
            }
        }
        private string getStringGoalParam(int index)
        {
            System.Xml.XmlNode thisParam = currentGoalNode.ChildNodes[index];
            string thisParamName = thisParam.Attributes["Name"].Value;
            return goalParamTokens[thisParamName];
        }
        private long getLongGoalParam(int index)
        {
            return Convert.ToInt64(getStringGoalParam(index));
        }
        private void UpdateGoalState(Godzuki.gCommandObject updObj)   // called for any rec'd reply objects
        {
            if (goalDoc == null)
                return;
            // if this cmd obj satisfies the currentky open goal state, take an appropriate action
            if (currentGoalNode.Name == "Goal") // check for scripting parameters, then mark as met
            {
                InitGoalParamList();
                currentGoalStepMet = true;
            }
            else
            {
                // ok - it's a real goal state - am I done?
                // most of this is done either in the cmd reply stuff
            }
        }
        private void InitGoalParamList()
        {
            MessageLoopTimer.Stop();
            goalParamTokens = new Dictionary<string, string>();
            string s;
            List<string> paramNames = Tokenize(currentGoalNode.Attributes["Parameters"].Value, ",");
            List<string> paramValues = Tokenize(currentGoalNode.Attributes["DefaultValues"].Value, ",");
            for (int i = 0; i < paramNames.Count; i++)
            {
                if (UseGoalDefaults.Checked)
                {
                    goalParamTokens.Add(paramNames[i], paramValues[i]);
                }
                else
                {
                    ParamCapture pc = new ParamCapture();
                    pc.StartPosition = FormStartPosition.CenterParent;
                    pc.ParameterDisplay.Text = paramNames[i];
                    pc.ValueDisplay.Text = paramValues[i];
                    if (pc.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                    {
                        goalParamTokens.Add(paramNames[i], pc.ValueDisplay.Text);
                    }
                    else
                    {
                        goalParamTokens.Add(paramNames[i], paramValues[i]);
                    }
                }
            }
            MessageLoopTimer.Start();
        }
        #endregion

        #region Utilities
        private void RefreshPortData_Click(object sender, EventArgs e)
        {
            string[] ports = Godzuki.ZukiComms.GetPortNames();
            string refText = availablePorts.SelectedText;
            availablePorts.Items.Clear();
            if (ports.Length == 0)
                availablePorts.Items.Add("None");
            else
                foreach (string s in ports)
                    availablePorts.Items.Add(s);
            if (refText != "" && ports.Contains(refText))
                availablePorts.SelectedText = refText;
            else
                availablePorts.SelectedIndex = 0;
            if (availableInstances.SelectedIndex < 0)
                availableInstances.SelectedIndex = 0;

            InitializeGoals();
        }

        private List<string> Tokenize(string s, string delim)
        {
            List<string> outList = new List<string>();
            int curIndex = 0;
            while (curIndex < s.Length)
            {
                int nextIndex = s.IndexOf(delim, curIndex);
                string thisToken = "";
                if (nextIndex < 0)  // last one
                {
                    thisToken = s.Substring(curIndex).Trim();
                    curIndex = s.Length;
                }
                else
                {
                    thisToken = s.Substring(curIndex, nextIndex - curIndex).Trim();
                    curIndex = nextIndex + delim.Length;
                }
                outList.Add(thisToken);
            }
            return outList;
        }

        private void LogText(string s)
        {
            if (s.Trim() != "")
                sessionLog.AppendText(s.Trim() + Environment.NewLine);
        }

        private string FindGoalFileName(string item)
        {
            return "../../../Goals/" + item.Replace('-', '/') + ".xml";
        }
        #endregion
    }
}
