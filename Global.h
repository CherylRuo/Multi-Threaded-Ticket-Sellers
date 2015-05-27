#ifndef GLOBAL_H // header guards
#define GLOBAL_H
#include<mutex>
#include<semaphore.h>
#include<vector>
#include<pthread.h>
#include"Seat.h"
#include"Seller.h"
#include<windows.h>
using namespace std;
// extern tells the compiler this variable is declared elsewhere
extern bool serviceDone;
extern std::vector<sem_t> customerSem;
extern pthread_mutex_t seatMutex;
extern pthread_mutex_t sellerQueueMutex[10];
extern pthread_mutex_t printMutex;
extern vector<Seat*> seats;
extern vector<Seller*> sellerArray;
extern int arriveCustomer;
extern int assignedCustomer;
extern int impatientCustomer;
extern int turnAway;
extern SYSTEMTIME startTime, endTime;
#endif