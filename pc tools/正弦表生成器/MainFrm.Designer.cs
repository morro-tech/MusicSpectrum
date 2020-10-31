namespace FFT正弦表生成器
{
    partial class MainFrm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainFrm));
            this.txtOuput = new System.Windows.Forms.TextBox();
            this.groupBox = new System.Windows.Forms.GroupBox();
            this.udfactors = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.btnCopy = new System.Windows.Forms.Button();
            this.udPoint = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.btnOk = new System.Windows.Forms.Button();
            this.groupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udfactors)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udPoint)).BeginInit();
            this.SuspendLayout();
            // 
            // txtOuput
            // 
            this.txtOuput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtOuput.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(227)))), ((int)(((byte)(234)))), ((int)(((byte)(236)))));
            this.txtOuput.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.txtOuput.Location = new System.Drawing.Point(20, 32);
            this.txtOuput.Margin = new System.Windows.Forms.Padding(5);
            this.txtOuput.Multiline = true;
            this.txtOuput.Name = "txtOuput";
            this.txtOuput.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtOuput.Size = new System.Drawing.Size(672, 543);
            this.txtOuput.TabIndex = 6;
            this.txtOuput.Text = " ";
            // 
            // groupBox
            // 
            this.groupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox.Controls.Add(this.udfactors);
            this.groupBox.Controls.Add(this.label2);
            this.groupBox.Controls.Add(this.btnCopy);
            this.groupBox.Controls.Add(this.udPoint);
            this.groupBox.Controls.Add(this.label1);
            this.groupBox.Controls.Add(this.btnOk);
            this.groupBox.Location = new System.Drawing.Point(702, 21);
            this.groupBox.Margin = new System.Windows.Forms.Padding(5);
            this.groupBox.Name = "groupBox";
            this.groupBox.Padding = new System.Windows.Forms.Padding(5);
            this.groupBox.Size = new System.Drawing.Size(225, 554);
            this.groupBox.TabIndex = 1;
            this.groupBox.TabStop = false;
            this.groupBox.Text = "选项";
            // 
            // udfactors
            // 
            this.udfactors.Location = new System.Drawing.Point(137, 68);
            this.udfactors.Margin = new System.Windows.Forms.Padding(5);
            this.udfactors.Maximum = new decimal(new int[] {
            65536,
            0,
            0,
            0});
            this.udfactors.Name = "udfactors";
            this.udfactors.Size = new System.Drawing.Size(76, 29);
            this.udfactors.TabIndex = 1;
            this.udfactors.Value = new decimal(new int[] {
            256,
            0,
            0,
            0});
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(26, 70);
            this.label2.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(83, 21);
            this.label2.TabIndex = 6;
            this.label2.Text = "放大倍数 :";
            // 
            // btnCopy
            // 
            this.btnCopy.Location = new System.Drawing.Point(30, 189);
            this.btnCopy.Margin = new System.Windows.Forms.Padding(5);
            this.btnCopy.Name = "btnCopy";
            this.btnCopy.Size = new System.Drawing.Size(183, 39);
            this.btnCopy.TabIndex = 5;
            this.btnCopy.Text = "复制";
            this.btnCopy.UseVisualStyleBackColor = true;
            this.btnCopy.Click += new System.EventHandler(this.btnCopy_Click);
            // 
            // udPoint
            // 
            this.udPoint.Location = new System.Drawing.Point(137, 34);
            this.udPoint.Margin = new System.Windows.Forms.Padding(5);
            this.udPoint.Maximum = new decimal(new int[] {
            65536,
            0,
            0,
            0});
            this.udPoint.Name = "udPoint";
            this.udPoint.Size = new System.Drawing.Size(76, 29);
            this.udPoint.TabIndex = 0;
            this.udPoint.Value = new decimal(new int[] {
            128,
            0,
            0,
            0});
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(26, 36);
            this.label1.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(83, 21);
            this.label1.TabIndex = 3;
            this.label1.Text = "生成点数 :";
            // 
            // btnOk
            // 
            this.btnOk.Location = new System.Drawing.Point(30, 120);
            this.btnOk.Margin = new System.Windows.Forms.Padding(5);
            this.btnOk.Name = "btnOk";
            this.btnOk.Size = new System.Drawing.Size(183, 39);
            this.btnOk.TabIndex = 4;
            this.btnOk.Text = "生成";
            this.btnOk.UseVisualStyleBackColor = true;
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // MainFrm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 21F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(941, 589);
            this.Controls.Add(this.groupBox);
            this.Controls.Add(this.txtOuput);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(5);
            this.Name = "MainFrm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "FFT正弦表生成器";
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.MainFrm_Paint);
            this.groupBox.ResumeLayout(false);
            this.groupBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udfactors)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udPoint)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txtOuput;
        private System.Windows.Forms.GroupBox groupBox;
        private System.Windows.Forms.NumericUpDown udPoint;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnOk;
        private System.Windows.Forms.Button btnCopy;
        private System.Windows.Forms.NumericUpDown udfactors;
        private System.Windows.Forms.Label label2;
    }
}

