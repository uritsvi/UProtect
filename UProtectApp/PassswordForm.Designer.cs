namespace UProtectApp
{
    partial class PassswordForm
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
            this.label1 = new System.Windows.Forms.Label();
            this.PasswordTextBox = new System.Windows.Forms.TextBox();
            this.LogInButton = new System.Windows.Forms.Button();
            this.IncorectPasswordText = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(101, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(78, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "EnterPassword";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // PasswordTextBox
            // 
            this.PasswordTextBox.Location = new System.Drawing.Point(88, 59);
            this.PasswordTextBox.Name = "PasswordTextBox";
            this.PasswordTextBox.Size = new System.Drawing.Size(106, 20);
            this.PasswordTextBox.TabIndex = 1;
            // 
            // LogInButton
            // 
            this.LogInButton.Location = new System.Drawing.Point(104, 98);
            this.LogInButton.Name = "LogInButton";
            this.LogInButton.Size = new System.Drawing.Size(75, 23);
            this.LogInButton.TabIndex = 2;
            this.LogInButton.Text = "Log In";
            this.LogInButton.UseVisualStyleBackColor = true;
            this.LogInButton.Click += new System.EventHandler(this.LogInButton_Click);
            // 
            // IncorectPasswordText
            // 
            this.IncorectPasswordText.AutoSize = true;
            this.IncorectPasswordText.ForeColor = System.Drawing.Color.Red;
            this.IncorectPasswordText.Location = new System.Drawing.Point(97, 82);
            this.IncorectPasswordText.Name = "IncorectPasswordText";
            this.IncorectPasswordText.Size = new System.Drawing.Size(92, 13);
            this.IncorectPasswordText.TabIndex = 3;
            this.IncorectPasswordText.Text = "IncorectPassword";
            this.IncorectPasswordText.Visible = false;
            // 
            // PassswordForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(275, 132);
            this.Controls.Add(this.IncorectPasswordText);
            this.Controls.Add(this.LogInButton);
            this.Controls.Add(this.PasswordTextBox);
            this.Controls.Add(this.label1);
            this.Name = "PassswordForm";
            this.Text = "PassswordForm";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.PassswordForm_Close);
            this.Load += new System.EventHandler(this.PassswordForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox PasswordTextBox;
        private System.Windows.Forms.Button LogInButton;
        private System.Windows.Forms.Label IncorectPasswordText;
    }
}