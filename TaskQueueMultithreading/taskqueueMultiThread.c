//Buket ŞENTÜRK 220201026
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define BILLION 1E9

const int max_title = 1000;
const int task_count = 3;
const int max_val = 50;


/* Struct for list nodes */
struct list_node_s {
   int    data;
   struct list_node_s* next;
};

/* Struct for task nodes */
struct task_node_s {
   int which_task;
   int option;
   int data;
   struct task_node_s* next;
};

/* Shared variables */
int thread_count;
pthread_mutex_t lock1; //mutex
pthread_mutex_t lock2; //mutex
pthread_cond_t cond;
pthread_cond_t cond2;

int n;    //number of task
int b=0; //boolean value to wait threat until signal


/* Head of linked list */
struct list_node_s* head = NULL;  

/* Head of task queue */
struct task_node_s* tasks_head = NULL;

/* Tail of task queue */
struct task_node_s* tasks_tail = NULL;

/* Usage */
void Usage(char* prog_name);

/* Thread function */
void* Thread_work(void* rank);

/* Task queue functions */
int Empty_queue(void);
int Terminate(long my_rank, int* which_task_p, int* option_p, int* data_p);
void Task_queue(int n);
void Task_enqueue(int which_task, int option, int data);
int Task_dequeue(long my_rank, int* which_task_p, int* option_p, int* data_p);

/* List operations */
int Insert(int value);
void Print(char title[]);
int Search(int value);
int Delete(int value);
void Free_list(void);
int Is_empty(void);

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   //struct timespec requestStart, requestEnd;
   //Start timer
   
   //clock_gettime(CLOCK_REALTIME, &requestStart);
  
   long thread;
   pthread_t* thread_handles;

   if(argc != 3) Usage(argv[0]);
   
   thread_count = strtol(argv[1], NULL, 10);
   n = strtol(argv[2], NULL, 10);   
   
   /* Allocate array for threads */
   thread_handles = malloc(thread_count*sizeof(pthread_t));//create thread array*******************************
  
   /* Initialize mutexes and condition variables */
   pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
   pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
  

   if(pthread_mutex_init(&lock1, NULL) != 0)
   {
      printf("ERROR: Cannot initiate mutex\n");
      return -1;
   }
   if(pthread_mutex_init(&lock2, NULL) != 0)
   {
      printf("ERROR: Cannot initiate mutex\n");
      return -1;
   }

  
   /* Start threads */
   for (thread = 0; thread < thread_count; thread++)//BUK****************************************
   {
    //sleep(1);
      pthread_create(&thread_handles[thread],NULL,Thread_work,(void*)thread);

   }   

   /* Generate tasks */
   Task_queue(n);


   /* Wait for threads to complete */
   for (thread = 0; thread < thread_count; thread++)//BUK****************************************
   {
      pthread_join(thread_handles[thread],NULL);
   }


   //destroy the mutex and cond var
   pthread_mutex_destroy(&lock1);
   pthread_mutex_destroy(&lock2);
   pthread_cond_destroy(&cond);
   pthread_cond_destroy(&cond2);
   Free_list();
  



   //End timer
   //clock_gettime(CLOCK_REALTIME, &requestEnd);

   // Calculate time it took
   //double accum = ( requestEnd.tv_sec - requestStart.tv_sec ) + ( requestEnd.tv_nsec - requestStart.tv_nsec ) / BILLION;
   //printf("Total time spent %lf \n", accum);

   
   return 0;
}  /* main */


/*-------------------------------------------------------------------

 * Purpose:     generate random tasks for the task queue, and 
 *              notify a thread to wake up from condition 
 *              wait and get a task
 */

void Task_queue(int n) {

   pthread_mutex_lock(&lock1);
   for(int i = 0; i < n; i++) {

         Task_enqueue(i, random() % task_count, random() % max_val);
         pthread_cond_signal(&cond); //add task and signal 
         b=1;                          
   }
   pthread_mutex_unlock(&lock1);
  
   pthread_mutex_lock(&lock2);         //lock for end of threads
   pthread_cond_wait(&cond2,&lock2);
   pthread_cond_broadcast(&cond);    //for broadcast
   Print("main:  Final list");
   pthread_mutex_unlock(&lock2);   
}  /* Task_queue */


/*-------------------------------------------------------------------
 * Function:    Empty_queue
 * Purpose:     Determine whether the task queue is empty
 * Return val:  0 task queue not empty
 *              1 otherwise
 */
int Empty_queue(void) {
   if (tasks_head == NULL)
      return 1;
   else
      return 0;
}  /* Empty_queue */


/*-------------------------------------------------------------------
 * Purpose:     insert a new task into task queue
 */
void Task_enqueue(int which_task, int option, int data){
   struct task_node_s* tmp_task = NULL;
   
   tmp_task = malloc(sizeof(struct task_node_s));
   tmp_task->which_task = which_task;
   tmp_task->option = option;
   tmp_task->data = data;
   tmp_task->next = NULL;
   
   if (tasks_tail == NULL) { //task list is empty   
      tasks_head = tmp_task;
      tasks_tail = tmp_task;
   } else {
      tasks_tail->next = tmp_task;
      tasks_tail = tmp_task;
   }
   
}  /* Task_enqueue */


/*-------------------------------------------------------------------
  * Purpose:     take a task from task queue
*/
int Task_dequeue(long my_rank, int* which_task_p, int* option_p, int* data_p){
   struct task_node_s* tmp_tasks_head = tasks_head;
   
   if (tmp_tasks_head == NULL) {
      //printf("Th %ld > Queue empty\n", my_rank);
      return 0;
   }
   
   *which_task_p = tmp_tasks_head->which_task;
   *option_p = tmp_tasks_head->option;
   *data_p = tmp_tasks_head->data;
   
   if (tasks_tail == tasks_head){ //last task
      tasks_tail = tasks_tail->next;

   }
   
   tasks_head = tasks_head->next;
   free(tmp_tasks_head);
   
   return 1;
}  /* Task_dequeue */

/*-------------------------------------------------------------------
 * Purpose:     When main thread signals a thread
 *              carry out a linked list operation
 */
void *Thread_work(void* rank) {
   long my_rank = (long) rank; 
   char title[max_title];
   int option = 0, data = 0, which_task;

   pthread_mutex_lock(&lock1);   //lock for dequeu task 

   while(!Terminate(my_rank, &which_task, &option, &data)) // terminate = 0
   {  
      pthread_cond_signal(&cond);

      switch (option) {
         case 0:
            if (Insert(data)){
               printf("Thread %ld: task %d: %d is inserted\n", 
                     my_rank, which_task, data);
            }

            else{
               printf("Thread %ld: task %d: %d cannot be inserted\n", 
                     my_rank, which_task, data);
            }
            break;
         case 1:
            if (Delete(data))
               printf("Thread %ld: task %d:  %d is deleted\n", 
                     my_rank, which_task, data);
            else
               printf("Thread %ld: task %d:  %d cannot be deleted\n", 
                     my_rank, which_task, data);
            break;
         case 2:
            if (Search(data))
               printf("Thread %ld: task %d:  %d is in the list\n", 
                     my_rank, which_task, data);
            else
               printf("Thread %ld: task %d:  %d is not in the list\n", 
                     my_rank, which_task, data);
            break;
         default:
            sprintf(title, "Th %ld:  task %d:  print list",
                  my_rank, which_task);
            Print(title);
            break;  
      }
      if(thread_count!=1){
         pthread_cond_wait(&cond, &lock1);
      }
   }
   pthread_mutex_unlock(&lock1);
   pthread_cond_signal(&cond2);
  
   return NULL;
}

/*-------------------------------------------------------------------
 * Purpose:     Wake up from main thread call and take a task
 *              from task queue
 */
int Terminate(long my_rank, int* which_task_p, int* option_p, int* data_p) {
    
   if(b==0) //wait for main thread to run 
   {
      pthread_cond_wait(&cond, &lock1);
   }
   if(Task_dequeue(my_rank,which_task_p,option_p,data_p) ==1){ 
      return 0;
   }
   else{
      return 1;
   }

   
}  /* Terminate */


/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {
   
   fprintf(stderr, "usage: %s <number of threads> <number of tasks>\n", 
         prog_name);
   exit(0);
}  /* Usage */


/*-----------------------------------------------------------------*/
/* Insert value in correct numerical location into list */
/* If value is not in list, return 1, else return 0 */
int Insert(int value) {
   
   struct list_node_s* curr = head;
   struct list_node_s* pred = NULL;
   struct list_node_s* temp;
   int rv = 1;

   while (curr != NULL && curr->data < value) {
      pred = curr;
      curr = curr->next;
   }
   
   if (curr == NULL || curr->data > value) {
      temp = malloc(sizeof(struct list_node_s));
      temp->data = value;
      temp->next = curr;
      if (pred == NULL)
         head = temp;
      else
         pred->next = temp;
   } else { /* value in list */
      rv = 0;
   }

   
   return rv;
}  /* Insert */

/*-----------------------------------------------------------------*/
void Print(char title[]) {
   struct list_node_s* temp;
   
   printf("%s:\n   ", title);
   
   temp = head;
   while (temp != (struct list_node_s*) NULL) {
      printf("%d ", temp->data);
      temp = temp->next;
   }
   printf("\n");
}  /* Print */


/*-----------------------------------------------------------------*/
int  Search(int value) {


   struct list_node_s* temp;
   
   temp = head;
   while (temp != NULL && temp->data < value)
      temp = temp->next;
   
   if (temp == NULL || temp->data > value) {
#     ifdef DEBUG
      printf("%d is not in the list\n", value);
#     endif

      return 0;
   } else {
#     ifdef DEBUG
      printf("%d is in the list\n", value);
#     endif

      return 1;
   }


}  /* Search */

/*-----------------------------------------------------------------*/
/* Deletes value from list */
/* If value is in list, return 1, else return 0 */
int Delete(int value) {

   struct list_node_s* curr = head;
   struct list_node_s* pred = NULL;
   int rv = 1;
   
   /* Find value */
   while (curr != NULL && curr->data < value) {
      pred = curr;
      curr = curr->next;
   }
   
   if (curr != NULL && curr->data == value) {
      if (pred == NULL) { /* first element in list */
         head = curr->next;
#        ifdef DEBUG
         printf("Freeing %d\n", value);
#        endif
         free(curr);
      } else { 
         pred->next = curr->next;
#        ifdef DEBUG
         printf("Freeing %d\n", value);
#        endif
         free(curr);
      }
   } else { /* Not in list */
      rv = 0;
   }
   
   return rv;
}  /* Delete */

/*-----------------------------------------------------------------*/
void Free_list(void) {
   struct list_node_s* current;
   struct list_node_s* following;
   
   if (Is_empty()) return;
   current = head; 
   following = current->next;
   while (following != NULL) {
#     ifdef DEBUG
      printf("Freeing %d\n", current->data);
#     endif
      free(current);
      current = following;
      following = current->next;
   }
#  ifdef DEBUG
   printf("Freeing %d\n", current->data);
#  endif
   free(current);
}  /* Free_list */

/*-----------------------------------------------------------------*/
int  Is_empty(void) {
   if (head == NULL)
      return 1;
   else
      return 0;
}  /* Is_empty */
