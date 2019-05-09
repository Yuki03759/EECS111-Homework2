#ifndef __TYPES_P2_H
#define __TYPES_P2_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>

#define EMPTY        0
#define WOMENPRESENT 1
#define MENPRESENT   2

using namespace std;

enum Gender{
    MALE,
    FEMALE
};

class Person
{

	int gender; // 0: male 1: female
	std::string str_gender;
	struct timeval t_create;
	struct timeval t_start;
	struct timeval t_end;
	long time_to_stay_ms;


	unsigned long order;
	unsigned long use_order;

public:
	Person();

	void set_gender(int data);
	int get_gender(void);

	void set_order(unsigned long data);
	unsigned long get_order(void);

	void set_use_order(unsigned long data);
	unsigned long get_use_order(void);

	void set_time(long data);
	int ready_to_leave(void);

	void start(void);
	void complete(void);
    void printPerson(int gender, long t);
};


// Class for the fittingroom
// You may need to add more class member variables and functions
class Fittingroom {
	
    int status; 
    int num;
	// You need to define the data structure to
    // save the information of people using the fittingroom
	// You can probebly use Standard Template Library (STL) vector
    

public:
	Fittingroom(){
		status = EMPTY;
	}
    vector<Person> waitList;
    vector<Person> inList;
    
    void set_num(int num);
    int get_num(void);
    
    void set_status(int status);
    int get_status(void);
    
    // You need to use this function to print the Fittingroom's status
	void print_status(void);

	// Call by reference
	// This is just an example. You can implement any function you need
	void add_person(Person& p);

    void printVector(vector<Person> v, string str);

    void printSendRoom(vector<Person> fitroom, int gender, long t);
};










#endif
