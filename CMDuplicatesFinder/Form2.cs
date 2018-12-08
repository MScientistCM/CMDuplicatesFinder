using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ClassLibrary;


namespace CMDuplicatesFinder
{
    public partial class Form2 : Form
    {
        private string path;

        private static string FIRST_NAME_STRING = "First name";
        private static string LAST_NAME_STRING = "Second name";
        private static string COMMON_NAME_STRING = "Common name";
        private static string DOB_STRING = "DOB";
        private static string CLUB_STRING = "Club";

        public Form2(string path)
        {
            InitializeComponent();

            this.path = path;
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            Trace.WriteLine("path:"+path);
            var Lines = File.ReadAllLines(path).Select(line => line.Split(';'));
            Trace.WriteLine("read");

            ClassLibrary.Class1 instance = new ClassLibrary.Class1();
            Trace.WriteLine("read2:" + instance.GetDuplicates(path));

            int lineCount = 0;

            int firstNameIndex = 0;
            int lastNameIndex = 1;
            int commonNameIndex = 2;
            int dobIndex = 3;
            int clubIndex = 9;

            foreach(string[] line in Lines)
            {
                if (lineCount % 1 == 0)
                    Trace.WriteLine("progress:"+lineCount);
                if (lineCount == 0)   //first line is the column titles
                {
                    int columnIndex = 0;
                    foreach (string l in line)
                    {
                        //makes sure the appropriate index for each desired column is correct:
                        if (l.Equals(FIRST_NAME_STRING))
                        {
                            firstNameIndex = columnIndex;
                        }
                        else if (l.Equals(LAST_NAME_STRING)) {
                            lastNameIndex = columnIndex;
                        }
                        else if (l.Equals(COMMON_NAME_STRING)) {
                            commonNameIndex = columnIndex;
                        }
                        else if (l.Equals(DOB_STRING)) {
                            dobIndex = columnIndex;
                        }
                        else if (l.Equals(CLUB_STRING)) {
                            clubIndex = columnIndex;
                        }
                        columnIndex++;
                    }
                }
                else
                {
                   // Staff currentStaff = new Staff(line, firstNameIndex, lastNameIndex, commonNameIndex, dobIndex, clubIndex);

                    int otherLineCount = 0;
                    foreach (string[] otherStaffLine in Lines)
                    {
                        //Debug.WriteLine("progressX:" + otherLineCount);
                        if (otherLineCount <= lineCount)
                        {
                           
                        } else
                        {
                           // Staff otherStaff = new Staff(otherStaffLine, firstNameIndex, lastNameIndex, commonNameIndex, dobIndex, clubIndex);

                           // if(Staff.Compare(currentStaff, otherStaff))
                            {
                             //   Trace.WriteLine("identical:" + currentStaff.print());
                            }
                        }

                        otherLineCount++;
                    }
                }

                //julio baptista very interesting duplicate case in october 2018 database
               
                lineCount++;
            }
            Trace.WriteLine("done");            
        }        

        private void Form2_FormClosing(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void Form2_FormClosed(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
