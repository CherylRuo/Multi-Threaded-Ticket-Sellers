/*
 * Seat.cpp
 *
 *  Created on: Feb 20, 2015
 *      Author: CherylRuo
 */

#include"Seat.h"
#include<string>
#include<vector>
std::string customerName;

Seat::Seat()
{
	customerName = "empty";
}

Seat::~Seat()
{

}
void Seat::setSeller(Seller* seller)
{
	this->seller = seller;
}
// check seat availability
bool Seat::checkAvailable()
{
	if (customerName != "empty")
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Seat::setCustomerName(string customerName)
{
	this->customerName = customerName;
}

string Seat::getCustomerName()
{
	return customerName;
}
