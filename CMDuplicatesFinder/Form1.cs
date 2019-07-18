using System;
using System.ComponentModel;
using System.Windows.Forms;

namespace CMDuplicatesFinder
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }        

        private void button1_Click(object sender, EventArgs e)
        {
            if (labelFilename.Text.ToString().Length > 0) 
            {
                new Form2(labelFilename.Text).Show();
                Hide();
            }            
        }

        private void debugInstructionsLabel_Click(object sender, EventArgs e)
        {

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void buttonBrowse_Click(object sender, EventArgs e)
        {
            if(openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                labelFilename.Text = openFileDialog1.FileName;
            }
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("https://champman0102.co.uk/showthread.php?t=12814");
        }
    }
}
