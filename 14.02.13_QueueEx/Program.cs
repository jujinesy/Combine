using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _14._02._13_QueueEx
{
    class Program
    {
        static void Main(string[] args)
        {
            QueueDemo ThomastonBankandTrust;
            ThomastonBankandTrust = new QueueDemo();
            ThomastonBankandTrust.QueueCustomers();
            // pause
            Console.Read();
        }
    }
}
