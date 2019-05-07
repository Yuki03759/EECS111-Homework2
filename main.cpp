#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include "types_p2.h"
#include "p2_threads.h"
#include "utils.h"

pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct timeval t_global_start;
enum Gender{
    MALE,
    FEMALE,
};

using namespace std;

int main(int argc, char** argv)
{
    int each_gender_cnt;
    int number_of_stalls;
    
    if(argc ==3)
    {    
        each_gender_cnt = atoi(argv[1]);
        number_of_stalls = atoi(argv[2]);
    }
    else
    {
        std::cout << "[ERROR] Expecting 2 arguments, but you got (x)." << std::endl;
        std::cout << "[USAGE] p2_exec <each_gender_cnt> <number_of_stalls>" << std::endl;
        return 0;
    }
    
	// This is to set the global start time
	gettimeofday(&t_global_start, NULL);


	pthread_t       tid = 0;
	int             status = 0;
	int             work = 0;

    //creating each_gender_cnt people
    Person p[2*each_gender_cnt];
    int male_cnt = each_gender_cnt;
    int female_cnt = each_gender_cnt;
    
    for(int i = 0; i < 2*each_gender_cnt; i++){
        
        p[i].set_order(i+1);
        
        //randomly assign gender
        srand( time(NULL) );
        int randNum = rand()%2;
        
        if(randNum == MALE && male_cnt > 0){
            p[i].set_gender(randNum);
            male_cnt--;
        }
        else{
            p[i].set_gender(FEMALE);
            female_cnt--;
        }
        
        cout << "male_cnt : " << male_cnt << endl;
        cout << "female_cnt : " << female_cnt << endl;
        std::cout << "trying to send " << p[i].get_gender() << std::endl;
        
        usleep(MSEC(200));
        p[i].start();
        
        usleep(MSEC(150));
        p[i].complete();
    
	
        if(pthread_create(&tid, NULL, threadfunc, &p[i])) {
            fprintf(stderr, "Error creating thread\n");		
        }
        usleep(MSEC(10));

        for (int i=0; i<5; i++) {
            printf("Wake up thread after (%d) seconds\n", (5-i));
            usleep(MSEC(1000));
        }

        printf("Wake up thread\n");
        status = pthread_cond_signal(&cond);
	


        /* wait for the second thread to finish */
        if(pthread_join(tid, NULL)) {
            fprintf(stderr, "Error joining thread\n");	
        }

    }
    
	return 0;


}

