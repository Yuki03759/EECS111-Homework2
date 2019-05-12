#include "p2_threads.h"
#include "utils.h"
#include <assert.h>

extern Fittingroom room;
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
extern struct timeval t_global_start;

using namespace std;


// create people and put them into waitList
void *createPerson(void *parm)
{
    
    struct timeval t_current;
    int total_num = 2 * room.get_num();
    int male_cnt = room.get_num();
    int female_cnt = room.get_num();
    
    srand( time(NULL) );
    assert ( room.inList.size() == 0);
    
    for(int i = 0; i < total_num; i++){
        
        // create a person
        Person* p = new Person();
        p->set_time(rand()%7+3);
        p->set_order(i+1);
        
        // randomly assign male or female
        if( (rand()%2 == MALE && male_cnt > 0) || female_cnt == 0){
            p->set_gender(MALE);
            male_cnt--;
        }
        else{
            p->set_gender(FEMALE);
            female_cnt--;
        }
         pthread_mutex_lock(&mutex_1);
        // push to waitList
        room.waitList.push_back(*p);   
        
        pthread_mutex_unlock(&mutex_1);
        
        // sleep between 1ms to 5ms
        int interval = rand()%5+1;
        usleep(MSEC(interval));
    }
    
    pthread_exit(0);
    
}

// 
void *assignPerson(void *parm)
{
    struct timeval t_current;
    vector<Person> temp;
    int num_to_add = 2 * room.get_num();
    int num_added = 0;
   
    //remove every person from waiting list and push back to temp
    
    srand(time(NULL));
    
    while(num_added != num_to_add){
        
        for(int j =0; j < room.waitList.size(); j++){
            
            //add to temp room            
            pthread_mutex_lock(&mutex_1);
            temp.push_back(room.waitList[0]);
            pthread_mutex_unlock(&mutex_1);
            
            // print [Input] A person goes into the queue 
            gettimeofday(&t_current, NULL);
            cout << "[" << get_elasped_time(t_global_start, t_current) << " ms]";
            if(room.waitList[0].get_gender() == FEMALE)
                room.waitList[0].woman_wants_to_enter();
            else
                room.waitList[0].man_wants_to_enter();
            
            //remove from waitList
            room.waitList.erase(room.waitList.begin());    
        }
        
        // inList size is the same as the stall or temp is empty 
        if (room.inList.size() == room.get_stall() || temp.size() == 0)
        {
            ;
        }
        // add a person to inList
        else
        {
            while( temp.size() != 0 && room.allowed(temp[0]) ){
                
              
                
                pthread_mutex_lock(&mutex_2);
                
                //print person goes into fitting room  
                gettimeofday(&t_current, NULL);
                cout << "[" << get_elasped_time(t_global_start, t_current) << " ms]";
                room.printSendRoom(temp[0]);
                
                //add person
                room.add_person(temp[0]);

                //print person goes into fitting room
                cout << "[" << get_elasped_time(t_global_start, t_current) << " ms]";
                room.person_goes(temp[0]);
                
                pthread_mutex_unlock(&mutex_2);     
                
                //remove temp
                temp.erase( temp.begin() );
                
                //increament num_added
                num_added++;
                
            }
        } 
    }
  
    pthread_exit(0);
}

void *removePerson(void *parm)
{   
 
    //remove after finished 
    struct timeval t_current;
 
    int num_to_remove = 2 * room.get_num();
    int num_removed = 0;
    int i = 0;
    
    while(num_removed != num_to_remove ){
      
        pthread_mutex_lock(&mutex_2);
        for(int i = 0; i < room.inList.size(); i++)
        {
            if(room.inList[i].ready_to_leave() )
            {
                gettimeofday(&t_current, NULL);
                cout << "[" << get_elasped_time(t_global_start, t_current) << " ms]";
                if(room.inList[i].get_gender() == FEMALE)
                    room.woman_leaves(i);
                else
                    room.man_leaves(i);
            
                room.remove_person(i);
           
                num_removed++;
                i--;
            }
        }

        pthread_mutex_unlock(&mutex_2);  
        
        if(room.inList.size() == 0)    
        {    
            room.set_status(EMPTY);
        }
        
    }
    pthread_exit(0);
}