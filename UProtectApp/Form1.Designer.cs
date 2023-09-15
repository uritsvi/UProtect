namespace UProtectApp
{
    partial class Form1
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
            this.AddButton = new System.Windows.Forms.Button();
            this.Entries = new System.Windows.Forms.CheckedListBox();
            this.NewEntryTextBox = new System.Windows.Forms.TextBox();
            this.FileButton = new System.Windows.Forms.Button();
            this.FolderButton = new System.Windows.Forms.Button();
            this.RemoveButton = new System.Windows.Forms.Button();
            this.SaveButton = new System.Windows.Forms.Button();
            this.StartDriverButton = new System.Windows.Forms.Button();
            this.StopDriverButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // AddButton
            // 
            this.AddButton.Location = new System.Drawing.Point(695, 21);
            this.AddButton.Name = "AddButton";
            this.AddButton.Size = new System.Drawing.Size(53, 23);
            this.AddButton.TabIndex = 5;
            this.AddButton.Text = "Add";
            this.AddButton.UseVisualStyleBackColor = true;
            this.AddButton.Click += new System.EventHandler(this.AddButton_Click);
            // 
            // Entries
            // 
            this.Entries.FormattingEnabled = true;
            this.Entries.Location = new System.Drawing.Point(28, 48);
            this.Entries.Name = "Entries";
            this.Entries.Size = new System.Drawing.Size(791, 394);
            this.Entries.TabIndex = 7;
            this.Entries.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.ItemCheck_Callback);
            // 
            // NewEntryTextBox
            // 
            this.NewEntryTextBox.Location = new System.Drawing.Point(28, 21);
            this.NewEntryTextBox.Name = "NewEntryTextBox";
            this.NewEntryTextBox.Size = new System.Drawing.Size(543, 20);
            this.NewEntryTextBox.TabIndex = 8;
            // 
            // FileButton
            // 
            this.FileButton.Location = new System.Drawing.Point(636, 21);
            this.FileButton.Name = "FileButton";
            this.FileButton.Size = new System.Drawing.Size(53, 23);
            this.FileButton.TabIndex = 9;
            this.FileButton.Text = "File...";
            this.FileButton.UseVisualStyleBackColor = true;
            this.FileButton.Click += new System.EventHandler(this.FileButton_Click);
            // 
            // FolderButton
            // 
            this.FolderButton.Location = new System.Drawing.Point(577, 21);
            this.FolderButton.Name = "FolderButton";
            this.FolderButton.Size = new System.Drawing.Size(53, 23);
            this.FolderButton.TabIndex = 10;
            this.FolderButton.Text = "Folder...";
            this.FolderButton.UseVisualStyleBackColor = true;
            this.FolderButton.Click += new System.EventHandler(this.FolderButton_Click);
            // 
            // RemoveButton
            // 
            this.RemoveButton.Enabled = false;
            this.RemoveButton.Location = new System.Drawing.Point(754, 21);
            this.RemoveButton.Name = "RemoveButton";
            this.RemoveButton.Size = new System.Drawing.Size(65, 23);
            this.RemoveButton.TabIndex = 11;
            this.RemoveButton.Text = "Remove";
            this.RemoveButton.UseVisualStyleBackColor = true;
            this.RemoveButton.Click += new System.EventHandler(this.RemoveButton_Click);
            // 
            // SaveButton
            // 
            this.SaveButton.Location = new System.Drawing.Point(739, 467);
            this.SaveButton.Name = "SaveButton";
            this.SaveButton.Size = new System.Drawing.Size(80, 23);
            this.SaveButton.TabIndex = 12;
            this.SaveButton.Text = "Save";
            this.SaveButton.UseVisualStyleBackColor = true;
            this.SaveButton.Click += new System.EventHandler(this.SaveButton_Click);
            // 
            // StartDriverButton
            // 
            this.StartDriverButton.Enabled = false;
            this.StartDriverButton.Location = new System.Drawing.Point(28, 467);
            this.StartDriverButton.Name = "StartDriverButton";
            this.StartDriverButton.Size = new System.Drawing.Size(80, 23);
            this.StartDriverButton.TabIndex = 13;
            this.StartDriverButton.Text = "Start Driver";
            this.StartDriverButton.UseVisualStyleBackColor = true;
            this.StartDriverButton.UseWaitCursor = true;
            this.StartDriverButton.Click += new System.EventHandler(this.StartDriverButton_Click);
            // 
            // StopDriverButton
            // 
            this.StopDriverButton.Location = new System.Drawing.Point(114, 467);
            this.StopDriverButton.Name = "StopDriverButton";
            this.StopDriverButton.Size = new System.Drawing.Size(80, 23);
            this.StopDriverButton.TabIndex = 14;
            this.StopDriverButton.Text = "Stop Driver";
            this.StopDriverButton.UseVisualStyleBackColor = true;
            this.StopDriverButton.Click += new System.EventHandler(this.StopDriverButton_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(852, 511);
            this.Controls.Add(this.StopDriverButton);
            this.Controls.Add(this.StartDriverButton);
            this.Controls.Add(this.SaveButton);
            this.Controls.Add(this.RemoveButton);
            this.Controls.Add(this.FolderButton);
            this.Controls.Add(this.FileButton);
            this.Controls.Add(this.NewEntryTextBox);
            this.Controls.Add(this.Entries);
            this.Controls.Add(this.AddButton);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button AddButton;
        private System.Windows.Forms.CheckedListBox Entries;
        private System.Windows.Forms.TextBox NewEntryTextBox;
        private System.Windows.Forms.Button FileButton;
        private System.Windows.Forms.Button FolderButton;
        private System.Windows.Forms.Button RemoveButton;
        private System.Windows.Forms.Button SaveButton;
        private System.Windows.Forms.Button StartDriverButton;
        private System.Windows.Forms.Button StopDriverButton;
    }
}

