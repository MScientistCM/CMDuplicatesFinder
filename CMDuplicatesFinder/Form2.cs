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

        public Form2(string path)
        {
            InitializeComponent();

            this.path = path;

            backgroundWorker1.WorkerReportsProgress = true;
            backgroundWorker1.WorkerSupportsCancellation = true;
            backgroundWorker1.DoWork += new DoWorkEventHandler(backgroundWorker1_DoWork);
            backgroundWorker1.RunWorkerCompleted += new RunWorkerCompletedEventHandler(backgroundWorker1_RunWorkerCompleted);
            backgroundWorker1.ProgressChanged += new ProgressChangedEventHandler(backgroundWorker1_ProgressChanged);
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            Trace.WriteLine("path:"+path);            

            if (backgroundWorker1.IsBusy != true)
            {
                Trace.WriteLine("async");
                // Start the asynchronous operation.
                backgroundWorker1.RunWorkerAsync(path);
            }

            //TODO: implement pause so it saves the duplicate arrays to a text file and then it can resume when reopen app   
            
            Trace.WriteLine("load method end");            
        }

        // This event handler is where the time-consuming work is done.
        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            Trace.WriteLine("asyncwork "+ (string)e.Argument);
            BackgroundWorker worker = sender as BackgroundWorker;

            ClassLibrary.Class1 instance = new ClassLibrary.Class1();
            
            //TODO: update progress bar from c++ to c#. Some references for updating progress bar from c++ to c#:
            //https://www.codeproject.com/Tips/695387/%2FTips%2F695387%2FCalling-Csharp-NET-methods-from-unmanaged-C-Cplusp
            //https://stackoverflow.com/questions/11909484/how-to-use-c-sharp-backgroundworker-to-report-progress-in-native-c-code
            //https://stackoverflow.com/questions/20529951/send-int-array-via-backgroundworker-reportprogress-in-c
            //https://stackoverflow.com/questions/30323112/how-to-return-data-from-a-backgroundworker-in-c
            //https://stackoverflow.com/questions/11123899/passing-a-c-sharp-class-object-in-and-out-of-a-c-dll-class
            //Search clr backgroundworker in google

            /* for (int i = 1; i <= 10; i++)
             {
                 if (worker.CancellationPending == true)
                 {
                     e.Cancel = true;
                     break;
                 }
                 else
                 {
                     // Perform a time consuming operation and report progress.
                     System.Threading.Thread.Sleep(500);
                     worker.ReportProgress(i * 10);
                 }
             }*/
            e.Result = instance.GetDuplicates((string)e.Argument);
            Trace.WriteLine("result:" + e.Result);            
        }             

        // This event handler updates the progress.
        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            Trace.WriteLine("progresschanged");
            //resultLabel.Text = (e.ProgressPercentage.ToString() + "%");
            this.progressBar1.Value = e.ProgressPercentage;
        }

        // This event handler deals with the results of the background operation.
        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Cancelled == true)
            {
                Trace.WriteLine("RunWorkerCompleted cancel");
                // resultLabel.Text = "Canceled!";
            }
            else if (e.Error != null)
            {
                Trace.WriteLine("RunWorkerCompleted error "+ e.Error.Message);
                // resultLabel.Text = "Error: " + e.Error.Message;
            }
            else
            {
                // resultLabel.Text = "Done!";
                //resultLabel.Text = e.Result.ToString();
                Trace.WriteLine("RunWorkerCompleted result:" + e.Result);
            }
        }

        private void Form2_FormClosing(object sender, EventArgs e)
        {
            //TODO cancel background work
            if (backgroundWorker1.WorkerSupportsCancellation == true)
            {
                // Cancel the asynchronous operation.
                backgroundWorker1.CancelAsync();
            }
            Application.Exit();
        }

        private void Form2_FormClosed(object sender, EventArgs e)
        {
            //TODO cancel background work
            if (backgroundWorker1.WorkerSupportsCancellation == true)
            {
                // Cancel the asynchronous operation.
                backgroundWorker1.CancelAsync();
            }
            Application.Exit();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
