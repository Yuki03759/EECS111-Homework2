#include "p2_threads.h"
#include "utils.h"

extern pthread_cond_t  cond;
extern pthread_mutex_t mutex;
extern Fittingroom room;
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;

using namespace std;


void *createPerson(void *parm){
    
    int total_num = 2 * room.get_num();
    int male_cnt = room.get_num();
    int female_cnt = room.get_num();
   
    int randNum = 0;
    
    srand( time(NULL) );
    
    
    for(int i = 0; i < total_num; i++){
        
        Person p;
        p.set_order(i+1);
        randNum = rand()%2; 
    
        //create a random person
        if( (randNum == MALE && male_cnt > 0) || female_cnt == 0){
            p.set_gender(MALE);
            male_cnt--;
        }
        else{
            p.set_gender(FEMALE);
            female_cnt--;
        }
        
        pthread_mutex_lock(&mutex_1);
        room.waitList.push_back(p);   
        pthread_mutex_unlock(&mutex_1);
        
    }
    
    cout << "create terminated " << endl;
    pthread_exit(0);
}

void *assignPerson(void *parm){
   
   
    vector<Person> temp;
    int num_to_remove = 2 * room.get_num();
    int num_removed = 0;
    //remove every person from waiting list and push back to temp
    int i = 0;
    
    string g;
    srand(time(NULL));
    
    while(num_removed < num_to_remove){
        
        pthread_mutex_lock(&mutex_1);
        
        for(int i =0; i < room.waitList.size(); i++){
            //add to temp room
            temp.push_back(room.waitList[i]);
            g = room.waitList[i].get_gender() == 0 ? "MALE" : "FEMALE";
            
            //remove from waitList
            room.waitList.erase(room.waitList.begin());
            num_removed++;
            i++;
        }
        
        pthread_mutex_unlock(&mutex_1);
    }
    
    // add whole needs to add
   
    int num_to_add = 2 * room.get_num();
    int num_added = 0;
    int interval = 0;
    
    while(num_added < num_to_add){
        pthread_mutex_lock(&mutex_2);
        
        for(int i = 0; i < temp.size(); i++){
            
            if(room.get_status() == EMPTY){
                room.inList.push_back(temp[i]);
            }
            else if(room.get_status() == MENPRESENT){
                if(temp[i].get_gender() == MALE){
                    room.inList.push_back(temp[i]);
                }
            }
            else if(room.get_status() == WOMENPRESENT){
                if(temp[i].get_gender() == FEMALE){
                    room.inList.push_back(temp[i]);
                }
            }
            else{
                cout << "not EMPTY, MANPRESENT, WOMANPRESNET" << endl;
            }
            
            //room.printSendRoom(room.inList, temp[i].get_gender(), interval);
            temp.erase(temp.begin());
        }
        num_added++;
       
        pthread_mutex_unlock(&mutex_2);
    }
    
    
    cout << "assign terminated " << endl;
   
    pthread_exit(0);
}

void *removePerson(void *parm){
    
    pthread_mutex_lock(&mutex_2);
    //remove after finished 
    
    pthread_mutex_unlock(&mutex_2);
    
    //cout << "remove terminated " << endl;
    pthread_exit(0);
}