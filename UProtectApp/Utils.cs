using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UProtectApp
{
    internal class Utils
    {
        public static void ExitWithError(string Msg)
        {
            MessageBox.Show(Msg);
            Environment.Exit(-1);
        }
    }
}
