namespace SimsonSpeedTester
{
    partial class Form1
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.trackBarRPM = new System.Windows.Forms.TrackBar();
            this.labelRPM = new System.Windows.Forms.Label();
            this.trackBarSpeed = new System.Windows.Forms.TrackBar();
            this.labelSpeed = new System.Windows.Forms.Label();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.buttonConnect = new System.Windows.Forms.Button();
            this.textBoxCOM = new System.Windows.Forms.TextBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.trackBarRPM)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarSpeed)).BeginInit();
            this.SuspendLayout();
            // 
            // trackBarRPM
            // 
            this.trackBarRPM.LargeChange = 10;
            this.trackBarRPM.Location = new System.Drawing.Point(12, 12);
            this.trackBarRPM.Maximum = 100;
            this.trackBarRPM.Name = "trackBarRPM";
            this.trackBarRPM.Size = new System.Drawing.Size(394, 45);
            this.trackBarRPM.TabIndex = 0;
            this.trackBarRPM.TickFrequency = 10;
            this.trackBarRPM.Value = 20;
            this.trackBarRPM.Scroll += new System.EventHandler(this.trackBarRPM_Scroll);
            // 
            // labelRPM
            // 
            this.labelRPM.AutoSize = true;
            this.labelRPM.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelRPM.Location = new System.Drawing.Point(412, 12);
            this.labelRPM.Name = "labelRPM";
            this.labelRPM.Size = new System.Drawing.Size(96, 24);
            this.labelRPM.TabIndex = 2;
            this.labelRPM.Text = "2000 RPM";
            // 
            // trackBarSpeed
            // 
            this.trackBarSpeed.LargeChange = 10;
            this.trackBarSpeed.Location = new System.Drawing.Point(12, 63);
            this.trackBarSpeed.Maximum = 100;
            this.trackBarSpeed.Name = "trackBarSpeed";
            this.trackBarSpeed.Size = new System.Drawing.Size(394, 45);
            this.trackBarSpeed.TabIndex = 0;
            this.trackBarSpeed.TickFrequency = 10;
            this.trackBarSpeed.Value = 25;
            this.trackBarSpeed.Scroll += new System.EventHandler(this.trackBarSpeed_Scroll);
            // 
            // labelSpeed
            // 
            this.labelSpeed.AutoSize = true;
            this.labelSpeed.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelSpeed.Location = new System.Drawing.Point(412, 63);
            this.labelSpeed.Name = "labelSpeed";
            this.labelSpeed.Size = new System.Drawing.Size(76, 24);
            this.labelSpeed.TabIndex = 2;
            this.labelSpeed.Text = "25 km/h";
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(118, 114);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(75, 23);
            this.buttonConnect.TabIndex = 3;
            this.buttonConnect.Text = "Verbinden";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // textBoxCOM
            // 
            this.textBoxCOM.Location = new System.Drawing.Point(12, 114);
            this.textBoxCOM.Name = "textBoxCOM";
            this.textBoxCOM.Size = new System.Drawing.Size(100, 20);
            this.textBoxCOM.TabIndex = 4;
            this.textBoxCOM.Text = "COM10";
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 500;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(520, 147);
            this.Controls.Add(this.textBoxCOM);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.labelSpeed);
            this.Controls.Add(this.labelRPM);
            this.Controls.Add(this.trackBarSpeed);
            this.Controls.Add(this.trackBarRPM);
            this.MaximumSize = new System.Drawing.Size(536, 186);
            this.MinimumSize = new System.Drawing.Size(536, 186);
            this.Name = "Form1";
            this.Text = "SimsonSpeed-Tester";
            ((System.ComponentModel.ISupportInitialize)(this.trackBarRPM)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarSpeed)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TrackBar trackBarRPM;
        private System.Windows.Forms.Label labelRPM;
        private System.Windows.Forms.TrackBar trackBarSpeed;
        private System.Windows.Forms.Label labelSpeed;
        private System.IO.Ports.SerialPort serialPort;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.TextBox textBoxCOM;
        private System.Windows.Forms.Timer timer1;
    }
}

