#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
 
/* semaphore declaration*/
sem_t chair_sem[3];
sem_t student;
sem_t ta_sleep;
 
/* threads */
pthread_t *Students;  // thread for N students
pthread_t TA; // separate thread for TA
 
int chair_count = 0;
int index_chair = 0;
pthread_mutex_t ChairAccess; // mutex to apply locks on chairs
 
/* this function checks that TA is sleeping and wakes him up when the student comes,
if no student is there i.e chair is empty, unlock chair mutex to allow for occupying a seat,
When the chair is occupied, the chair count decreases which tells that only the number of students can come.
Then TA helps the student */
 
void * TA_check(){ 
   while(1)
   {
       sem_wait(&ta_sleep); // TA is currently sleeping.
       printf("student comes in\n");
       printf("*************** TA has been awakened by a student. ***************\n");
       while(1)
       {
           // when TA is awaken by a student, occupy a space(put lock on chairs)
           pthread_mutex_lock(&ChairAccess);
           if(chair_count == 0) // check if the number of chairs is empty, then unlock the chairs and to let them occupy
           {
               // if the chairs are empty, break the loop.
               pthread_mutex_unlock(&ChairAccess);
               break;
 
           }
 
           // TA has the next student in the chair.
 
           sem_post(&chair_sem[index_chair]); // signal the chair that it has been occupied
           chair_count--;  // chair is being occupied(countdown)
           printf("Student  left his/her chair in the waiting room and went to TA. Remaining Chairs %d\n",3 - chair_count);
           index_chair = (index_chair + 1) % 3;
           pthread_mutex_unlock(&ChairAccess);
           // unlock chair access
           printf("\t TA is currently helping the student.\n");
           sleep(5);
           sem_post(&student);
           usleep(1000);
 
       }
 
   }
 
}
 
/* this function assume that each student spends 10-time quanta with TA
During this time, no student could come. (put mutex on chairs to avoid others accessing the chairs)
students occupy seats until all 3 chairs are full.
When all the chairs are occupied, if there is a student comes at this period of time,
he will return and come back again when the waiting seats become empty*/
 
 
void *Student_Check(void *threadID)
{
   int Time_with_ta;
   while(1)
 
   {
 
       printf("Student %ld doing assignment .\n", (long)threadID);
 
       Time_with_ta = rand() % 10 + 1;; // assume that each student takes 10-time of TA
       sleep(Time_with_ta);       
       printf("Student %ld needs help from the TA\n", (long)threadID);
       pthread_mutex_lock(&ChairAccess); // as student is coming, he sits on the chair(putlocker)
       int count = chair_count;
 
       pthread_mutex_unlock(&ChairAccess);
       if(count < 3)       // if chair count < 3, students will sit and wait.
       {
           if(count == 0)      // If a student sits on the first empty chair, wake up the TA.
               sem_post(&ta_sleep); // wake up TA semaphore
           else
           printf("Student %ld sat on a chair waiting for the TA to finish. \n", (long)threadID);
 
           pthread_mutex_lock(&ChairAccess); // mutex lock, so no one can sit on preoccupied chair
 
           int index = (index_chair + chair_count) % 3;
           chair_count++;
           printf("Student sat on chair.Chairs Remaining: %d\n", 3 - chair_count);
           pthread_mutex_unlock(&ChairAccess);
           // unlock
           sem_wait(&chair_sem[index]); // Student leaves the chair.
           printf("\t Student %ld is getting help from the TA. \n", (long)threadID);
           sem_wait(&student);     // Student waits the next turn.
           printf("Student %ld left TA room.\n",(long)threadID);
 
       }
 
       else
           printf("Student %ld will return at another time. \n", (long)threadID);
 
           // If there are no chairs left.
 
   }
 
}
 
int main(){
 
   int number_of_students = 5;    
 
   int id;
   srand(time(NULL));
   // Initializing Mutex Lock and Semaphores.
 
   sem_init(&ta_sleep, 0, 0);
 
   sem_init(&student, 0, 0);
 
   for(id = 0; id < 3; ++id) // Chairs array of 3 semaphores.
       sem_init(&chair_sem[id], 0, 0);
   pthread_mutex_init(&ChairAccess, NULL);
   Students = (pthread_t*) malloc(sizeof(pthread_t)*number_of_students);
   // Creating TA thread
   pthread_create(&TA, NULL, TA_check, (void*) (long)id); 
   // Creating N student thread(n = 5)
   for(id = 0; id < number_of_students; id++)
       pthread_create(&Students[id], NULL, Student_Check,(void*) (long)id);
   // Waiting for TA thread and N Student threads.
       pthread_join(TA, NULL);
   for(id = 0; id < number_of_students; id++)
pthread_join(Students[id], NULL);
free(Students);
   return 0;
 
}
