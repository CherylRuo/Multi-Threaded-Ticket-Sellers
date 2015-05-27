/*
 * Customer.h
 *
 *  Created on: Feb 20, 2015
 *      Author: CherylRuo
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_
#include<string>
#include<sstream>
using namespace std;
class Customer {
public:
	Customer(int customerId);
	~Customer();
	std::string getName();
	int getID();
	int getWaitTime();
	void addWaitTime(int sleepTime);

private:
	int customerid;
	std::string customerName;
	int waittime;
};
#endif /* CUSTOMER_H_ */