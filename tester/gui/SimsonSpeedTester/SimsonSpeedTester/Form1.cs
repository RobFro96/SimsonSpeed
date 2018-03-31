using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SimsonSpeedTester
{
    public partial class Form1 : Form
    {
        private bool updateRPM, updateSpeed;

        public Form1()
        {
            InitializeComponent();
        }

        private void trackBarRPM_Scroll(object sender, EventArgs e)
        {
            int value = trackBarRPM.Value;
            value = value == 0 ? 1 : value;

            updateRPM = true;

            labelRPM.Text = (value * 100) + " RPM";
        }

        private void trackBarSpeed_Scroll(object sender, EventArgs e)
        {
            int value = trackBarSpeed.Value;
            value = value < 10 ? 10 : value;

            updateSpeed = true;

            labelSpeed.Text = (value) + " km/h";
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Close();
                buttonConnect.Text = "Verbinden";
            }
            else
            {
                serialPort.PortName = textBoxCOM.Text;
                try
                {
                    serialPort.Open();
                    buttonConnect.Text = "Trennen";
                    updateRPM = true;
                    updateSpeed = true;
                }
                catch (Exception )
                {
                    MessageBox.Show("Serielle Schnittstelle konnte nicht geöffent werden.");
                }
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (!serialPort.IsOpen)
                return;

            if (updateRPM)
            {
                int value = trackBarRPM.Value;
                value = value == 0 ? 1 : value;

                UInt16 ocr1a = (UInt16)(16000000 / (80 * value));
                serialPort.Write("r" + ocr1a + ";");

                updateRPM = false;
            }

            if (updateSpeed)
            {

                int value = trackBarSpeed.Value;
                value = value < 5 ? 5 : value;

                UInt16 delay = (UInt16)((71 * 78) / value);
                serialPort.Write("s" + delay + ";");

                updateSpeed = false;
            }
            
        }
    }
}
