using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections; // needed for Queue 

namespace _14._02._13_QueueEx
{
    struct BankCustomer
    {
        internal string name;
        internal string bankingActivity;
        internal int accountNumber;
        internal float amount;
    }
    enum BankingActivity
    { deposit, withdrawl, transferFunds }

    class QueueDemo
    {  // Create a customer
        private BankCustomer localBankCustomer;
        // create the Queue
        Queue localBankQueue = new Queue();

        public void QueueCustomers()
        {
            // the customers come in one at a time and get in line 
            // waiting for the first teller to show up

            // The first customer
            localBankCustomer.name = "J P Morgan";
            localBankCustomer.bankingActivity = BankingActivity.deposit.ToString();
            localBankCustomer.accountNumber = 335445;
            localBankCustomer.amount = 5600.00F;

            localBankQueue.Enqueue(localBankCustomer);

            // second customer
            localBankCustomer.name = "Butch Cassidy";
            localBankCustomer.bankingActivity = BankingActivity.transferFunds.ToString();
            localBankCustomer.accountNumber = 555445;
            localBankCustomer.amount = 3500.00F;

            localBankQueue.Enqueue(localBankCustomer);

            // third customer
            localBankCustomer.name = "John Dillinger";
            localBankCustomer.bankingActivity = BankingActivity.withdrawl.ToString();
            localBankCustomer.accountNumber = 12345;
            localBankCustomer.amount = 2000.00F;

            localBankQueue.Enqueue(localBankCustomer);

            // see how many people are in line 
            Console.WriteLine(" ");
            Console.WriteLine("Count of Items in the Queue at the start :" + localBankQueue.Count);
            Console.WriteLine(" ");
            // now we want to remove items from the queue so we can work on them 
            // the items are an object so we need to recognize that 
            // put the queue data in the localBankcustomer structure 

            localBankCustomer = (BankCustomer)localBankQueue.Dequeue();
            // write out 
            Console.WriteLine("Name: " + localBankCustomer.name);
            Console.WriteLine("Activity: " + localBankCustomer.bankingActivity);
            Console.WriteLine("Account Number: " + localBankCustomer.accountNumber);
            Console.WriteLine("Amount: " + localBankCustomer.amount.ToString());
            Console.WriteLine("Count of Items in the Queue after first Dequeue:" + localBankQueue.Count);
            Console.WriteLine("----------------------------------------------------------------");
            Console.WriteLine(" ");
            // now we can go throught the reamainder of the bank's sustomers using a loop 
            do
            {
                localBankCustomer = (BankCustomer)localBankQueue.Dequeue();
                // write out 
                Console.WriteLine("Name: " + localBankCustomer.name);
                Console.WriteLine("Activity: " + localBankCustomer.bankingActivity);
                Console.WriteLine("Account Number: " + localBankCustomer.accountNumber);
                Console.WriteLine("Amount: " + localBankCustomer.amount.ToString());
                Console.WriteLine("Count of Items in the Queue after Dequeue: " + localBankQueue.Count);
                Console.WriteLine("----------------------------------------------------------------");
                Console.WriteLine(" ");
            } while (localBankQueue.Count != 0);

        }
    }
}