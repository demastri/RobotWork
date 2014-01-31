using System;
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
        bool isConnected;
        bool isExecutingGoal;
        Godzuki.ZukiComms gz = new Godzuki.ZukiComms();

        public MainCnCForm()
        {
            isConnected = isExecutingGoal = false;
            InitializeComponent();
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
                    ConnectToRobotButton.Text = "Disconnect";
                    isConnected = true;
                }
                else
                {
                    gz.curData.Add("Couldn't open port, please check connections...");
                    MessageBox.Show("Couldn't open port, please check connections...");
                }
            }
            else
            {
                gz.ShutDown();
                isConnected = false;
            }

        }

        private void LogText(string s)
        {
            sessionLog.Text += s + Environment.NewLine;
        }
        private void MessageLoopTimer_Tick(object sender, EventArgs e)
        {
            while (gz.hasData)
            {
                LogText(gz.curData[0]);
                gz.curData.RemoveAt(0);
            }
        }

        private void GetServoPositionButton_Click(object sender, EventArgs e)
        {
            LogText("Request Servo Position");
            // here's how the command system will work
            
            // wrong way:
            // since there's no real commands to route here, we can simply use the comms object 
            // to get strings which we can pack and unpack as appropriate.
            // for now
            // gz.PostCommand(ServoDeviceID, ServoInstanceID, ServoReadCommandID);
            gz.PostCommand("#5001030106000000!");

            // right way?
            // put the stringification back into the comms object
            // route command objects as normal...
            // ideally
            // gz.PostCommand(ServoDeviceID, ServoInstanceID, ServoReadCommandID);

            // in any case, at some point the command will return a response 
            // that will cause the display to update...
        }
    }
}
