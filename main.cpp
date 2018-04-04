#include<cstdarg>
#include<Windows.h>
#include<iostream>
#include <cmath>
#include<ctime>
#define MAX_COUNT 10
#define CHAIRS 5//Numbers of chairs,M+1
using namespace std;
int     waiting = 0;          //numbers of customers who are waiting
char    close_door;         
int     a = 0;            //the count number of customer
int     finish = 0;           //numbers of customers who are finished
HANDLE Mutex = CreateMutex(NULL, FALSE, "Mutex");  //Mutex between lines 
HANDLE barbers = CreateSemaphore(NULL, 1, 1, "barbers"); //Semaphore between lines
HANDLE customers = CreateSemaphore(NULL, 0, CHAIRS, "customers"); //Semaphore between lines
int random()//Create random numbers of customers
{
	srand((int)time(NULL));
	return rand() % 5000;
}

DWORD WINAPI customer(LPVOID pParm2)// Customers
{
	if (ReleaseSemaphore(customers, 1, NULL))//V(customer)
	{
		WaitForSingleObject(Mutex, INFINITE);
		a++;
		cout << "You are  " << a<< "  customers of the day " << endl;
		if (waiting != 0)
		{
			cout << waiting << " customers are waiting^" << endl;
		}
		else if(waiting == 0|| waiting == -1)
			cout << "Barber cutting the hair of customer " <<a<< endl;
		waiting++;
		ResumeThread(customers);//aweking barber
		ReleaseMutex(Mutex);//Release Mutex
		WaitForSingleObject(barbers, INFINITE);  
	}
	else
	{
		a++;
		cout <<"customer "<< a << " is leaving. " << endl; 
	}

	return 0;
}
DWORD WINAPI barber(LPVOID pParm1)//Barbers
{
	while (true)
	{
		WaitForSingleObject(customers, INFINITE);//p(customers)
		WaitForSingleObject(Mutex, INFINITE);   //wait for mutex
		ReleaseSemaphore(barbers, 1, NULL);      //release mutex
		ResumeThread(barbers);                 //awkening customers
		Sleep(5000);                               //haircutting
		finish++;                                
		cout <<"Customer "<< a<< " is finished." << endl;
		waiting--;                             
		ReleaseMutex(Mutex);                   //v(mutex);
	}
	return 0;
}

int main()
{
	
	cout << "We have " << CHAIRS-1 << " chairs in the waiting room. "<< endl;
	HANDLE hThreadCustomer;
	HANDLE hThreadBarder;
	hThreadBarder = CreateThread(NULL, 0, barber, NULL, 0, NULL); 
	while (close_door != 'y')
	{
		Sleep(random());
						
		if (finish >= MAX_COUNT)
		{
			while (waiting != 0)
			{
				Sleep(1000);
				continue;
			}
			cout <<  "Are we closing?" << endl; 
			cin >> close_door;
			if (close_door == 'y')
			{
				cout << "We are closed" << endl;
				system("pause");
				return 0;
			}
			else
			{
				finish = 0;
				a = 0;
				cout << "We are still open." << endl;
			}
		}
		hThreadCustomer = CreateThread(NULL, 0, customer, NULL, 0, NULL);
	}
	return 0;
}
