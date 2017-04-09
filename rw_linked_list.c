/*
 * Linked List with read write lock
 *
 * Compile : gcc -g -Wall -o rw_linked_list rw_linked_list.c
 * Run : ./rw_linked_list noOfSamples n m noOfThreads mMember mInsert mDelete
 *
 * */
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <pthread.h>
#include <math.h>


#define MAX_THREADS 1024
#define MAX_RANDOM 65535

// Number of samples
int noOfSamples = 0;

// Number of nodes in the linked list
int n = 0;

// Number of random operations in the linked list
int m = 0;

// Number of threads to execute
int noOfThreads = 0;

// Fractions of each operation
float mInsert = 0.0, mDelete = 0.0, mMember = 0.0;

// Total number of each operation
int noOfInsert = 0, noOfDelete = 0, noOfMember = 0;

//Minimum number of samples needed for confidence and accuracy
float minNoOfSamples = 0.0;

struct list_node_s** head = NULL;
pthread_rwlock_t rwlock;

//Node definition
struct list_node_s {
    int data;
    struct list_node_s *next;
};

int Insert(int value, struct list_node_s **head_pp);

int Delete(int value, struct list_node_s **head_pp);

int Member(int value, struct list_node_s *head_p);

double calcTime(clock_t startTime, clock_t endTime);

void validateInput(int argc, char *argv[]);

void populate(int n, struct list_node_s** head_p);

double findStd(double timeArray[], int noOfSamples, double mean);

void *threadOperation(void *id);

void writeOutput(double mean, double std);

int main(int argc, char *argv[]) {

    //Retrieve and validate inputs
    validateInput(argc, argv);
	
	int sampleNumber = 0;
	
	//Time spend for a particular sample
	double sampleTime = 0;
	
	//Total time spent for all the samples
	double totalTime = 0;
	
	//Mean of times of all the samples
	double mean = 0;
	
	//Standard deviation of all the samples
	double std = 0;  
	
	//Time values of all the samples
	double timeArray[noOfSamples]; 
	
	//Time keeping variables 
    clock_t startTime, endTime;	
    
    //Random seed for random values
    srand(time(NULL));

    for(sampleNumber = 0; sampleNumber < noOfSamples; sampleNumber++){
		
		//Defining threads
		pthread_t* threadHandlers;
		threadHandlers = malloc(sizeof(pthread_t) * noOfThreads);
		
		// Calculating the total number of each operation
		noOfInsert = mInsert * m;
		noOfDelete = mDelete * m;
		noOfMember = mMember * m;

		int *threadId;
		threadId = (int *)malloc(sizeof(int) * noOfThreads);

		head = malloc(sizeof(struct list_node_s));
				
		//Populating linked list with random values	
		populate(n,head);

		// Initializing the read write lock
		pthread_rwlock_init(&rwlock, NULL);
		
		//Recording time at operations start
		startTime = clock();

		// Thread Creation
		int i = 0;
		for (i = 0; i < noOfThreads; i++) {
			threadId[i] = i;
			pthread_create(&threadHandlers[i], NULL, threadOperation, (void *) &threadId[i]);			
		}

		// Thread Join
		for (i = 0; i < noOfThreads; i++) {
			pthread_join(threadHandlers[i], NULL);			
		}
		
		//Recording the time at operation ends
		endTime = clock();
    
		// Destroying the read write lock
		pthread_rwlock_destroy(&rwlock);

		//Deallocate the memory of this sample's linked list
		//free(head);
		
		//Deallocate the memory of this sample's thread handlers
		free(threadHandlers);
		
		sampleTime = calcTime(startTime, endTime);
		timeArray[sampleNumber] = sampleTime;
		totalTime = totalTime + sampleTime;
		
		//printf("Average time of sample %d = %f \n", sampleNumber, sampleTime);		
	}
	
	//Calculate mean for all the samples
	mean = totalTime / noOfSamples;
	
	//Calculate Standard deviation for all the samples
	std = findStd(timeArray, noOfSamples, mean);
	
	writeOutput(mean, std);
	//printf("Average time spent = %f\n",mean);
	//printf("Standard Deviation = %f\n",(std));

    return 0;
}


// Linked List Membership function
int Member(int value, struct list_node_s *head_p) {
    struct list_node_s *current_p = head_p;

    while (current_p != NULL && current_p->data < value)
        current_p = current_p->next;

    if (current_p == NULL || current_p->data > value) {
        return 0;
    }
    else {
        return 1;
    }

}

// Linked List Insertion function
int Insert(int value, struct list_node_s **head_pp) {
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;
    struct list_node_s *temp_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;

        if (pred_p == NULL)
            *head_pp = temp_p;
        else
            pred_p->next = temp_p;

        return 1;
    }
    else
        return 0;
}

// Linked List Deletion function
int Delete(int value, struct list_node_s **head_pp) {
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {
            *head_pp = curr_p->next;
            free(curr_p);
        }
        else {
            pred_p->next = curr_p->next;
            free(curr_p);
        }

        return 1;

    }
    else
        return 0;
}

//Retrieve and validate the inputs
void validateInput(int argc, char *argv[]) {

    
    if (argc != 8) {
        printf("Enter the command with arguments noOfSamples, n, m, noOfThreads, mMember, mInsert and mDelete\n");
        exit(0);
    }
    
    //Input retrieval
	noOfSamples = (int) strtol(argv[1], (char**) NULL, 10);
    n = (int) strtol(argv[2], (char**) NULL, 10);
    m = (int) strtol(argv[3], (char**) NULL, 10);
	noOfThreads = (int) strtol(argv[4], (char**) NULL, 10);
    mMember = (float) atof(argv[5]);
    mInsert = (float) atof(argv[6]);
    mDelete = (float) atof(argv[7]);

    //Input validation
	if (noOfSamples <= 0){
		printf("Invalid input for noOfSamples (noOfSamples > 0)\n");
		exit(0);
	} else if (n <= 0){
		printf("Invalid input for n (n > 0)\n");
		exit(0);
	}else if (m <= 0){
		printf("Invalid input for m (m > 0)\n");
		exit(0);

	}else if (mMember + mInsert + mDelete != 1.0){
		printf("Invalid combination of fractions (Total = 1.0)\n");
		exit(0);
	}
	
    //Input validation
    if (noOfThreads <= 0 || noOfThreads > MAX_THREADS) {
        printf("Invalid no. of Threads (0 < noOfThreads < %d)", MAX_THREADS);
        exit(0);
    }
}

// Thread Operations
void *threadOperation(void *threadId) {
	
	//Number of each operations allocated for the thread
	int noOfThreadTotal = 0;
    int noOfThreadInsert = 0;
    int noOfThreadDelete = 0;
    int noOfThreadMember = 0;

    int id = *(int *)threadId; 
    
    //Calculate the number of member operations per thread
    if (noOfMember % noOfThreads <= id) {
        noOfThreadMember = noOfMember / noOfThreads;
    }
    else {
        noOfThreadMember = noOfMember / noOfThreads + 1;
    }
    
    //Calculate the number of insert operations per thread
    if (noOfInsert % noOfThreads <= id) {
        noOfThreadInsert = noOfInsert / noOfThreads;
    }
    else {
        noOfThreadInsert = noOfInsert / noOfThreads + 1;
    }

    //Calculate the number of delete operations per thread
    if (noOfDelete % noOfThreads <= id) {
        noOfThreadDelete = noOfDelete / noOfThreads;
    }
    else{
        noOfThreadDelete = noOfDelete / noOfThreads + 1;
    }

    noOfThreadTotal = noOfThreadInsert + noOfThreadDelete + noOfThreadMember;

    int totalExecuted = 0;
	int memberExecuted = 0;
	int insertExecuted = 0;
	int deleteExecuted = 0;

	int isMemberFinished = 0;
	int isInsertFinished = 0;
	int isDeleteFinished = 0;

    while (totalExecuted < noOfThreadTotal) {

        int randomNumber = rand() % MAX_RANDOM;
        int randomOperation = rand() % 3;

        if (randomOperation == 0 && isMemberFinished == 0) {
            if (memberExecuted < noOfThreadMember) {
                pthread_rwlock_rdlock(&rwlock);
                Member(randomNumber, *head);
                pthread_rwlock_unlock(&rwlock);
                memberExecuted++;
            } else
                isMemberFinished = 1;
        }
        else if (randomOperation == 1 && isInsertFinished == 0) {
            if (insertExecuted < noOfThreadInsert) {
                pthread_rwlock_wrlock(&rwlock);
                Insert(randomNumber, head);
                pthread_rwlock_unlock(&rwlock);
                insertExecuted++;
            } else
                isInsertFinished = 1;
        }
        else if (randomOperation == 2 && isDeleteFinished == 0) {

            if (deleteExecuted < noOfThreadDelete) {
                pthread_rwlock_wrlock(&rwlock);
                Delete(randomNumber, head);
                pthread_rwlock_unlock(&rwlock);
                deleteExecuted++;
            } else
                isDeleteFinished = 1;
        }
        totalExecuted = insertExecuted + memberExecuted + deleteExecuted;   
    }
    return NULL;
}


//Linked list population function
void populate(int n, struct list_node_s** head_p){
	
	int num,i;
	int inserted;
	*head_p = malloc(sizeof(struct list_node_s));
  
	for(i=0; i<n; ){
		num = rand() % MAX_RANDOM;
		
		if(i==0){
			(*head_p)->data = num;
			i++;
		}
		else
			inserted = Insert(num, head_p);
		  
		if(inserted == 1)
			i++;	  
	}
}

//Calculating time between operation start and end
double calcTime(clock_t startTime, clock_t endTime) {
    return (double) (endTime - startTime) / CLOCKS_PER_SEC;
}

//Standard Deviation calculating function
double findStd(double timeArray[], int noOfSamples, double mean){
	
  int i;
  float std=0;
  float temp=0.0;
  
  for(i=0; i<noOfSamples; i++){
    timeArray[i] -= mean;
    temp = timeArray[i]*timeArray[i];
    std += temp;
  }
  
  std = std/noOfSamples;
  std = sqrt(std);
  minNoOfSamples = pow((100*1.96*std)/(5*mean),2);
  //printf("Minimum no. of samples needed = %f\n", minNoOfSamples);
  return std;
}

void writeOutput(double mean, double std) {
	FILE *f = fopen("results.txt", "a"); 
	if (f == NULL)
	{
		printf("Error occured while opening the file \n");        
		exit(1);
	}
	
	fprintf(f,"\n === Read Write Lock === \n\n"); 
	fprintf(f,"Member Fraction= %.2f\n", mMember);  
	fprintf(f,"Insert Fraction = %.3f\n", mInsert);  
	fprintf(f,"Delete Fraction = %.3f\n", mDelete); 
	fprintf(f,"Number of Threads = %d\n", noOfThreads); 
	fprintf(f,"Number of Samples = %d\n", noOfSamples);   
	fprintf(f,"Min number of samples needed = %f\n", minNoOfSamples);  
	fprintf(f,"Mean = %f\n", mean);  
	fprintf(f,"Standard Deviation = %f\n", std);             
	fprintf(f,"________________________________________ \n\n");                   
	
	fclose(f); 
}
