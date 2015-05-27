/*
 * Seller.cpp
 *
 *  Created on: Feb 20, 2015
 *      Author: CherylRuo
 */

#include"Seller.h"
Seller::Seller(int sellerId, int totalN)
{
	soldCount = 0;
	this->sellerId = sellerId;
	this->totalN = totalN;
	if (sellerId == 0)
		sellerName = "H0";
	else if (sellerId > 0 && sellerId < 4)
	{
		std::ostringstream ss;
		ss << "M"<<sellerId;
		sellerName = ss.str();
	}
	else
	{
		std::ostringstream ss;
		ss << "L" << (sellerId-3);
		sellerName = ss.str();
	}
	pair<int, int> aPair;
	aPair.first = 40;
	aPair.second = 59;
	order.push_back(aPair);
	aPair.first = 30;
	aPair.second = 39;
	order.push_back(aPair);
	aPair.first = 60;
	aPair.second = 69;
	order.push_back(aPair);
	aPair.first = 20;
	aPair.second = 29;
	order.push_back(aPair);
	aPair.first = 70;
	aPair.second = 79;
	order.push_back(aPair);
	aPair.first = 10;
	aPair.second = 19;
	order.push_back(aPair);
	aPair.first = 80;
	aPair.second = 89;
	order.push_back(aPair);
	aPair.first = 0;
	aPair.second = 9;
	order.push_back(aPair);
	aPair.first = 90;
	aPair.second = 99;
	order.push_back(aPair);
}

Seller::~Seller()
{

}

int Seller::getSoldCount()
{
	return soldCount;
}

void Seller::print(string str)
{
	SYSTEMTIME now;
	GetSystemTime(&now);
	double elapsed = now.wSecond - startTime.wSecond;
	if (elapsed < 0)
	{
		elapsed = 60 + elapsed;
	}
	int min = 0;
	int sec = (int)elapsed;
	if (sec >= 60)
	{
		min++;
		sec -= 60;
	}
	std::stringstream ss;
	ss << min << ':' << sec;
	str = ss.str() + " " + str;
	pthread_mutex_lock(&printMutex);
	std::cout << str << "\n";
	pthread_mutex_unlock(&printMutex);
}

int Seller::getSizeOfCustomerQueue()
{
	return customerQueue.size();
}

bool Seller::addCustomerToQueue(Customer* customer)
{
	if (customerQueue.size() >= totalN)
	{
		return false;
	}
	else
	{
		customerQueue.push_back(customer); // add customer to queue
		return true;
	}
}

string Seller::getSellerName()
{
	return sellerName;
}

void Seller::setSellerName(string sellerName)
{
	this->sellerName = sellerName;
}

int Seller::getPriceLevel()
{
	if (sellerId == 0) // high price level: H1
		return 0;
	else if (sellerId > 0 && sellerId < 4) // medium price level: M1, M2, M3
		return 1;
	else // low price level: L1, L2, L3, L4, L5, L6
		return 2;
}

bool Seller::assignSeat() //if seat is avalible, assign it to customer
{
	if (customerQueue.empty())
		return false;

	Customer* customer = customerQueue.front();

	if (getPriceLevel() == 0) //High level
	{
		for (int i = 0; i < 100; i++) // assign seat from front to back
		{
			Seat* serveSeat = seats[i];
			if (serveSeat->checkAvailable() == true)
			{
				std::stringstream ss;
				soldCount++;
				ss << sellerName << std::setfill('0') << std::setw(2) << soldCount;
				serveSeat->setCustomerName(ss.str());
				serveSeat->setSeller(this);
				customerQueue.erase(customerQueue.begin());
				string str = "Customer: " + customer->getName();
				print(str);
				return true;
			}
			else
				continue;
		}
	}

	else if (getPriceLevel() == 1)
	{
		for (int i = 0; i < order.size(); i++) // the number of pairs
		{
			for (int j = order[i].first; j < order[i].second; j++) // range: from the start number to the end number
			{
				Seat* serveSeat = seats[j];
				if (serveSeat->checkAvailable() == true)
				{
					std::stringstream ss;
					soldCount++;
					ss << sellerName << std::setfill('0') << std::setw(2) << soldCount;
					serveSeat->setCustomerName(ss.str());
					serveSeat->setSeller(this);
					customerQueue.erase(customerQueue.begin());
					string str = "Customer: " + customer->getName();
					print(str);
					return true;
				}
				else
					continue;
			}
		}
	}
	else
	{
		for (int i = 99; i >= 0; i--) // assign seat from back to front
		{
			Seat* serveSeat = seats[i];
			if (serveSeat->checkAvailable() == true)
			{
				std::stringstream ss;
				soldCount++;
				ss << sellerName << std::setfill('0') << std::setw(2) << soldCount;
				serveSeat->setCustomerName(ss.str());
				serveSeat->setSeller(this);
				customerQueue.erase(customerQueue.begin());
				string str = "Customer: " + customer->getName();
				print(str);				
				return true;
			}
			else
				continue;
		}
	}
	serviceDone = true; // if no seat can be found, service is done
	return false;
}

void Seller::printSeat()
{
	pthread_mutex_lock(&printMutex);
	std::cout << "\nSeat chart:\n";
	for (int i = 0; i<100; i++)		//PRINT THE SEATchart
	{
		Seat* seat = seats[i];
		if (i % 10 == 0)
		{
			if (!seat->checkAvailable())
			{
				std::cout << "\n" + seat->getCustomerName() + " ";
			}
			else
			{
				std::stringstream ss4;
				ss4 << "  X  ";
				std::string string = ss4.str();
				std::cout << "\n" << string;
			}
		}
		else
		{
			if (!seat->checkAvailable())
			{
				std::cout << seat->getCustomerName() + " ";
			}
			else
			{
				std::stringstream ss;
				ss << "  X  ";
				std::string str = ss.str();
				std::cout << str;
			}
		}
	}
	cout << "\n";


	pthread_mutex_unlock(&printMutex);

}

void Seller::serveCustomer() // get customer from queue to sell ticket and assign seat
{
	int sleepTime = 0;
	if (serviceDone == false)
	{
		if (customerQueue.size() == 0)
		{
			sem_wait(&customerSem[sellerId]); // wait for customer
		}
		pthread_mutex_lock(&sellerQueueMutex[sellerId]);
		// lock the seat before assigning it, and unlock it afterwards
		pthread_mutex_lock(&seatMutex);			
		assignSeat();
		pthread_mutex_unlock(&sellerQueueMutex[sellerId]);
		pthread_mutex_unlock(&seatMutex);
		printSeat();
		if (getPriceLevel() == 0)      // High level
		{
			unsigned int random;
			rand_s(&random);
			random = random % 2;
			int sleepTime = random + 1; // generate 1 or 2 mins
			Sleep(sleepTime*1000);
		}
		else if (getPriceLevel() == 1) // Medium level
		{
			unsigned int random;
			rand_s(&random);
			random = random % 3;
			sleepTime = random + 2; // generate 2 or 3 or 4 mins
			Sleep(sleepTime*1000);
		}
		else                          // Low level
		{
			unsigned int random;
			rand_s(&random);
			random = random % 4;
			int sleepTime = random + 4; // generate 4 or 5 or 6 or 7 mins
			Sleep(sleepTime*1000);
		}
	}
	// Extra credit: impatient customer
	for (int i = 0; i<customerQueue.size(); i++)
	{
		Customer* customer = customerQueue[i];
		customer->addWaitTime(sleepTime);
		if (customer->getWaitTime() >= 10)	// impatient customer
		{
			std::stringstream ss;
			ss << "\n***" << customer->getName() << " have wait" << customer->getWaitTime() << " leave" << this->getSellerName() << " Queue  because of impatient";
			std::string string = ss.str();
			print(string);
			turnAway++;
			impatientCustomer++;
			customerQueue.erase(customerQueue.begin() + i);
			i--;
		}

	}
}
void Seller::printQueue()
{
	while (customerQueue.size()!=0)
	{
		string str = customerQueue.front()->getName();
		print(str);
		customerQueue.erase(customerQueue.begin());
	}
}
