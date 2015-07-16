using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Collections; // needed for Queue 

namespace _14._02._13_QueueEx2
{
    public struct Coordinate
    {
        public int idx, x, y;

        public Coordinate(int num, int p1, int p2)
        {
            idx = num;
            x = p1;
            y = p2;
        }
    }

    public class Works
    {
        private int piX;
        private int piY;
        private string cont;
        public Works()
        { }
        public Works(Point pi, string content)
        {
            piX = pi.X;
            piY = pi.Y;
            cont = content;
        }
        public int X
        {
            get { return piX; }
            set { piX = value; }
        }
        public int Y
        {
            get { return piY; }
            set { piY = value; }
        }
        public string content
        {
            get { return cont; }
            set { cont = value; }
        }
    }

    public class ThreadPools
    {

        private static volatile ThreadPools instance;
        private static object syncRoot = new Object();
        private ThreadPools() { }

        public static ThreadPools Instance
        {
            get
            {
                if (instance == null)
                {
                    lock (syncRoot)
                    {
                        if (instance == null)
                            instance = new ThreadPools();
                    }
                }
                return instance;
            }
        }


        public Queue TellerLine = new Queue();
        //public int AmountOnDeposit = 10000;
        //public enum BankingActivity
        //{ deposit, withdrawl, transferFunds };


        //public void QueueContentsCopy(Queue localQueue)
        //{
        //    BankCustomer tempCustomer = new BankCustomer();
        //    Queue copyoflocalQueue = new Queue();
        //    // make the copy 
        //    copyoflocalQueue = (Queue)localQueue.Clone();
        //    Console.WriteLine(" ");
        //    Console.WriteLine("View the queue using a copy");
        //    do
        //    {
        //        tempCustomer = (BankCustomer)copyoflocalQueue.Dequeue();
        //        Console.WriteLine("Name: " + tempCustomer.name + ",  Activity: " + tempCustomer.bankingActivity + ",  Account no: " + tempCustomer.accountNumber.ToString() + ", Amount $" + tempCustomer.amount.ToString());
        //    } while (copyoflocalQueue.Count != 0);
        //}
        //public void QueueContentsEnum(Queue localQueue)
        //{
        //    BankCustomer tempCustomer = new BankCustomer();
        //    // get the built in enumerator
        //    System.Collections.IEnumerator en = localQueue.GetEnumerator();
        //    Console.WriteLine(" ");
        //    Console.WriteLine("View the queue using an enumerator");
        //    while (en.MoveNext())
        //    {
        //        tempCustomer = (BankCustomer)en.Current;
        //        Console.WriteLine("Name: " + tempCustomer.name + ",  Activity: " + tempCustomer.bankingActivity + ",  Account no: " + tempCustomer.accountNumber.ToString() + ", Amount $" + tempCustomer.amount.ToString());

        //    }
        //}
        //public void QueuePeek(Queue localQeue)
        //{
        //    BankCustomer tempCustomer = new BankCustomer();
        //    tempCustomer = (BankCustomer)localQeue.Peek();
        //    Console.WriteLine("The next Customer in line: " + tempCustomer.name);
        //}

        //public void ProcessCustomerRequest(BankCustomer customer)
        //{
        //    Console.WriteLine("Customer: " + customer.name);
        //    Console.WriteLine("Activity: " + customer.bankingActivity);
        //    if ((customer.bankingActivity == "deposit") | customer.bankingActivity == "transferFunds")
        //    {
        //        AmountOnDeposit += customer.amount;
        //        Console.WriteLine("Amount on Deposit: " + AmountOnDeposit);
        //    }
        //    if (customer.bankingActivity == "withdrawl" && (customer.name != "John Dillinger"))
        //    {
        //        AmountOnDeposit -= customer.amount;
        //        Console.WriteLine("Amount on Deposit: " + AmountOnDeposit);
        //    }
        //    if ((customer.bankingActivity == "withdrawl") && (customer.name == "John Dillinger"))
        //    {
        //        AmountOnDeposit = 0;
        //        Console.WriteLine("Big Bank Robery !!!    Amount on Deposit: " + AmountOnDeposit);
        //    }
        //    Console.WriteLine("------------------------------------------------");
        //}
    }
}
