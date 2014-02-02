namespace ZukiCnC
{
    partial class MainCnCForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.sessionLog = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.availableInstances = new System.Windows.Forms.ComboBox();
            this.RefreshPortData = new System.Windows.Forms.Button();
            this.button12 = new System.Windows.Forms.Button();
            this.goalList = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.ConnectToRobotButton = new System.Windows.Forms.Button();
            this.availablePorts = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.knob1 = new AuSharp.Knob();
            this.MotorSpeed = new AuSharp.Knob();
            this.button11 = new System.Windows.Forms.Button();
            this.button10 = new System.Windows.Forms.Button();
            this.button9 = new System.Windows.Forms.Button();
            this.button8 = new System.Windows.Forms.Button();
            this.button7 = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.groupBox9 = new System.Windows.Forms.GroupBox();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.groupBox8 = new System.Windows.Forms.GroupBox();
            this.ReadRangerButton = new System.Windows.Forms.Button();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.SetServoButton = new System.Windows.Forms.Button();
            this.ServoTargetPos = new AuSharp.Knob();
            this.GetServoPositionButton = new System.Windows.Forms.Button();
            this.CurrentServoPosition = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.SDcommandChoice = new System.Windows.Forms.ComboBox();
            this.button2 = new System.Windows.Forms.Button();
            this.SnapPictureButton = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.MessageLoopTimer = new System.Windows.Forms.Timer(this.components);
            this.ClearCmdLogButton = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox9.SuspendLayout();
            this.groupBox8.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.groupBox7.SuspendLayout();
            this.groupBox6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.sessionLog);
            this.groupBox1.Location = new System.Drawing.Point(12, 362);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(270, 196);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Command Log";
            // 
            // sessionLog
            // 
            this.sessionLog.AcceptsReturn = true;
            this.sessionLog.AcceptsTab = true;
            this.sessionLog.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.sessionLog.Location = new System.Drawing.Point(12, 19);
            this.sessionLog.Multiline = true;
            this.sessionLog.Name = "sessionLog";
            this.sessionLog.ReadOnly = true;
            this.sessionLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.sessionLog.Size = new System.Drawing.Size(252, 171);
            this.sessionLog.TabIndex = 0;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.availableInstances);
            this.groupBox2.Controls.Add(this.RefreshPortData);
            this.groupBox2.Controls.Add(this.button12);
            this.groupBox2.Controls.Add(this.goalList);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.ConnectToRobotButton);
            this.groupBox2.Controls.Add(this.availablePorts);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Location = new System.Drawing.Point(288, 362);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(353, 196);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Connection";
            // 
            // availableInstances
            // 
            this.availableInstances.FormattingEnabled = true;
            this.availableInstances.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10"});
            this.availableInstances.Location = new System.Drawing.Point(159, 19);
            this.availableInstances.Name = "availableInstances";
            this.availableInstances.Size = new System.Drawing.Size(37, 21);
            this.availableInstances.TabIndex = 14;
            // 
            // RefreshPortData
            // 
            this.RefreshPortData.Location = new System.Drawing.Point(202, 167);
            this.RefreshPortData.Name = "RefreshPortData";
            this.RefreshPortData.Size = new System.Drawing.Size(64, 23);
            this.RefreshPortData.TabIndex = 13;
            this.RefreshPortData.Text = "Refresh";
            this.RefreshPortData.UseVisualStyleBackColor = true;
            this.RefreshPortData.Click += new System.EventHandler(this.RefreshPortData_Click);
            // 
            // button12
            // 
            this.button12.Location = new System.Drawing.Point(208, 44);
            this.button12.Name = "button12";
            this.button12.Size = new System.Drawing.Size(93, 23);
            this.button12.TabIndex = 12;
            this.button12.Text = "Execute";
            this.button12.UseVisualStyleBackColor = true;
            // 
            // goalList
            // 
            this.goalList.FormattingEnabled = true;
            this.goalList.Location = new System.Drawing.Point(89, 46);
            this.goalList.Name = "goalList";
            this.goalList.Size = new System.Drawing.Size(113, 21);
            this.goalList.TabIndex = 11;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(14, 49);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(69, 13);
            this.label3.TabIndex = 10;
            this.label3.Text = "Current Goal:";
            // 
            // ConnectToRobotButton
            // 
            this.ConnectToRobotButton.Location = new System.Drawing.Point(208, 17);
            this.ConnectToRobotButton.Name = "ConnectToRobotButton";
            this.ConnectToRobotButton.Size = new System.Drawing.Size(93, 23);
            this.ConnectToRobotButton.TabIndex = 8;
            this.ConnectToRobotButton.Text = "Connect";
            this.ConnectToRobotButton.UseVisualStyleBackColor = true;
            this.ConnectToRobotButton.Click += new System.EventHandler(this.ConnectToRobotButton_Click);
            // 
            // availablePorts
            // 
            this.availablePorts.FormattingEnabled = true;
            this.availablePorts.Location = new System.Drawing.Point(89, 19);
            this.availablePorts.Name = "availablePorts";
            this.availablePorts.Size = new System.Drawing.Size(64, 21);
            this.availablePorts.TabIndex = 9;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(14, 22);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(80, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "COM/Instance:";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.knob1);
            this.groupBox3.Controls.Add(this.MotorSpeed);
            this.groupBox3.Controls.Add(this.button11);
            this.groupBox3.Controls.Add(this.button10);
            this.groupBox3.Controls.Add(this.button9);
            this.groupBox3.Controls.Add(this.button8);
            this.groupBox3.Controls.Add(this.button7);
            this.groupBox3.Location = new System.Drawing.Point(647, 362);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(189, 196);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Motion Control";
            // 
            // knob1
            // 
            this.knob1.KnobColor = System.Drawing.Color.Black;
            this.knob1.Location = new System.Drawing.Point(7, 133);
            this.knob1.MarkerColor = System.Drawing.Color.Black;
            this.knob1.MaxAngle = 180;
            this.knob1.Maximum = 80;
            this.knob1.Minimum = 40;
            this.knob1.Name = "knob1";
            this.knob1.Size = new System.Drawing.Size(57, 62);
            this.knob1.TabIndex = 10;
            this.knob1.Text = "Turn Speed";
            this.knob1.TextKnobRelation = AuSharp.TextKnobRelation.KnobAboveText;
            this.knob1.TickColor = System.Drawing.Color.Black;
            this.knob1.Value = 50;
            // 
            // MotorSpeed
            // 
            this.MotorSpeed.KnobColor = System.Drawing.Color.Black;
            this.MotorSpeed.Location = new System.Drawing.Point(122, 133);
            this.MotorSpeed.MarkerColor = System.Drawing.Color.Black;
            this.MotorSpeed.MaxAngle = 180;
            this.MotorSpeed.Maximum = 150;
            this.MotorSpeed.Minimum = 60;
            this.MotorSpeed.Name = "MotorSpeed";
            this.MotorSpeed.Size = new System.Drawing.Size(57, 62);
            this.MotorSpeed.TabIndex = 9;
            this.MotorSpeed.Text = "Fwd Speed";
            this.MotorSpeed.TextKnobRelation = AuSharp.TextKnobRelation.KnobAboveText;
            this.MotorSpeed.TickColor = System.Drawing.Color.Black;
            this.MotorSpeed.Value = 80;
            this.MotorSpeed.DoubleClick += new System.EventHandler(this.MotorSpeed_DoubleClick);
            // 
            // button11
            // 
            this.button11.Location = new System.Drawing.Point(75, 124);
            this.button11.Name = "button11";
            this.button11.Size = new System.Drawing.Size(30, 23);
            this.button11.TabIndex = 4;
            this.button11.Text = "V";
            this.button11.UseVisualStyleBackColor = true;
            // 
            // button10
            // 
            this.button10.Location = new System.Drawing.Point(39, 95);
            this.button10.Name = "button10";
            this.button10.Size = new System.Drawing.Size(30, 23);
            this.button10.TabIndex = 3;
            this.button10.Text = "<";
            this.button10.UseVisualStyleBackColor = true;
            // 
            // button9
            // 
            this.button9.Location = new System.Drawing.Point(111, 95);
            this.button9.Name = "button9";
            this.button9.Size = new System.Drawing.Size(30, 23);
            this.button9.TabIndex = 2;
            this.button9.Text = ">";
            this.button9.UseVisualStyleBackColor = true;
            // 
            // button8
            // 
            this.button8.Location = new System.Drawing.Point(75, 95);
            this.button8.Name = "button8";
            this.button8.Size = new System.Drawing.Size(30, 23);
            this.button8.TabIndex = 1;
            this.button8.Text = "X";
            this.button8.UseVisualStyleBackColor = true;
            // 
            // button7
            // 
            this.button7.Location = new System.Drawing.Point(75, 66);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(30, 23);
            this.button7.TabIndex = 0;
            this.button7.Text = "^";
            this.button7.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.groupBox9);
            this.groupBox4.Controls.Add(this.groupBox8);
            this.groupBox4.Controls.Add(this.trackBar1);
            this.groupBox4.Controls.Add(this.groupBox7);
            this.groupBox4.Controls.Add(this.groupBox6);
            this.groupBox4.Controls.Add(this.groupBox5);
            this.groupBox4.Location = new System.Drawing.Point(12, 12);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(824, 324);
            this.groupBox4.TabIndex = 3;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Monitoring";
            // 
            // groupBox9
            // 
            this.groupBox9.Controls.Add(this.textBox4);
            this.groupBox9.Location = new System.Drawing.Point(284, 19);
            this.groupBox9.Name = "groupBox9";
            this.groupBox9.Size = new System.Drawing.Size(264, 112);
            this.groupBox9.TabIndex = 9;
            this.groupBox9.TabStop = false;
            this.groupBox9.Text = "Attitude";
            // 
            // textBox4
            // 
            this.textBox4.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.textBox4.Location = new System.Drawing.Point(6, 19);
            this.textBox4.Multiline = true;
            this.textBox4.Name = "textBox4";
            this.textBox4.ReadOnly = true;
            this.textBox4.Size = new System.Drawing.Size(252, 87);
            this.textBox4.TabIndex = 1;
            // 
            // groupBox8
            // 
            this.groupBox8.Controls.Add(this.ReadRangerButton);
            this.groupBox8.Controls.Add(this.textBox3);
            this.groupBox8.Controls.Add(this.label4);
            this.groupBox8.Location = new System.Drawing.Point(284, 137);
            this.groupBox8.Name = "groupBox8";
            this.groupBox8.Size = new System.Drawing.Size(264, 49);
            this.groupBox8.TabIndex = 8;
            this.groupBox8.TabStop = false;
            this.groupBox8.Text = "Ranging";
            // 
            // ReadRangerButton
            // 
            this.ReadRangerButton.Location = new System.Drawing.Point(141, 17);
            this.ReadRangerButton.Name = "ReadRangerButton";
            this.ReadRangerButton.Size = new System.Drawing.Size(47, 23);
            this.ReadRangerButton.TabIndex = 7;
            this.ReadRangerButton.Text = "Read";
            this.ReadRangerButton.UseVisualStyleBackColor = true;
            this.ReadRangerButton.Click += new System.EventHandler(this.ReadRangerButton_Click);
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(96, 19);
            this.textBox3.Name = "textBox3";
            this.textBox3.ReadOnly = true;
            this.textBox3.Size = new System.Drawing.Size(39, 20);
            this.textBox3.TabIndex = 6;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 22);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(87, 13);
            this.label4.TabIndex = 5;
            this.label4.Text = "Current Reading:";
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(6, 273);
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(812, 45);
            this.trackBar1.TabIndex = 3;
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.SetServoButton);
            this.groupBox7.Controls.Add(this.ServoTargetPos);
            this.groupBox7.Controls.Add(this.GetServoPositionButton);
            this.groupBox7.Controls.Add(this.CurrentServoPosition);
            this.groupBox7.Controls.Add(this.label1);
            this.groupBox7.Location = new System.Drawing.Point(284, 192);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Size = new System.Drawing.Size(264, 75);
            this.groupBox7.TabIndex = 2;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "Servo Control";
            // 
            // SetServoButton
            // 
            this.SetServoButton.Location = new System.Drawing.Point(211, 17);
            this.SetServoButton.Name = "SetServoButton";
            this.SetServoButton.Size = new System.Drawing.Size(47, 23);
            this.SetServoButton.TabIndex = 4;
            this.SetServoButton.Text = "Set";
            this.SetServoButton.UseVisualStyleBackColor = true;
            this.SetServoButton.Click += new System.EventHandler(this.SetServoButton_Click);
            // 
            // ServoTargetPos
            // 
            this.ServoTargetPos.KnobColor = System.Drawing.Color.Black;
            this.ServoTargetPos.Location = new System.Drawing.Point(161, 7);
            this.ServoTargetPos.MarkerColor = System.Drawing.Color.Black;
            this.ServoTargetPos.MaxAngle = 90;
            this.ServoTargetPos.Maximum = 135;
            this.ServoTargetPos.Minimum = 45;
            this.ServoTargetPos.Name = "ServoTargetPos";
            this.ServoTargetPos.Size = new System.Drawing.Size(57, 62);
            this.ServoTargetPos.TabIndex = 8;
            this.ServoTargetPos.Text = "Target Pos";
            this.ServoTargetPos.TextKnobRelation = AuSharp.TextKnobRelation.KnobAboveText;
            this.ServoTargetPos.TickColor = System.Drawing.Color.Black;
            this.ServoTargetPos.Value = 45;
            this.ServoTargetPos.DoubleClick += new System.EventHandler(this.ServoTargetPos_DoubleClick);
            // 
            // GetServoPositionButton
            // 
            this.GetServoPositionButton.Location = new System.Drawing.Point(96, 42);
            this.GetServoPositionButton.Name = "GetServoPositionButton";
            this.GetServoPositionButton.Size = new System.Drawing.Size(47, 23);
            this.GetServoPositionButton.TabIndex = 7;
            this.GetServoPositionButton.Text = "Read";
            this.GetServoPositionButton.UseVisualStyleBackColor = true;
            this.GetServoPositionButton.Click += new System.EventHandler(this.GetServoPositionButton_Click);
            // 
            // CurrentServoPosition
            // 
            this.CurrentServoPosition.Location = new System.Drawing.Point(96, 19);
            this.CurrentServoPosition.Name = "CurrentServoPosition";
            this.CurrentServoPosition.ReadOnly = true;
            this.CurrentServoPosition.Size = new System.Drawing.Size(47, 20);
            this.CurrentServoPosition.TabIndex = 6;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(84, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "Current Position:";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.SDcommandChoice);
            this.groupBox6.Controls.Add(this.button2);
            this.groupBox6.Controls.Add(this.SnapPictureButton);
            this.groupBox6.Controls.Add(this.pictureBox1);
            this.groupBox6.Location = new System.Drawing.Point(554, 19);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(264, 248);
            this.groupBox6.TabIndex = 2;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Imaging";
            // 
            // SDcommandChoice
            // 
            this.SDcommandChoice.FormattingEnabled = true;
            this.SDcommandChoice.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9"});
            this.SDcommandChoice.Location = new System.Drawing.Point(6, 216);
            this.SDcommandChoice.Name = "SDcommandChoice";
            this.SDcommandChoice.Size = new System.Drawing.Size(68, 21);
            this.SDcommandChoice.TabIndex = 3;
            this.SDcommandChoice.Text = "1";
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(168, 215);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(80, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "Play / Pause";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // SnapPictureButton
            // 
            this.SnapPictureButton.Location = new System.Drawing.Point(82, 215);
            this.SnapPictureButton.Name = "SnapPictureButton";
            this.SnapPictureButton.Size = new System.Drawing.Size(80, 23);
            this.SnapPictureButton.TabIndex = 1;
            this.SnapPictureButton.Text = "Snap";
            this.SnapPictureButton.UseVisualStyleBackColor = true;
            this.SnapPictureButton.Click += new System.EventHandler(this.SnapPictureButton_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.pictureBox1.Location = new System.Drawing.Point(6, 19);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(252, 190);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.pictureBox2);
            this.groupBox5.Location = new System.Drawing.Point(6, 19);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(272, 248);
            this.groupBox5.TabIndex = 1;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Mapping";
            // 
            // pictureBox2
            // 
            this.pictureBox2.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.pictureBox2.Location = new System.Drawing.Point(6, 19);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(258, 219);
            this.pictureBox2.TabIndex = 3;
            this.pictureBox2.TabStop = false;
            // 
            // MessageLoopTimer
            // 
            this.MessageLoopTimer.Interval = 250;
            this.MessageLoopTimer.Tick += new System.EventHandler(this.MessageLoopTimer_Tick);
            // 
            // ClearCmdLogButton
            // 
            this.ClearCmdLogButton.Location = new System.Drawing.Point(232, 558);
            this.ClearCmdLogButton.Name = "ClearCmdLogButton";
            this.ClearCmdLogButton.Size = new System.Drawing.Size(44, 23);
            this.ClearCmdLogButton.TabIndex = 4;
            this.ClearCmdLogButton.Text = "Clr";
            this.ClearCmdLogButton.UseVisualStyleBackColor = true;
            this.ClearCmdLogButton.Click += new System.EventHandler(this.ClearCmdLogButton_Click);
            // 
            // MainCnCForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(848, 590);
            this.Controls.Add(this.ClearCmdLogButton);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "MainCnCForm";
            this.Text = "Zuki CnC";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox9.ResumeLayout(false);
            this.groupBox9.PerformLayout();
            this.groupBox8.ResumeLayout(false);
            this.groupBox8.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.groupBox7.ResumeLayout(false);
            this.groupBox7.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.groupBox5.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button SnapPictureButton;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.TextBox sessionLog;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.Button GetServoPositionButton;
        private System.Windows.Forms.TextBox CurrentServoPosition;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button SetServoButton;
        private System.Windows.Forms.Button button12;
        private System.Windows.Forms.ComboBox goalList;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button ConnectToRobotButton;
        private System.Windows.Forms.ComboBox availablePorts;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button button11;
        private System.Windows.Forms.Button button10;
        private System.Windows.Forms.Button button9;
        private System.Windows.Forms.Button button8;
        private System.Windows.Forms.Button button7;
        private System.Windows.Forms.GroupBox groupBox9;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.GroupBox groupBox8;
        private System.Windows.Forms.Button ReadRangerButton;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button RefreshPortData;
        private System.Windows.Forms.ComboBox availableInstances;
        private System.Windows.Forms.Timer MessageLoopTimer;
        private AuSharp.Knob ServoTargetPos;
        private AuSharp.Knob MotorSpeed;
        private AuSharp.Knob knob1;
        private System.Windows.Forms.ComboBox SDcommandChoice;
        private System.Windows.Forms.Button ClearCmdLogButton;
    }
}

