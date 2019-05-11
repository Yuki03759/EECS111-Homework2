#include "types_p2.h"
#include "utils.h"

using namespace std;

void Person::set_gender(int data) { gender = data; }
int Person::get_gender(void)      { return gender; }

void Person::set_order(unsigned long data) { order = data; }
unsigned long Person::get_order(void)      { return order; }

void Person::set_use_order(unsigned long data) { use_order = data; }
unsigned long Person::get_use_order(void)      { return use_order; }

void Person::set_time(long data) { time_to_stay_ms = data; }

int Person::ready_to_leave(void) {
	struct timeval t_curr;
	gettimeofday(&t_curr, NULL);

	if (get_elasped_time(t_start, t_curr) >= time_to_stay_ms) { return 1; }
	else { return 0; }
}

void Person::start(void) {
	gettimeofday(&t_start, NULL);
//	printf("(%lu)th person enters the fittingroom: \n", order);
//	printf(" - (%lu) milliseconds after the creation\n", get_elasped_time(t_create, t_start));
}

void Person::complete(void) {
	gettimeofday(&t_end, NULL);
	printf("(%lu)th person comes out of the fittingroom: \n", order);
	printf(" - (%lu) milliseconds after the creation\n", get_elasped_time(t_create, t_end));
	printf(" - (%lu) milliseconds after using the fittingroom\n", get_elasped_time(t_start, t_end));
}

void Person::printPerson(){
    
    string g = (gender == 0) ? "MALE" : "FEMALE";
    cout << "[input] A person (" << g << ") goes into the queue stays " << time_to_stay_ms << "ms" << endl;
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
void Fittingroom::print_status(void) {
	printf("Print fittingroom status\n");
    cout << "Status : " << status << endl;
}

void Fittingroom::change_status(Person& p)
{
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
}

void Fittingroom::add_person(Person& p) 
{
	inList.push_back(p);
    p.start();
    
    //change status
    change_status(p);
   
}



void Fittingroom::printVector(vector<Person> v, string str){
    
    cout << str << endl;
    cout << "Gender\tOrder" << endl;
    cout << "-------------------" << endl;
    for(int i = 0; i < v.size(); i++){
        if(v[i].get_gender() == MALE)
            cout << "Male" <<'\t';
        else
            cout << "Female" << '\t';
        cout << v[i].get_order() << endl; 
    }
}


void Fittingroom::printSendRoom(vector<Person> fitroom, int gender, long t){
    
    string g = (gender == 0) ? "MALE" : "FEMALE";
    cout << "[" << "?" << " ms][Queue] Send (" << g << ") into the fitting room"
          << "(stay " << t << "ms),"
           << "Status:" << endl;
    cout << "Total:" << "x (Men: x, Women: x)" << endl;
}
