using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using Microsoft.Win32;

namespace UProtectApp
{
    public partial class Form1 : Form
    {
        const int MaxPathLen = 256;
        const int MaxPaths = (1 << 10);

        public Form1()
        {
            InitializeComponent();
        }

        private void AddItem(string Item)
        {
            if(Item == "")
            {
                return;
            }

            bool res =
                KprotectDll.AddFilePathToProtect(Item);

            if (!res)
            {
                MessageBox.Show("Item with the same name already exists!");
                return;
            }
            Entries.Items.Add(Item);
        }
        private void RemoveItem(string Item)
        {
            bool res =
                KprotectDll.RemoveMiniFilterPath(Item);
            if (!res)
            {
                MessageBox.Show("Failed to delete path!");
                return;
            }
            
            Entries.Items.Remove(Item);

        }


        void LoadAllBuiledPaths()
        {

            bool res = true;
            int size = MaxPathLen * MaxPaths;
            IntPtr buffer = Marshal.AllocHGlobal(size);

            do
            {  
                res = KprotectDll.QueryMiniFilter(
                    buffer,
                    size
                );
                if (!res)
                {
                    break;
                }

                string miniFilterBuiledPaths =
                    Marshal.PtrToStringUni(buffer);

                var items = miniFilterBuiledPaths.Split(';');
                foreach(var item in items)
                {
                    if(item == "")
                    {
                        continue;
                    }
                    AddItem(item);
                }


            } while (false);

            Marshal.FreeHGlobal(buffer);

        }

        void StartApp()
        {
            bool res = true;
            if (!res)
            {
                Utils.ExitWithError("Failed to init aho corasick interface!");
                return;
            }
            LoadAllBuiledPaths();

        }

        private void PasswordCallback(
            string Password,
            out bool Close,
            bool AppClosed)
        {
            Close = false;

            if (AppClosed)
            {
                this.Close();
                return;
            }
            if (Password == "a")
            {
                Close = true;
            }

            StartApp();

        }

        private void SwapStartStopDriverButtonsState()
        {
            StartDriverButton.Enabled = !StartDriverButton.Enabled;
            StopDriverButton.Enabled = !StopDriverButton.Enabled;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            bool res =
             KprotectDll.InitKProtectInteface();

            var passwordForm = new PassswordForm(PasswordCallback);
            passwordForm.ShowDialog();

            if (!KprotectDll.IsDriverLoaded())
            {
                SwapStartStopDriverButtonsState();
            }
        }
        private void AddButton_Click(object sender, EventArgs e)
        {
 

            AddItem(NewEntryTextBox.Text);
        }

        private void FileButton_Click(object sender, EventArgs e)
        {

            using (OpenFileDialog dialog = new OpenFileDialog())
            {
                dialog.Filter = "All files (*.*)|*.*";

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    AddItem(dialog.FileName);
                }
            }

        }

        private void FolderButton_Click(object sender, EventArgs e)
        {
            using (FolderBrowserDialog dialog = new FolderBrowserDialog())
            {
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    AddItem(dialog.SelectedPath);

                }
            }

        }

        private void RemoveButton_Click(object sender, EventArgs e)
        {
            var remove = new List<object>();
            foreach(var item in Entries.CheckedItems)
            {
                remove.Add(item);
            }

            foreach(var item in remove)
            {
                 RemoveItem((string)item);
            }
        }

        private void ItemCheck_Callback(object sender, ItemCheckEventArgs e)
        {
            if(Entries.CheckedItems.Count == 1 && e.NewValue == CheckState.Unchecked)
            {
                RemoveButton.Enabled = false;
                return;
            }
            RemoveButton.Enabled = true;
        }

        private void SaveButton_Click(object sender, EventArgs e)
        {
            bool res = 
                KprotectDll.ApplyFilePaths();
            if (!res)
            {
                MessageBox.Show("Failed to save paths!");
            }
        }

        private void StartDriverButton_Click(object sender, EventArgs e)
        {
            bool res = 
                KprotectDll.StartDriver();

            if (!res)
            {
                return;
            }
            SwapStartStopDriverButtonsState();

        }

        private void StopDriverButton_Click(object sender, EventArgs e)
        {

            bool res = 
                KprotectDll.StopDriver();
            if (!res)
            {
                return;
            }
            SwapStartStopDriverButtonsState();
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            KprotectDll.ShutDownd();
        }

        private void Entries_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
