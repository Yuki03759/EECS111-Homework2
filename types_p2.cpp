#include "types_p2.h"
#include "utils.h"

using namespace std;

void Person::set_gender(int data) { gender = data; }
int Person::get_gender(void)      { return gender; }

void Person::set_order(unsigned long data) { order = data; }
unsigned long Person::get_order(void)      { return order; }

void Person::set_use_order(unsigned long data) { use_order = data; }
unsigned long Person::get_use_order(void)      { return use_order; }

void Person::set_time(long data)        { time_to_stay_ms = data; }
unsigned long Person::get_time(void)    { return time_to_stay_ms; }

void Person::start(void)    { gettimeofday(&t_start, NULL);}
void Person::complete(void) { gettimeofday(&t_end, NULL);}

int Person::ready_to_leave(void) 
{
	struct timeval t_curr;
	gettimeofday(&t_curr, NULL);

	if (get_elasped_time(t_start, t_curr) >= time_to_stay_ms) { return 1; }
	else { return 0; }
}


void Person::woman_wants_to_enter()
{    
    string g = (gender == 0) ? "Man" : "Woman";
    cout << "[input] A person ("  << g <<  ") goes into the queue" << endl;
}

void Person::man_wants_to_enter(){
    
    string g = (gender == 0) ? "Man" : "Woman";
    cout << "[input] A person ("  << g <<  ") goes into the queue" << endl;
}


Person::Person() {
	gettimeofday(&t_create, NULL);
}

void Fittingroom::set_num(int data) { num = data; }
int Fittingroom::get_num(void)      { return num; }

void Fittingroom::set_stall(int data) { stall = data; }
int Fittingroom::get_stall(void)      { return stall; }

void Fittingroom::set_status(int data) { status = data; }
int Fittingroom::get_status(void)     { return status; }

// You need to use this function to print the Fittingroom's status
void Fittingroom::print_status(void) 
{
    string str;
    if(status == 0)
    {
        str = "EMPTY";
    }
    else if(status == 1)
    {
        str = "WOMENPRESENT";
    }
    else
    {
        str = "MENPRESENT";
    }
        
        
    cout << str << endl;
}

void Fittingroom::change_status(Person& p)
{
    // false == 0 & true == 1
    int old_status = status;
    
    if(status == EMPTY)
    {
        if(p.get_gender() == MALE)
        {
            //change status to MANPRESENT
            status = MENPRESENT;
        }
        else
        {
            //change status to WOMENPRESENT
            status = WOMENPRESENT;
        }
    }
    
    if(inList.size() == 0)
    {
        status = EMPTY;
    }
    
    if (old_status != status)
        status_changed = true;
    else
        status_changed = false;
}


void Fittingroom::add_person(Person& p) 
{
	inList.push_back(p);
    inList[inList.size()-1].start();
    
    if(p.get_gender() == MALE){
        num_men++;
    }
    else
        num_women++;
    
    total++;
    
    //change status
    change_status(p);
   
}


void Fittingroom::remove_person(int i)
{
    assert(inList[i].ready_to_leave());
    //male or female count
    if(inList[i].get_gender() == MALE)
        num_men--;
    else
        num_women--;
    total--;
    
    // remove the person
    inList.erase(inList.begin() + i);
    
    // change status
    change_status(inList[i]);
    
}

bool Fittingroom::allowed(Person& p)
{
    if( inList.size() == stall )
        return false;
    
    return 
          (p.get_gender() == 0 && status == MENPRESENT)
        ||
          (p.get_gender() == 1 && status == WOMENPRESENT)
        || 
          (status == EMPTY);
          
}


void Fittingroom::printSendRoom(Person &p){
    
    string g = (p.get_gender() == 0) ? "Man" : "Woman";
    string s;
    
    if(status == EMPTY)
        s = "EMPTY";
    else if (status == MENPRESENT)
        s = "MANPRESENT";
    else
        s = "WOMANPRESENT";
    
    cout << "[Queue] Send (" << g  <<  ") into the fitting room"
          << " (stay " << p.get_time() << " ms),"
           << " Status: ";
    cout << "Total: " << total << " (Men: " << num_men <<
            " Women: " << num_women << ")" << endl;
}

void Fittingroom::person_goes(Person &p){
    string g = (p.get_gender() == 0) ? "Man" : "Woman";
    string str_status;
    
    cout << "[fitting room] (" << g  << ") goes into the fitting room (Stay " << p.get_time() 
            << "ms), ";
    
    cout << "Status: Total: " << total << 
            " (Men: " << num_men << ", Women: " << num_women << ")" << endl;
  
}


void Fittingroom::woman_leaves(Person &p)
{
    assert(p.ready_to_leave());
    string g = (p.get_gender() == 0) ? "Man" : "Woman";
    string s;
    
    if(status == EMPTY)
        s = "EMPTY";
    else if (status == MENPRESENT)
        s = "MANPRESENT";
    else
        s = "WOMANPRESENT";
    
    cout << "[fitting room] (" << g  << ") left the fitting room. ";
    
    if(status_changed == true)
        cout << "Status is changed, Status is (" << s << "): ";
    else
        cout << "State is (" << s << "): ";
    
    cout << "Total: " << total << " (Men: " << num_men <<
            " Women: " << num_women << ")" << endl;
}    

void Fittingroom::man_leaves(Person &p)
{    
    assert(p.ready_to_leave());
    string g = (p.get_gender() == 0) ? "Man" : "Woman";
    string str;
    int s;
    
    if(status == EMPTY)
        str = "EMPTY";
    else if (status == MENPRESENT)
        str = "MANPRESENT";
    else
        str = "WOMANPRESENT";
    cout << "[fitting room] (" << g  << ") left the fitting room. ";
    
    if(status_changed == true)
        cout << "Status is changed, Status is (" << str << "): ";
    else
        cout << "Status is (" << str << "): ";
    
    cout << "Total: " << total << " (Men: " << num_men <<
            " Women: " << num_women << ")" << endl;
}        