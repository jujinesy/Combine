using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Threading.Tasks;

namespace _12._10._12_CS_exit
{
    class Program
    {
        static void Main(string[] args)
        {
            Process[] p = Process.GetProcessesByName("PcServer");
            if (p.GetLength(0) > 0)
                p[0].Kill();
        }
    }
}
