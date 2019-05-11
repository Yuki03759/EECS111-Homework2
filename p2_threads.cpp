#include "p2_threads.h"
#include "utils.h"
#include <assert.h>

extern Fittingroom room;
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
extern struct timeval t_global_start;

using namespace std;


void *createPerson(void *parm){
    struct timeval t_current;
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
        interval = rand()%5+1;
        
        room.waitList.push_back(*p);   
        
        pthread_mutex_unlock(&mutex_1);
        usleep(MSEC(interval));
       
        }
    
    pthread_exit(0);
    
}

void *assignPerson(void *parm){
 
    struct timeval t_current;
    vector<Person> temp;
    int num_to_add = 2 * room.get_num();
    int num_added = 0;
    int interval = 0;
   
    
    //remove every person from waiting list and push back to temp
   
    srand(time(NULL));
    
    while(num_added != num_to_add){
        
        pthread_mutex_lock(&mutex_1);
        
        for(int j =0; j < room.waitList.size(); j++){
            
            //add to temp room
            temp.push_back(room.waitList[0]);
            gettimeofday(&t_current, NULL);
            
            cout << "[" << get_elasped_time(t_global_start, t_current) << " ms]";
            if(room.waitList[0].get_gender() == FEMALE)
                room.waitList[0].woman_wants_to_enter();
            else
                room.waitList[0].man_wants_to_enter();
            
            //remove from waitList
            room.waitList.erase(room.waitList.begin());    
        }
           
        pthread_mutex_unlock(&mutex_1);
        //assignment Person part 2 - add people linst
        
        pthread_mutex_lock(&mutex_2);
        if (room.inList.size() == room.get_stall() || temp.size() == 0)
        {
            ;
        }
       else
       {
            while( temp.size() != 0 && room.allowed(temp[0]) ){
                gettimeofday(&t_current, NULL);
                cout << "[" << get_elasped_time(t_global_start, t_current) << " ms]";
                interval = rand()%7+3;
                room.add_person(temp[0]);
                room.printSendRoom(temp[0], interval);
                temp.erase( temp.begin() );
                num_added++;
                usleep(MSEC(interval));
                
            }
       } 
        pthread_mutex_unlock(&mutex_2);
    }
     
  
    pthread_exit(0);
}

void *removePerson(void *parm){
    
    //remove after finished 
    struct timeval t_current;
 
    int num_to_remove = 2 * room.get_num();
    int num_removed = 0;
    int i = 0;
    while(num_removed < num_to_remove ){
        
        pthread_mutex_lock(&mutex_2);
      
        for(int i = 0; i < room.inList.size(); i++){
            
            if(room.inList[i].ready_to_leave() == 1){
                //room.inList.erase(room.inList.begin()+i);
                gettimeofday(&t_current, NULL);
                room.remove_person(i);
                cout << "[" << get_elasped_time(t_global_start, t_current) << " ms]";
                room.printLeaveRoom(room.inList[i]);
                
                num_removed++;
                i--;
            }
        }
        
        if(room.inList.size() == 0)    
        {    
            room.set_status(EMPTY);
        }
        
        pthread_mutex_unlock(&mutex_2);  
    }
    

    pthread_exit(0);
}