using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UProtectApp
{
    public partial class PassswordForm : Form
    {
        private bool _SelfClose = false;

        public delegate void PassswordCallback(
            string Password, 
            out bool Close, 
            bool AppClosed
        );

        private PassswordCallback _Callback;

        public PassswordForm(PassswordCallback callback)
        {
            _Callback = callback;
            InitializeComponent();

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void LogInButton_Click(object sender, EventArgs e)
        {
            bool close;
            _Callback(
                PasswordTextBox.Text, 
                out close, 
                false
            );

            if (close)
            {
                Close();
                _SelfClose = true;
                return;
            }

            IncorectPasswordText.Visible = true;
        }

        private void PassswordForm_Load(object sender, EventArgs e)
        {

        }

        private void PassswordForm_Close(object sender, FormClosedEventArgs e)
        {
            if (_SelfClose)
            {
                return;
            }

            bool close;
            _Callback
                ("", 
                out close, 
                true
            );
        }
    }
}
