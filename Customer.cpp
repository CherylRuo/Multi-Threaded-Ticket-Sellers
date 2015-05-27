/*
 * Customer.cpp
 *
 *  Created on: Feb 20, 2015
 *      Author: CherylRuo
 */

#include"Customer.h"
Customer::Customer(int customerId)
{
	this->customerid = customerId;
	std::ostringstream s;
	s << "Customer_" << customerid;
	customerName = s.str();
	waittime = 0;
}

Customer::~Customer() 
{
	
}

std::string Customer::getName()
{
	return this->customerName;
}

int Customer::getWaitTime()
{
	return waittime;
}

void Customer::addWaitTime(int sleepTime)
{
	waittime = waittime + sleepTime;
}

int Customer::getID()
{
	return customerid;
}
