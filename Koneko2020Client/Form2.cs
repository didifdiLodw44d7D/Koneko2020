using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Koneko2020Client
{
    public partial class Form2 : Form
    {
        public string ans;
        public Form2()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ans = "こ";
        }

        private void button2_Click(object sender, EventArgs e)
        {
            ans = "ね";
        }
    }
}
