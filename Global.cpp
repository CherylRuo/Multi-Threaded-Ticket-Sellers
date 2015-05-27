#include"Global.h"

pthread_mutex_t seatMutex;
pthread_mutex_t sellerQueueMutex[10];
pthread_mutex_t printMutex;
std::vector<sem_t> customerSem; //send the signal to the sellers if the queue has customer
bool serviceDone = false;
vector<Seat*> seats;
vector<Seller*> sellerArray;
int arriveCustomer = 0; // count of arrive customers
int assignedCustomer = 0; // count of assigned customers
int impatientCustomer = 0;
int turnAway = 0;
SYSTEMTIME startTime, endTime; //timer
