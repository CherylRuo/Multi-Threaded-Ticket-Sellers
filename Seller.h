/*
 * Seller.h
 *
 *  Created on: Feb 20, 2015
 *      Author: CherylRuo
 */

#ifndef SELLER_H_
#define SELLER_H_
#define _CRT_RAND_S
#include <string>
#include <vector>
#include "Global.h"
#include "Customer.h"
#include "Seat.h"
#include <semaphore.h>
#include <pthread.h>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <iomanip> 
#include<stdlib.h>
using namespace std;
class Seller {
public:
	Seller(int sellerId, int totalN);
	~Seller();
	void print(string str);
	void serveCustomer();
	int getPriceLevel();
	bool assignSeat();
	bool addCustomerToQueue(Customer* customer);
	string getSellerName();
	int getSizeOfCustomerQueue();
	void setSellerName(string sellerName);
	void printQueue();
	void printSeat();
	int getSoldCount();

private:
	vector<Customer*> customerQueue;
	int sellerId;
	int totalN;//totalN is total number of customers for each seller;
	vector<pair<int, int>> order;
	int soldCount;
	string sellerName;
};

#endif /* SELLER_H_ */
