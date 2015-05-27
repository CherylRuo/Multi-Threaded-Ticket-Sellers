/*
 * Seat.h
 *
 *  Created on: Feb 20, 2015
 *      Author: CherylRuo
 */
#ifndef SEAT_H_
#define SEAT_H_
#include<string>
#include<vector>
using namespace std;
class Seller;
class Customer;
class Seat {
public:
	Seat();
	~Seat();
	bool checkAvailable();
	void setSeller(Seller* seller);
	void setCustomerName(string customerName);
	string getCustomerName();
private:
	Seller* seller;
	std::string customerName;
};
#endif /* SEAT_H_ */
