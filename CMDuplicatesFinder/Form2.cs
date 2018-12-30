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
        private static readonly int RESULT_INDEX_STATUS = 0;
        private static readonly int RESULT_INDEX_CURRENT_STAFF = 1;
        private static readonly int RESULT_INDEX_TO_USER = 2;

        private static readonly string STATUS_LOADING_CSV = "Loading csv file...";
        private static readonly string STATUS_PARSING_CSV = "csv file successfully loaded! Parsing csv file...";
        private static readonly string STATUS_FINDING_DUPLICATES_1 = "csv file successfully parsed! ";
        private static readonly string STATUS_FINDING_DUPLICATES_2 = " staff found. ";
        private static readonly string STATUS_FINDING_DUPLICATES_3 = "Finding duplicates...";
        private static readonly string STATUS_COMPLETED = "Completed! Final result automatically saved into DuplicatesList.txt file.";
        private static readonly string STATUS_LABEL2_COMPLETED = "Duplicate staff found (automatically saved into DuplicatesList.txt file):";
        private static readonly string STATUS_FAILED_OPEN = "Failed to open csv file. Make sure you have exported a valid csv file as per the tool guide instructions. Please restart the tool and try again.";        
        private static readonly string STATUS_ERROR = "An error has occurred. Please restart the tool and try again, following the tool guide instructions. Error message: ";
        private static readonly string STATUS_ERROR_LABEL = "Stopped. An error has occurred.";

        private readonly string path;

        public Form2(string path)
        {
            InitializeComponent();

            this.path = path;

            backgroundWorker1.WorkerReportsProgress = true;
            backgroundWorker1.WorkerSupportsCancellation = true;
            backgroundWorker1.DoWork += new DoWorkEventHandler(backgroundWorker1_DoWork);
            backgroundWorker1.RunWorkerCompleted += new RunWorkerCompletedEventHandler(backgroundWorker1_RunWorkerCompleted);
            backgroundWorker1.ProgressChanged += new ProgressChangedEventHandler(backgroundWorker1_ProgressChanged);

            this.FormClosing += new FormClosingEventHandler(Form2_FormClosing);
            this.FormClosed += new FormClosedEventHandler(Form2_FormClosed);
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            Trace.WriteLine("path:"+path);            

            if (!backgroundWorker1.IsBusy)
            {
                Trace.WriteLine("async trigger");
                // Start the asynchronous operation.
                label1.Text = STATUS_LOADING_CSV;
                backgroundWorker1.RunWorkerAsync(path);
            }

            //TODO: implement pause so it saves the duplicate arrays to a text file and then it can resume when reopen app            
            Trace.WriteLine("async operation triggered");            
        }

        // This event handler is where the time-consuming work is done.
        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            Trace.WriteLine("asyncwork "+ (string)e.Argument);
            BackgroundWorker worker = sender as BackgroundWorker;

            ProgressReporter progressReporter = new ProgressReporter(STATUS_LOADING_CSV);
            worker.ReportProgress(0, progressReporter);

            //creates an instance of the C++ class Class1, which contains the find duplicates code
            ClassLibrary.Class1 instance = new ClassLibrary.Class1();

            if(worker.CancellationPending == true)
            {
                Trace.WriteLine("Cancelation pending");
                e.Cancel = true;
            }
            //calls the c++ functions for reading, parsing and finding duplicates, and 
            //processes the return values to update the UI in C#, then calls again, etc, 
            //until its completed or cancelled
            else if (instance.ReadCSVFile((string)e.Argument))
            {       
                //successfully read csv file

                if (worker.CancellationPending == true)
                {
                    Trace.WriteLine("canceled3");
                    e.Cancel = true;
                }
                else
                {
                    Trace.WriteLine("success reading csv file");
                    progressReporter.SetStatus(STATUS_PARSING_CSV);
                    worker.ReportProgress(0, progressReporter);

                    int totalStaff = instance.ParseCSV();
                    Trace.WriteLine("total staff:" + totalStaff);
                    progressReporter.SetTotalStaff(totalStaff);
                    progressReporter.SetStatus(STATUS_FINDING_DUPLICATES_1 + totalStaff + STATUS_FINDING_DUPLICATES_2 + STATUS_FINDING_DUPLICATES_3);
                    worker.ReportProgress(1, progressReporter);

                    bool completed = false;
                    string[] result;
                    while (!completed)
                    {
                        if (worker.CancellationPending == true)
                        {
                            Trace.WriteLine("canceled");
                            e.Cancel = true;
                            break;
                        }               

                        /*gets a list of duplicate staff from c++ layer and splits it in 3 strings:
                         *the first contains the status ('1' == completed and '0' == in progress), for internal use by the code only
                         *the second is the number of staffs already checked, for internal use by the code only,
                         *and the third is the list to be shown to the user*/
                        result = instance.GetDuplicates().Split(new string[] { "\n" }, 3, StringSplitOptions.None);
                        progressReporter.UpdateResult(result[RESULT_INDEX_TO_USER]);
                        if (Int32.Parse(result[RESULT_INDEX_STATUS]) == 1)
                        {
                            //completed the whole processing, leaves the loop and prints the 
                            //final output to the user via backgroundWorker1_RunWorkerCompleted
                            completed = true;
                            Trace.WriteLine("completed");
                        }
                        else
                        {
                            //still in progress, updates the ui to the user                            
                            int progress = Int32.Parse(result[RESULT_INDEX_CURRENT_STAFF]) + 2;                            
                            worker.ReportProgress(progress, progressReporter);
                            Trace.WriteLine("progress:" + progress);                                                        
                        }
                    }
                    e.Result = progressReporter.GetResult();
                    Trace.WriteLine("Final result:" + e.Result);
                    if (worker.CancellationPending == true)
                    {
                        Trace.WriteLine("canceled4");
                        e.Cancel = true;                        
                    }
                }                
            } else
            {
                //failed to read csv file
                Trace.WriteLine("failed to read csv file");
                if (worker.CancellationPending == true)
                {
                    Trace.WriteLine("canceled2");
                    e.Cancel = true;
                } else {
                    //TODO handle fail
                    Trace.WriteLine(STATUS_FAILED_OPEN);
                    e.Result = STATUS_FAILED_OPEN;                    
                }                
            }

            //releases c++ resources
            instance.releaseAllResources();
        }             

        // This event handler updates the progress.
        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            Trace.WriteLine("progresschanged"+ e.ProgressPercentage);
            ProgressReporter pr = (ProgressReporter)e.UserState;
            if(pr.GetTotalStaff() > 0)
            {
                this.progressBar1.Maximum = pr.GetTotalStaff()+1;
            }
            textBox1.Text = pr.GetResult();
            label1.Text = pr.GetStatus();            
            this.progressBar1.Value = e.ProgressPercentage;       
        }

        // This event handler deals with the results of the background operation.
        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Cancelled == true)
            {
                Trace.WriteLine("RunWorkerCompleted cancel");
                //closes the app on cancel, 
                //dont need to handle anything else because the current progress result is automatically saved in the output file
                Application.Exit();                
            }
            else if (e.Error != null)
            {
                //some unknown error happened. Prints default error message to user and ends processing
                Trace.WriteLine("RunWorkerCompleted error "+ e.Error.Message);                
                                
                textBox1.Text = STATUS_ERROR + e.Error.Message;
                label1.Text = STATUS_ERROR_LABEL;                
                label2.Text = STATUS_ERROR_LABEL;                
                this.progressBar1.Value = this.progressBar1.Maximum;                
            }
            else
            {
                if (e.Result.ToString().Equals(STATUS_FAILED_OPEN))
                {
                    //failed to open csv file. Prints helpful error message to user and ends processing
                    Trace.WriteLine("failed to open csv file");

                    textBox1.Text = STATUS_FAILED_OPEN;
                    label1.Text = STATUS_ERROR_LABEL;
                    label2.Text = STATUS_ERROR_LABEL;
                    this.progressBar1.Value = this.progressBar1.Maximum;
                }
                else
                {
                    //successfully completed the processing. Prints the final result to the user and updates the UI accordingly.
                    Trace.WriteLine("RunWorkerCompleted result:" + e.Result);

                    textBox1.Text = e.Result.ToString();
                    label1.Text = STATUS_COMPLETED;
                    label2.Text = STATUS_LABEL2_COMPLETED;
                    this.progressBar1.Value = this.progressBar1.Maximum;
                }
            }
        }

        private void Form2_FormClosing(object sender, EventArgs e)
        {
            Trace.WriteLine("Form2 closing");
            CloseTool();
        }

        private void Form2_FormClosed(object sender, EventArgs e)
        {
            Trace.WriteLine("Form2 closed");
            CloseTool();
        }

        private void CloseTool()
        {
            if (!backgroundWorker1.IsBusy)
            {
                //the background worker isnt running, this can happen if user closed the app after seeing an error message
                //or after the processing is completed successfully.
                Trace.WriteLine("not doing any processing");
                Application.Exit();
            } 
            else if (backgroundWorker1.WorkerSupportsCancellation == true)
            {
                //Cancels the asynchronous operation. This can happen if user closed the app while it was doing the processing.
                Trace.WriteLine("CancelAsync triggered");                
                backgroundWorker1.CancelAsync();
                //TODO: when it crashes on exception in c++ side, check if the app stops running if the user clicks X
            }
            else
            {                
                Trace.WriteLine("dont support cancelation");
                Application.Exit();
            }

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void progressBar1_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }
    }
}
