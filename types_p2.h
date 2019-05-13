#ifndef __TYPES_P2_H
#define __TYPES_P2_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <assert.h>

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
    unsigned long get_time(void);
    
	int ready_to_leave(void);
    void woman_wants_to_enter();
    void man_wants_to_enter();
    void start(void);
	void complete(void);
    void printPerson();
};


// Class for the fittingroom
// You may need to add more class member variables and functions
class Fittingroom {
	
    int status; 
    int num;
    int stall;
    int num_men;
    int num_women;
    int total;
    bool status_changed;
	// You need to define the data structure to
    // save the information of people using the fittingroom
	// You can probebly use Standard Template Library (STL) vector
    

public:
	Fittingroom(){
		status = EMPTY;
        num_men = 0;
        num_women = 0;
        total = 0;
        status_changed = false;
	}
    vector<Person> waitList;
    vector<Person> inList;
    
    void set_num(int num);
    int get_num(void);
    
    void set_stall(int stall);
    int get_stall(void);
    
    void set_status(int status);
    int get_status(void);
    
    // You need to use this function to print the Fittingroom's status
	void print_status(void);

	void change_status(Person& p);
    
    void add_person(Person& p);

    void remove_person(int i);
    
    bool allowed(Person& p);
    
    void printSendRoom(Person &p);
    
    void printLeaveRoom(Person &p);
    void woman_leaves(Person &p);
	void man_leaves(Person &p);
    void person_goes(Person &p);
};










#endif
