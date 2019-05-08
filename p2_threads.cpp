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
    
    cout << "number of people is : " << total_num << endl;
    
    srand( time(NULL) );
    cout << "random number " << endl;
    for(int i = 0; i < total_num; i++){
        
        Person p;
        p.set_order(i+1);
        int randNum = rand()%2; 
        cout << randNum << endl;
        
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
   
   
    pthread_mutex_lock(&mutex_1);
/*   
    if(room.get_status() == EMPTY or 
        room.get_status() == MENPRESENT){
        //remove every men from waiting list
        int i = 0;
        while(room.waitList[i].get_gender() == MALE){
            //add to fitting room
            room.inList.push_back(room.waitList[i]);
            //remove men from waitList
            room.waitList.erase(room.waitList.begin());
            i++;
        }
    }
    else
    {
        //remove every women 
        //add to fitting room 
        
        //unlock1
    }
*/    
    pthread_mutex_unlock(&mutex_1);
    
    // add whole needs to add
    pthread_mutex_lock(&mutex_2);
    
    
    pthread_mutex_unlock(&mutex_2);
    cout << "assign terminated " << endl;
   
    pthread_exit(0);
}

void *removePerson(void *parm){
    
    pthread_mutex_lock(&mutex_2);
    //remove after finished 
    
    pthread_mutex_unlock(&mutex_2);
    
    cout << "remove terminated " << endl;
    pthread_exit(0);
}