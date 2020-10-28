namespace TrueStepTerminal
{
    partial class frmMain
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
            this.grpParameters = new System.Windows.Forms.GroupBox();
            this.chkWriteToFlash = new System.Windows.Forms.CheckBox();
            this.btnWriteParameters = new System.Windows.Forms.Button();
            this.btnReadParameters = new System.Windows.Forms.Button();
            this.dataGridViewParameters = new System.Windows.Forms.DataGridView();
            this.grpOverview = new System.Windows.Forms.GroupBox();
            this.lblAngleError = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.lblAngleValue = new System.Windows.Forms.Label();
            this.btnControlGetAngle = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this.gprControls = new System.Windows.Forms.GroupBox();
            this.tabControls = new System.Windows.Forms.TabControl();
            this.tabPageMain = new System.Windows.Forms.TabPage();
            this.btnMove = new System.Windows.Forms.Button();
            this.chkChangeDirection = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.rdClosedLoop = new System.Windows.Forms.RadioButton();
            this.rdOpenLoop = new System.Windows.Forms.RadioButton();
            this.chkEnable = new System.Windows.Forms.CheckBox();
            this.btnControlStep = new System.Windows.Forms.Button();
            this.tabPageExperimental = new System.Windows.Forms.TabPage();
            this.label7 = new System.Windows.Forms.Label();
            this.cmbStepLoop = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.cmbStepDirection = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.cmbStepSize = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.cmbStepCurrent = new System.Windows.Forms.ComboBox();
            this.grpConnection = new System.Windows.Forms.GroupBox();
            this.btnConnect = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.cmbPortBaud = new System.Windows.Forms.ComboBox();
            this.cmbPortName = new System.Windows.Forms.ComboBox();
            this.btnClose = new System.Windows.Forms.Button();
            this.tabPageTuning = new System.Windows.Forms.TabPage();
            this.chkTuningEnable = new System.Windows.Forms.CheckBox();
            this.btnAutoTune = new System.Windows.Forms.Button();
            this.grpParameters.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewParameters)).BeginInit();
            this.grpOverview.SuspendLayout();
            this.gprControls.SuspendLayout();
            this.tabControls.SuspendLayout();
            this.tabPageMain.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabPageExperimental.SuspendLayout();
            this.grpConnection.SuspendLayout();
            this.tabPageTuning.SuspendLayout();
            this.SuspendLayout();
            // 
            // grpParameters
            // 
            this.grpParameters.Controls.Add(this.chkWriteToFlash);
            this.grpParameters.Controls.Add(this.btnWriteParameters);
            this.grpParameters.Controls.Add(this.btnReadParameters);
            this.grpParameters.Controls.Add(this.dataGridViewParameters);
            this.grpParameters.Location = new System.Drawing.Point(12, 164);
            this.grpParameters.Name = "grpParameters";
            this.grpParameters.Size = new System.Drawing.Size(400, 202);
            this.grpParameters.TabIndex = 1;
            this.grpParameters.TabStop = false;
            this.grpParameters.Text = "Parameters";
            // 
            // chkWriteToFlash
            // 
            this.chkWriteToFlash.AutoSize = true;
            this.chkWriteToFlash.Location = new System.Drawing.Point(319, 77);
            this.chkWriteToFlash.Name = "chkWriteToFlash";
            this.chkWriteToFlash.Size = new System.Drawing.Size(67, 17);
            this.chkWriteToFlash.TabIndex = 4;
            this.chkWriteToFlash.Text = "To Flash";
            this.chkWriteToFlash.UseVisualStyleBackColor = true;
            // 
            // btnWriteParameters
            // 
            this.btnWriteParameters.Location = new System.Drawing.Point(319, 48);
            this.btnWriteParameters.Name = "btnWriteParameters";
            this.btnWriteParameters.Size = new System.Drawing.Size(75, 23);
            this.btnWriteParameters.TabIndex = 3;
            this.btnWriteParameters.Text = "Write";
            this.btnWriteParameters.UseVisualStyleBackColor = true;
            this.btnWriteParameters.Click += new System.EventHandler(this.btnWriteParameters_Click);
            // 
            // btnReadParameters
            // 
            this.btnReadParameters.Location = new System.Drawing.Point(319, 19);
            this.btnReadParameters.Name = "btnReadParameters";
            this.btnReadParameters.Size = new System.Drawing.Size(75, 23);
            this.btnReadParameters.TabIndex = 2;
            this.btnReadParameters.Text = "Read";
            this.btnReadParameters.UseVisualStyleBackColor = true;
            this.btnReadParameters.Click += new System.EventHandler(this.btnReadParameters_Click);
            // 
            // dataGridViewParameters
            // 
            this.dataGridViewParameters.AllowUserToAddRows = false;
            this.dataGridViewParameters.AllowUserToDeleteRows = false;
            this.dataGridViewParameters.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridViewParameters.Location = new System.Drawing.Point(9, 19);
            this.dataGridViewParameters.Name = "dataGridViewParameters";
            this.dataGridViewParameters.Size = new System.Drawing.Size(291, 177);
            this.dataGridViewParameters.TabIndex = 0;
            // 
            // grpOverview
            // 
            this.grpOverview.Controls.Add(this.lblAngleError);
            this.grpOverview.Controls.Add(this.label9);
            this.grpOverview.Controls.Add(this.lblAngleValue);
            this.grpOverview.Controls.Add(this.btnControlGetAngle);
            this.grpOverview.Controls.Add(this.label8);
            this.grpOverview.Location = new System.Drawing.Point(12, 72);
            this.grpOverview.Name = "grpOverview";
            this.grpOverview.Size = new System.Drawing.Size(400, 86);
            this.grpOverview.TabIndex = 2;
            this.grpOverview.TabStop = false;
            this.grpOverview.Text = "Overview";
            // 
            // lblAngleError
            // 
            this.lblAngleError.AutoSize = true;
            this.lblAngleError.Location = new System.Drawing.Point(56, 53);
            this.lblAngleError.Name = "lblAngleError";
            this.lblAngleError.Size = new System.Drawing.Size(13, 13);
            this.lblAngleError.TabIndex = 15;
            this.lblAngleError.Text = "0";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(6, 53);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(29, 13);
            this.label9.TabIndex = 14;
            this.label9.Text = "Error";
            // 
            // lblAngleValue
            // 
            this.lblAngleValue.AutoSize = true;
            this.lblAngleValue.Location = new System.Drawing.Point(56, 24);
            this.lblAngleValue.Name = "lblAngleValue";
            this.lblAngleValue.Size = new System.Drawing.Size(13, 13);
            this.lblAngleValue.TabIndex = 13;
            this.lblAngleValue.Text = "0";
            // 
            // btnControlGetAngle
            // 
            this.btnControlGetAngle.Location = new System.Drawing.Point(319, 19);
            this.btnControlGetAngle.Name = "btnControlGetAngle";
            this.btnControlGetAngle.Size = new System.Drawing.Size(75, 23);
            this.btnControlGetAngle.TabIndex = 1;
            this.btnControlGetAngle.Text = "Get Angle";
            this.btnControlGetAngle.UseVisualStyleBackColor = true;
            this.btnControlGetAngle.Click += new System.EventHandler(this.btnControlGetAngle_Click);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 24);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(34, 13);
            this.label8.TabIndex = 12;
            this.label8.Text = "Angle";
            // 
            // gprControls
            // 
            this.gprControls.Controls.Add(this.tabControls);
            this.gprControls.Location = new System.Drawing.Point(12, 372);
            this.gprControls.Name = "gprControls";
            this.gprControls.Size = new System.Drawing.Size(400, 157);
            this.gprControls.TabIndex = 3;
            this.gprControls.TabStop = false;
            this.gprControls.Text = "Controls";
            // 
            // tabControls
            // 
            this.tabControls.Controls.Add(this.tabPageMain);
            this.tabControls.Controls.Add(this.tabPageTuning);
            this.tabControls.Controls.Add(this.tabPageExperimental);
            this.tabControls.Location = new System.Drawing.Point(9, 19);
            this.tabControls.Name = "tabControls";
            this.tabControls.SelectedIndex = 0;
            this.tabControls.Size = new System.Drawing.Size(385, 132);
            this.tabControls.TabIndex = 12;
            // 
            // tabPageMain
            // 
            this.tabPageMain.Controls.Add(this.btnMove);
            this.tabPageMain.Controls.Add(this.chkChangeDirection);
            this.tabPageMain.Controls.Add(this.groupBox1);
            this.tabPageMain.Controls.Add(this.chkEnable);
            this.tabPageMain.Controls.Add(this.btnControlStep);
            this.tabPageMain.Location = new System.Drawing.Point(4, 22);
            this.tabPageMain.Name = "tabPageMain";
            this.tabPageMain.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageMain.Size = new System.Drawing.Size(377, 106);
            this.tabPageMain.TabIndex = 0;
            this.tabPageMain.Text = "Main";
            this.tabPageMain.UseVisualStyleBackColor = true;
            // 
            // btnMove
            // 
            this.btnMove.Location = new System.Drawing.Point(294, 33);
            this.btnMove.Name = "btnMove";
            this.btnMove.Size = new System.Drawing.Size(75, 23);
            this.btnMove.TabIndex = 16;
            this.btnMove.Text = "Move";
            this.btnMove.UseVisualStyleBackColor = true;
            this.btnMove.Click += new System.EventHandler(this.btnMove_Click);
            // 
            // chkChangeDirection
            // 
            this.chkChangeDirection.AutoSize = true;
            this.chkChangeDirection.Location = new System.Drawing.Point(126, 81);
            this.chkChangeDirection.Name = "chkChangeDirection";
            this.chkChangeDirection.Size = new System.Drawing.Size(108, 17);
            this.chkChangeDirection.TabIndex = 15;
            this.chkChangeDirection.Text = "Change Direction";
            this.chkChangeDirection.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.rdClosedLoop);
            this.groupBox1.Controls.Add(this.rdOpenLoop);
            this.groupBox1.Location = new System.Drawing.Point(6, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(115, 69);
            this.groupBox1.TabIndex = 14;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Mode";
            // 
            // rdClosedLoop
            // 
            this.rdClosedLoop.AutoSize = true;
            this.rdClosedLoop.Location = new System.Drawing.Point(6, 42);
            this.rdClosedLoop.Name = "rdClosedLoop";
            this.rdClosedLoop.Size = new System.Drawing.Size(84, 17);
            this.rdClosedLoop.TabIndex = 16;
            this.rdClosedLoop.Text = "Closed Loop";
            this.rdClosedLoop.UseVisualStyleBackColor = true;
            this.rdClosedLoop.CheckedChanged += new System.EventHandler(this.rdClosedLoop_CheckedChanged);
            // 
            // rdOpenLoop
            // 
            this.rdOpenLoop.AutoSize = true;
            this.rdOpenLoop.Checked = true;
            this.rdOpenLoop.Location = new System.Drawing.Point(6, 19);
            this.rdOpenLoop.Name = "rdOpenLoop";
            this.rdOpenLoop.Size = new System.Drawing.Size(78, 17);
            this.rdOpenLoop.TabIndex = 15;
            this.rdOpenLoop.TabStop = true;
            this.rdOpenLoop.Text = "Open Loop";
            this.rdOpenLoop.UseVisualStyleBackColor = true;
            this.rdOpenLoop.CheckedChanged += new System.EventHandler(this.rdOpenLoop_CheckedChanged);
            // 
            // chkEnable
            // 
            this.chkEnable.AutoSize = true;
            this.chkEnable.Location = new System.Drawing.Point(6, 81);
            this.chkEnable.Name = "chkEnable";
            this.chkEnable.Size = new System.Drawing.Size(59, 17);
            this.chkEnable.TabIndex = 13;
            this.chkEnable.Text = "Enable";
            this.chkEnable.UseVisualStyleBackColor = true;
            this.chkEnable.CheckedChanged += new System.EventHandler(this.chkEnable_CheckedChanged);
            // 
            // btnControlStep
            // 
            this.btnControlStep.Location = new System.Drawing.Point(296, 77);
            this.btnControlStep.Name = "btnControlStep";
            this.btnControlStep.Size = new System.Drawing.Size(75, 23);
            this.btnControlStep.TabIndex = 0;
            this.btnControlStep.Text = "Step";
            this.btnControlStep.UseVisualStyleBackColor = true;
            this.btnControlStep.Click += new System.EventHandler(this.btnControlStep_Click);
            // 
            // tabPageExperimental
            // 
            this.tabPageExperimental.Controls.Add(this.label7);
            this.tabPageExperimental.Controls.Add(this.cmbStepLoop);
            this.tabPageExperimental.Controls.Add(this.label6);
            this.tabPageExperimental.Controls.Add(this.label3);
            this.tabPageExperimental.Controls.Add(this.cmbStepDirection);
            this.tabPageExperimental.Controls.Add(this.label4);
            this.tabPageExperimental.Controls.Add(this.cmbStepSize);
            this.tabPageExperimental.Controls.Add(this.label5);
            this.tabPageExperimental.Controls.Add(this.cmbStepCurrent);
            this.tabPageExperimental.Location = new System.Drawing.Point(4, 22);
            this.tabPageExperimental.Name = "tabPageExperimental";
            this.tabPageExperimental.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageExperimental.Size = new System.Drawing.Size(377, 106);
            this.tabPageExperimental.TabIndex = 1;
            this.tabPageExperimental.Text = "Experimental";
            this.tabPageExperimental.UseVisualStyleBackColor = true;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(3, 3);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(237, 13);
            this.label7.TabIndex = 11;
            this.label7.Text = "Fine Step Settings used for experimental purpose";
            // 
            // cmbStepLoop
            // 
            this.cmbStepLoop.FormattingEnabled = true;
            this.cmbStepLoop.Items.AddRange(new object[] {
            "1",
            "200",
            "400",
            "800",
            "1600"});
            this.cmbStepLoop.Location = new System.Drawing.Point(226, 79);
            this.cmbStepLoop.Name = "cmbStepLoop";
            this.cmbStepLoop.Size = new System.Drawing.Size(48, 21);
            this.cmbStepLoop.TabIndex = 10;
            this.cmbStepLoop.Text = "1";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(189, 82);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(31, 13);
            this.label6.TabIndex = 9;
            this.label6.Text = "Loop";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(5, 28);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(74, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "Step Direction";
            // 
            // cmbStepDirection
            // 
            this.cmbStepDirection.FormattingEnabled = true;
            this.cmbStepDirection.Items.AddRange(new object[] {
            "CW",
            "CCW"});
            this.cmbStepDirection.Location = new System.Drawing.Point(97, 25);
            this.cmbStepDirection.Name = "cmbStepDirection";
            this.cmbStepDirection.Size = new System.Drawing.Size(48, 21);
            this.cmbStepDirection.TabIndex = 4;
            this.cmbStepDirection.Text = "CW";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(5, 55);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(52, 13);
            this.label4.TabIndex = 5;
            this.label4.Text = "Step Size";
            // 
            // cmbStepSize
            // 
            this.cmbStepSize.FormattingEnabled = true;
            this.cmbStepSize.Items.AddRange(new object[] {
            "225",
            "450",
            "900",
            "1800"});
            this.cmbStepSize.Location = new System.Drawing.Point(97, 52);
            this.cmbStepSize.Name = "cmbStepSize";
            this.cmbStepSize.Size = new System.Drawing.Size(48, 21);
            this.cmbStepSize.TabIndex = 6;
            this.cmbStepSize.Text = "1800";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(5, 82);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(41, 13);
            this.label5.TabIndex = 7;
            this.label5.Text = "Current";
            // 
            // cmbStepCurrent
            // 
            this.cmbStepCurrent.FormattingEnabled = true;
            this.cmbStepCurrent.Items.AddRange(new object[] {
            "100",
            "200",
            "300",
            "400",
            "500",
            "600",
            "700",
            "800",
            "900",
            "1000"});
            this.cmbStepCurrent.Location = new System.Drawing.Point(97, 79);
            this.cmbStepCurrent.Name = "cmbStepCurrent";
            this.cmbStepCurrent.Size = new System.Drawing.Size(48, 21);
            this.cmbStepCurrent.TabIndex = 8;
            this.cmbStepCurrent.Text = "500";
            // 
            // grpConnection
            // 
            this.grpConnection.Controls.Add(this.btnConnect);
            this.grpConnection.Controls.Add(this.label2);
            this.grpConnection.Controls.Add(this.label1);
            this.grpConnection.Controls.Add(this.cmbPortBaud);
            this.grpConnection.Controls.Add(this.cmbPortName);
            this.grpConnection.Location = new System.Drawing.Point(12, 12);
            this.grpConnection.Name = "grpConnection";
            this.grpConnection.Size = new System.Drawing.Size(400, 54);
            this.grpConnection.TabIndex = 4;
            this.grpConnection.TabStop = false;
            this.grpConnection.Text = "Connection";
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(319, 17);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(75, 23);
            this.btnConnect.TabIndex = 4;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(154, 22);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(32, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Baud";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(26, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Port";
            // 
            // cmbPortBaud
            // 
            this.cmbPortBaud.FormattingEnabled = true;
            this.cmbPortBaud.Location = new System.Drawing.Point(192, 19);
            this.cmbPortBaud.Name = "cmbPortBaud";
            this.cmbPortBaud.Size = new System.Drawing.Size(96, 21);
            this.cmbPortBaud.TabIndex = 1;
            // 
            // cmbPortName
            // 
            this.cmbPortName.FormattingEnabled = true;
            this.cmbPortName.Location = new System.Drawing.Point(38, 19);
            this.cmbPortName.Name = "cmbPortName";
            this.cmbPortName.Size = new System.Drawing.Size(96, 21);
            this.cmbPortName.TabIndex = 0;
            this.cmbPortName.DropDown += new System.EventHandler(this.cmbPortName_DropDown);
            // 
            // btnClose
            // 
            this.btnClose.Location = new System.Drawing.Point(337, 535);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(75, 23);
            this.btnClose.TabIndex = 5;
            this.btnClose.Text = "&Close";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // tabPageTuning
            // 
            this.tabPageTuning.Controls.Add(this.btnAutoTune);
            this.tabPageTuning.Controls.Add(this.chkTuningEnable);
            this.tabPageTuning.Location = new System.Drawing.Point(4, 22);
            this.tabPageTuning.Name = "tabPageTuning";
            this.tabPageTuning.Size = new System.Drawing.Size(377, 106);
            this.tabPageTuning.TabIndex = 2;
            this.tabPageTuning.Text = "Tuning";
            this.tabPageTuning.UseVisualStyleBackColor = true;
            // 
            // chkTuningEnable
            // 
            this.chkTuningEnable.AutoSize = true;
            this.chkTuningEnable.Location = new System.Drawing.Point(3, 12);
            this.chkTuningEnable.Name = "chkTuningEnable";
            this.chkTuningEnable.Size = new System.Drawing.Size(100, 17);
            this.chkTuningEnable.TabIndex = 16;
            this.chkTuningEnable.Text = "Enable Logging";
            this.chkTuningEnable.UseVisualStyleBackColor = true;
            this.chkTuningEnable.CheckedChanged += new System.EventHandler(this.chkTuningEnable_CheckedChanged);
            // 
            // btnAutoTune
            // 
            this.btnAutoTune.Location = new System.Drawing.Point(3, 80);
            this.btnAutoTune.Name = "btnAutoTune";
            this.btnAutoTune.Size = new System.Drawing.Size(75, 23);
            this.btnAutoTune.TabIndex = 17;
            this.btnAutoTune.Text = "Auto Tune";
            this.btnAutoTune.UseVisualStyleBackColor = true;
            this.btnAutoTune.Click += new System.EventHandler(this.btnAutoTune_Click);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(424, 570);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.grpConnection);
            this.Controls.Add(this.gprControls);
            this.Controls.Add(this.grpOverview);
            this.Controls.Add(this.grpParameters);
            this.Name = "frmMain";
            this.Text = "TrueStep Terminal";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmMain_FormClosing);
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.grpParameters.ResumeLayout(false);
            this.grpParameters.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewParameters)).EndInit();
            this.grpOverview.ResumeLayout(false);
            this.grpOverview.PerformLayout();
            this.gprControls.ResumeLayout(false);
            this.tabControls.ResumeLayout(false);
            this.tabPageMain.ResumeLayout(false);
            this.tabPageMain.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tabPageExperimental.ResumeLayout(false);
            this.tabPageExperimental.PerformLayout();
            this.grpConnection.ResumeLayout(false);
            this.grpConnection.PerformLayout();
            this.tabPageTuning.ResumeLayout(false);
            this.tabPageTuning.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.GroupBox grpParameters;
        private System.Windows.Forms.GroupBox grpOverview;
        private System.Windows.Forms.GroupBox gprControls;
        private System.Windows.Forms.GroupBox grpConnection;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cmbPortBaud;
        private System.Windows.Forms.ComboBox cmbPortName;
        private System.Windows.Forms.Button btnControlStep;
        private System.Windows.Forms.Button btnControlGetAngle;
        private System.Windows.Forms.DataGridView dataGridViewParameters;
        private System.Windows.Forms.Button btnWriteParameters;
        private System.Windows.Forms.Button btnReadParameters;
        private System.Windows.Forms.ComboBox cmbStepDirection;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox cmbStepCurrent;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox cmbStepSize;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox chkWriteToFlash;
        private System.Windows.Forms.ComboBox cmbStepLoop;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.TabControl tabControls;
        private System.Windows.Forms.TabPage tabPageMain;
        private System.Windows.Forms.TabPage tabPageExperimental;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton rdClosedLoop;
        private System.Windows.Forms.RadioButton rdOpenLoop;
        private System.Windows.Forms.CheckBox chkEnable;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label lblAngleValue;
        private System.Windows.Forms.Label lblAngleError;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.CheckBox chkChangeDirection;
        private System.Windows.Forms.Button btnMove;
        private System.Windows.Forms.TabPage tabPageTuning;
        private System.Windows.Forms.CheckBox chkTuningEnable;
        private System.Windows.Forms.Button btnAutoTune;
    }
}

