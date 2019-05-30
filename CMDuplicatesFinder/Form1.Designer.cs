namespace CMDuplicatesFinder
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
            this.button1 = new System.Windows.Forms.Button();
            this.helloWorldLabel = new System.Windows.Forms.Label();
            this.buttonBrowse = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.labelFilename = new System.Windows.Forms.Label();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.label1 = new System.Windows.Forms.Label();
            this.linkLabel1 = new System.Windows.Forms.LinkLabel();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button1.Location = new System.Drawing.Point(150, 122);
            this.button1.Margin = new System.Windows.Forms.Padding(2);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(249, 49);
            this.button1.TabIndex = 2;
            this.button1.Text = "Find possible duplicates!";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // helloWorldLabel
            // 
            this.helloWorldLabel.AutoSize = true;
            this.helloWorldLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 16F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.helloWorldLabel.Location = new System.Drawing.Point(84, 20);
            this.helloWorldLabel.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.helloWorldLabel.Name = "helloWorldLabel";
            this.helloWorldLabel.Size = new System.Drawing.Size(378, 26);
            this.helloWorldLabel.TabIndex = 3;
            this.helloWorldLabel.Text = "Welcome to CMDuplicatesFinder v1.0";
            // 
            // buttonBrowse
            // 
            this.buttonBrowse.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonBrowse.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonBrowse.Location = new System.Drawing.Point(405, 85);
            this.buttonBrowse.Name = "buttonBrowse";
            this.buttonBrowse.Size = new System.Drawing.Size(93, 25);
            this.buttonBrowse.TabIndex = 4;
            this.buttonBrowse.Text = "Browse";
            this.buttonBrowse.UseVisualStyleBackColor = true;
            this.buttonBrowse.Click += new System.EventHandler(this.buttonBrowse_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.labelFilename);
            this.groupBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.Location = new System.Drawing.Point(51, 72);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(348, 45);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Path to the exported database .csv file:";
            this.groupBox1.Enter += new System.EventHandler(this.groupBox1_Enter);
            // 
            // labelFilename
            // 
            this.labelFilename.AutoEllipsis = true;
            this.labelFilename.Location = new System.Drawing.Point(7, 20);
            this.labelFilename.Name = "labelFilename";
            this.labelFilename.Size = new System.Drawing.Size(329, 18);
            this.labelFilename.TabIndex = 0;
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.DefaultExt = "csv";
            this.openFileDialog1.Filter = "Exported database file (*.csv)|*.csv";
            this.openFileDialog1.Title = "Select an exported database .csv file";
            this.openFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog1_FileOk);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(52, 210);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(157, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "Need help? Visit the official site:";
            // 
            // linkLabel1
            // 
            this.linkLabel1.AutoSize = true;
            this.linkLabel1.Location = new System.Drawing.Point(215, 210);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new System.Drawing.Size(265, 13);
            this.linkLabel1.TabIndex = 7;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "https://github.com/MScientistCM/CMDuplicatesFinder";
            this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(544, 232);
            this.Controls.Add(this.linkLabel1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonBrowse);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.helloWorldLabel);
            this.Controls.Add(this.button1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "Form1";
            this.Text = "CMDuplicatesFinder";
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label helloWorldLabel;
        private System.Windows.Forms.Button buttonBrowse;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label labelFilename;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.LinkLabel linkLabel1;
    }
}

