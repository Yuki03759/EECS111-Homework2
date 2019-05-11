#include "p2_threads.h"
#include "utils.h"
#include <assert.h>
extern pthread_cond_t  cond;
extern pthread_mutex_t mutex;
extern Fittingroom room;
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;

using namespace std;


void *createPerson(void *parm){
    
    cout << "\ncreatePerson started\n" << endl;
    int total_num = 2 * room.get_num();
    int male_cnt = room.get_num();
    int female_cnt = room.get_num();
    int interval;
    srand( time(NULL) );
    assert ( room.inList.size() == 0);
    for(int i = 0; i < total_num; i++){
        //create a random person
        Person* p = new Person();
        p->set_time(rand()%4+1);
        
        pthread_mutex_lock(&mutex_1);
        
        if( (rand()%2 == MALE && male_cnt > 0) || female_cnt == 0){
            p->set_gender(MALE);
            male_cnt--;
        }
        else{
            p->set_gender(FEMALE);
            female_cnt--;
        }
        
        p->set_order(i+1);
        interval = rand()%4+1;
        p->printPerson();
        usleep(MSEC(interval));
        room.waitList.push_back(*p);   
        
        pthread_mutex_unlock(&mutex_1);
        
    }
    
    cout << "create terminated " << endl;
    pthread_exit(0);
    
}

void *assignPerson(void *parm){
 
    cout << "\nassignPerson started\n" << endl;
    vector<Person> temp;
    int num_to_add = 2 * room.get_num();
    int num_added = 0;
   
    
    //remove every person from waiting list and push back to temp
   
    srand(time(NULL));
    
    while(num_added != num_to_add){
        
        pthread_mutex_lock(&mutex_1);
        
        for(int j =0; j < room.waitList.size(); j++){
            
            //add to temp room
            temp.push_back(room.waitList[0]);
            
            //remove from waitList
            cout << "Person "<< room.waitList[0].get_order();
            cout << " will be put in temp queue" << endl;
            room.waitList.erase(room.waitList.begin());    
        }
           
        pthread_mutex_unlock(&mutex_1);
        //assignment Person part 2 - add people linst
        
        pthread_mutex_lock(&mutex_2);
        if (room.inList.size() == room.get_stall())
        {
            cout << "room full " << room.inList.size() << " out of " << room.get_stall() << endl;
            room.printVector(       room.inList           , "inlist"  );
        }
        else if (temp.size() == 0 )
        {
            //cout << "none to add" << endl;
            ;
        }
       else
       {
            while( temp.size() != 0 && room.allowed(temp[0]) ){
                room.add_person(temp[0]);
                temp.erase( temp.begin() );
                num_added++;
                cout << "person " << room.inList[room.inList.size()-1].get_order() << " removed. Status is " ;
                room.print_status();
            }
       } 
        pthread_mutex_unlock(&mutex_2);
    }
     
    cout << "num_added : " << num_added << endl;
    cout << "num_to_add : " << num_to_add << endl;
    
    cout << "assign terminated " << endl;
  
    pthread_exit(0);
}

void *removePerson(void *parm){
    
    //remove after finished 
 
    int num_to_remove = 2 * room.get_num();
    int num_removed = 0;
    int i = 0;
    while(num_removed < num_to_remove ){
        
        pthread_mutex_lock(&mutex_2);
      
        for(int i = 0; i < room.inList.size(); i++){
            
            if(room.inList[i].ready_to_leave() == 1){
                cout << "Person " << room.inList[i].get_order() << " removed. Status is " ;
                room.print_status();
                room.inList.erase(room.inList.begin()+i);
                num_removed++;
                i--;
            }
        }
        
        if(room.inList.size() == 0)    
        {    
            room.set_status(EMPTY);
            //cout << "Status is ";
            //room.print_status();
        }
        //cout << "num_removed " << num_removed << endl;
        pthread_mutex_unlock(&mutex_2);  
    }
    
    cout << "remove terminated " << endl;

    pthread_exit(0);
}