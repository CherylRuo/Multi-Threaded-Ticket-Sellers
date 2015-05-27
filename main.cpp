/*
 * main.cpp
 *
 *  Created on: Feb 20, 2015
 *      Author: CherylRuo
 */
#define _CRT_RAND_S
#include"Seller.h"
#include"Seat.h"
#include"Customer.h"
#include<iostream>
#include<mutex>
#include<semaphore.h>
#include<iostream>
#include<sstream>
#include<pthread.h>
#include<iomanip>
#include<cstdlib>
#include"Global.h"
#include<Windows.h>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
using namespace std;

void* buying(void* value);
void assignCustomer(Customer* customer);
void print(string str);

void* selling(void* value)
{
	Seller* seller = (Seller*) value;
	string str = seller->getSellerName() + " Start selling tickets.";
	print(str);
	while (!serviceDone)
	{
		if (seller->getSizeOfCustomerQueue() == 0)
		{
			continue;
		}
		seller->serveCustomer();
	}
	return NULL;
}

void* buying(void* value)
{
	Customer* customer = (Customer*)value;
	unsigned int k;
	rand_s(&k);
	k = k % 60;
	Sleep(k*1000);
	assignCustomer(customer);
	return NULL;
}

void print(string str) // same method as in seller
{
	pthread_mutex_lock(&printMutex);
	std::cout << str << "\n";
	pthread_mutex_unlock(&printMutex);
}

void assignCustomer(Customer* customer) // assign customers to seller queue
{
	arriveCustomer++; // how many customers are served
	if (serviceDone == true)
	{
		string str = customer->getName() + " leaves. Service is done because all tickets are sold out.";
		print(str);
		return;
	}

	// randomly choose one of 10 sellers. 0->High, 1-3->Medium1-3, 4-9->Low4-9
	unsigned int random;
	rand_s(&random);
	random = random % 10;
	pthread_mutex_lock(&sellerQueueMutex[random]); // lock the chosen seller
	Seller* seller = sellerArray[random];
	if (seller->addCustomerToQueue(customer))
	{
		pthread_mutex_unlock(&sellerQueueMutex[random]); // unlock the chosen seller
		sem_post(&customerSem[random]);
		string str = customer->getName() + " enters " + seller->getSellerName() + "'s queue and wait";
		print(str);
		assignedCustomer++;
	}
	else
	{
		pthread_mutex_unlock(&sellerQueueMutex[random]); // unlock the chosen seller
		stringstream ss;
		ss << seller->getSizeOfCustomerQueue();
		string str = ss.str();
		string output = customer->getName() + " can not enter " + seller->getSellerName() + "'s queue. Queue Size is " + str;
		print(output);
	}
}
int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		string str = "please provide number of customers for each sellers";
		print(str);
		return 0;
	}
	int N = atoi (argv[1]);
	// initialize seatMutex
	pthread_mutex_init(&seatMutex, NULL);
	pthread_mutex_init(&printMutex, NULL);
	// generate 100 seats
	for (int i = 0; i < 100; i++)
	{
		Seat* seat = new Seat();
		seats.push_back(seat);
	}
	GetSystemTime(&startTime);
	for (int i = 0; i < 10; i++)
	{
		// initialize sellerQueueMutex
		pthread_mutex_init(&sellerQueueMutex[i], NULL);
		// initialize semaphore
		sem_t sem;
		sem_init(&sem, 0, 0);
		customerSem.push_back(sem);
		
	}
	for (int i = 0; i < 10; i++)
	{
		// create 10 seller threads
		Seller* seller = new Seller(i, N); // N is command line input number of customers
		sellerArray.push_back(seller);
		pthread_t sellerThreadId;
		pthread_attr_t sellerAttr;
		pthread_attr_init(&sellerAttr);
		pthread_create(&sellerThreadId, &sellerAttr, selling, seller);
	}
	// generate 200 customers
	for (int i = 0; i < 300; i++)
	{
		Customer* customer = new Customer(i);
		pthread_t customerThreadId;
		pthread_attr_t customerAttr;
		pthread_attr_init(&customerAttr);
		pthread_create(&customerThreadId, &customerAttr, buying, customer);
	}
	//signal(SIGALRM, timerHandler);
	Sleep(65000);
	cout << "\nAssigned high price customers: " << sellerArray[0]->getSoldCount();
	int countTotal = sellerArray[0]->getSoldCount();
	int count = 0;
	for (int i = 1; i < 4; i++)
	{
		count = count + sellerArray[i]->getSoldCount();
	}
	countTotal += count;
	cout << "\nAssigned medium price customers: " << count;
	count = 0;
	for (int i = 4; i < 10; i++)
	{
		count = count + sellerArray[i]->getSoldCount();
	}
	countTotal += count;
	cout << "\nAssigned low price customers: " << count;

	cout << "\nTurnaway number: " << 300 - countTotal;
	cout << "\nImpatient Customer: " << impatientCustomer;
	int i;
	cin >> i;
}
