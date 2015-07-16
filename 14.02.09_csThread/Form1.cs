using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace _14._02._09_csThread
{
    public partial class Form1 : Form
    {
        Thread th1;
        Thread th2;
        Thread th3;


        // progress bar
        private delegate void myDelegate(int theValue, int theMax);
        private void updateProgress(int theValue, int theMax)
        {
            if (theMax != 0)
                proStatus.Maximum = theMax;
            proStatus.Value = theValue;
        }


        private void myThread()
        {
            for (int i = 0; i < 100; i++)
            {

                this.Invoke(new myDelegate(updateProgress), new object[] { i * 10, 100 });
                Thread.Sleep(500);
            }
        }



        // Th2

        private delegate void myDelegate2(int theValue, int theMax);
        private void updateProgress2(int theValue, int theMax)
        {
            //Console.WriteLine("1 : " + theValue.ToString());
            listBox1.Items.Add(theValue.ToString());
            listBox1.SetSelected(listBox1.Items.Count - 1, true);
        }

        private void myThread2()
        {
            for (int i = 0; i < 100; i++)
            {
                this.Invoke(new myDelegate2(updateProgress2), new object[] { i, 100 });
                Thread.Sleep(100);
            }
            th2.Abort();
        }


        // Th3
        private delegate void myDelegate3(int theValue, int theMax);
        private void updateProgress3(int theValue, int theMax)
        {
            //Console.WriteLine("2 : " + theValue.ToString());
            listBox1.Items.Add("2 > " + theValue.ToString());
            listBox1.SetSelected(listBox1.Items.Count - 1, true);
        }

        private void myThread3()
        {
            for (int i = 0; i < 100; i += 2)
            {
                this.Invoke(new myDelegate2(updateProgress3), new object[] { i, 50 });
                Thread.Sleep(80);
            }
            th1.Abort();
        }



        private void button1_Click(object sender, EventArgs e)
        {

            th1 = new Thread(new ThreadStart(myThread));
            th2 = new Thread(new ThreadStart(myThread2));
            th3 = new Thread(new ThreadStart(myThread3));

            th1.Start();
            th2.Start();
            th3.Start();

        }
        public Form1()
        {
            InitializeComponent();
        }
    }
}
