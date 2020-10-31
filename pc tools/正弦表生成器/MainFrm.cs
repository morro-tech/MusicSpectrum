/******************************************************************************
 * @brief     FFT 正弦查找表生成工具
 *
 * Copyright (c) 2020, <master_roger@sina.com>
 *
 * SPDX-iicense-Identifier: Apache-2.0
 *
 * Change iogs: 
 * Date           Author       Notes 
 * 2020-09-20     Morro        
 ******************************************************************************/
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
using System.Text;
using System;

namespace FFT正弦表生成器
{
    public partial class MainFrm : Form
    {
        public MainFrm()
        {
            InitializeComponent();
            btnOk_Click(null, null);
    }

        /// <summary>
        /// 背景绘制
        /// </summary>

        private void BackGroundPaint(object sender, PaintEventArgs e, ColorBlend cb)
        {
            Graphics g = e.Graphics;
            Rectangle r = e.ClipRectangle;
            using (LinearGradientBrush lgb = new
                LinearGradientBrush(r, Color.Empty, Color.Empty, 90f))
            {
                lgb.InterpolationColors = cb;
                g.FillRectangle(lgb, r);
            }
        }
        /**
         *  判断一个数是2的几次幂
         *  @param n	给定的数 
         */
        private int log2(int n)
        {
            if (n == 1)
                return 0;

            return 1 + log2(n >> 1);
        }
        private void MainFrm_Paint(object sender, PaintEventArgs e)
        {
            ColorBlend cb = new ColorBlend(2);
            cb.Colors = new Color[]
            {
                Color.FromArgb(224, 233, 243), 
                Color.FromArgb(246, 245, 254)
            };
            cb.Positions = new float[] { 0f, 1f };
            BackGroundPaint(sender, e, cb);
        }

        private void btnOk_Click(object sender, System.EventArgs e)
        {
            int N = (int)udPoint.Value;
            if ((N & N - 1) != 0)
            {
                MessageBox.Show("生成点数必须满足是2的N次幂", "提示");
                return;
            }

            StringBuilder sb = new StringBuilder();
            sb.Append(string.Format(
            "#define FFT_N {0} \r\n" +
            "#define FFT_M {1} \r\n\r\n" +
            "/* FFT旋转因子查找表 -----------------------------------------------------------\r\n" + 
            "* sin_tab:cos(2 * pi * r / N) * {2}\r\n" +
            "* cos_tab:sin(2 * pi * r / N) * {2} \r\n" +
            "* 其中[N为FFT点数, r: 0 - N / 2 - 1, 放大{2}倍]\r\n" + 
            "*******************************************************************************/\r\n\r\n"
            , N, log2(N), udfactors.Value));

            /*正弦表*/
            sb.Append(string.Format("static const int sin_tab[FFT_N / 2]="));
            sb.Append("\r\n{\r\n    ");
            //sin
            for (int i = 0; i < N / 2; i++)
            {
                int v = (int)Math.Round (Math.Sin(2 * Math.PI * i  /  N) * (double)udfactors.Value);
                sb.Append(string.Format("{0,4:D}, ", v));
                if (i > 0 && (i + 1) % 0x8 == 0)
                    sb.Append("\r\n    ");
            }            
            sb.Append("\r\n};\r\n\r\n");
            /*余弦表*/
            sb.Append(string.Format("static const int cos_tab[FFT_N / 2]="));
            sb.Append("\r\n{\r\n    ");
            //cos
            for (int i = 0; i < N / 2; i++)
            {
                int v = (int)Math.Round(Math.Cos(2 * Math.PI * i / N) * (double)udfactors.Value);
                sb.Append(string.Format("{0,4:D}, ", v));

                if (i > 0 && (i + 1) % 0x8 == 0)
                    sb.Append("\r\n    ");
            }
            sb.Append("\r\n};");
            txtOuput.Text = sb.ToString();
        }

        private void btnCopy_Click(object sender, System.EventArgs e)
        {
            Clipboard.SetText(txtOuput.Text);
        }
    }
}
