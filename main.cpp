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

struct timeval t_global_start;
Fittingroom room;

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

	pthread_t       tid[2];
    
    room.set_num(each_gender_cnt);
    room.set_stall(number_of_stalls);
    
    
    if(pthread_create(&tid[0], NULL, createPerson, NULL)) {
        fprintf(stderr, "Error creating thread\n");		
    }
    if(pthread_create(&tid[1], NULL, assignPerson, NULL)) {
        fprintf(stderr, "Error creating thread\n");		
    }
    if(pthread_create(&tid[2], NULL, removePerson, NULL)) {
        fprintf(stderr, "Error creating thread\n");		
    }	  
        
    /* wait for the second thread to finish */
    for(int i=0; i < 3; i++){
        if(pthread_join(tid[i], NULL)) {
            fprintf(stderr, "Error joining thread\n");	
        }
    }
    
    //room.printVector(room.inList, "waitList after thread");
    
    
	return 0;


}

