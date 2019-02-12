#include <stdio.h>
#include <stdlib.h>

#include <time.h>

struct tsk_node_s{
	int task_num; //starting from 0 and increase one by one 
	int task_type; // insert:0, delete:1, search:2
	int value;
	struct tsk_node_s* next;
};


struct lst_node_s {
	int data;
	struct lst_node_s* next;
};



// Two glboal variables to store address of front and rear nodes. 
struct tsk_node_s* front = NULL;   //head of Task Queue 
struct tsk_node_s* rear = NULL;		//tail of Task Queue


struct lst_node_s* headList = NULL;   //head of List Queue
struct lst_node_s* tailList = NULL;		//tail of List Queue


int searchInsert(int value){   //to check if the value was added before

	struct lst_node_s* queueList = (struct lst_node_s*)malloc(sizeof(struct lst_node_s*));

	queueList = headList;
	while (queueList != NULL ) {
	    if (queueList->data == value) {

	        return 0;   // returns 0 if previously added
	    }
	    queueList = queueList->next;
	}

	return 1;  //returns 1 if it has not been added before
}

void insert(int numOfTask,int value){   // insert to the list queue 

	struct lst_node_s* queueList = (struct lst_node_s*)malloc(sizeof(struct lst_node_s*));
	int temp = searchInsert(value);   //to find out if it was added before

	if(temp == 1){ //is added because it has not been added before. 
		
		queueList -> data = value;
		queueList -> next = headList;

		headList = queueList;
		
		printf("   Task %d ~ Insert %d ==> %d inserted. \n", numOfTask ,value , value );
	}
	else{
		printf("   Task %d ~ Insert %d ==> %d cannot be inserted. \n", numOfTask ,value , value );
	}	
}

void delete(int numOfTask,int value){    //to delete the requested value


	
	struct lst_node_s* queueList = (struct lst_node_s*)malloc(sizeof(struct lst_node_s*));
	struct lst_node_s* prev = (struct lst_node_s*)malloc(sizeof(struct lst_node_s*));
	int flag = 0 ; 

	prev = NULL;
	queueList=headList;

	while(queueList != NULL ){


		if (queueList-> data == value)
		{
			if(prev==NULL)
                headList = queueList->next;
            else
                prev->next = queueList->next;

            printf("   Task %d ~ Delete %d ==> %d deleted. \n", numOfTask ,value , value );

            flag = 1;
            free(queueList); //need to free up the memory to prevent memory leak
			break;

		}

		prev = queueList;
		queueList = queueList->next;

	}
	
		
	if(flag==0){
		printf("   Task %d ~ Delete %d ==> %d cannot be deleted. \n", numOfTask ,value , value );
	}
	

}

void search(int numOfTask,int value){   //checks whether there is a requested value in the list

	struct lst_node_s* queueList = (struct lst_node_s*)malloc(sizeof(struct lst_node_s*));

	int founded = -1;    

	queueList = headList;
	while (queueList != NULL ) { 
	    if (queueList->data == value) {
	        
	 		founded=value;
	 		printf("   Task %d ~ Search %d ==> %d found. \n", numOfTask ,value , value );
	       
	    }

	    queueList = queueList->next;
	    
	    
	}

	if (founded==-1)
	{
		printf("   Task %d ~ Search %d ==> %d is not found. \n", numOfTask ,value , value );
	}

}



void sortList(){       // to sort from small to large 

	if(headList == NULL){       // if queue is empty
		return;		
	}

	//if is not empty 
	struct lst_node_s* i = (struct lst_node_s*)malloc(sizeof(struct lst_node_s*));
	struct lst_node_s* j = (struct lst_node_s*)malloc(sizeof(struct lst_node_s*));

	int temp;

	for (i = headList ; i -> next != NULL ; i = i -> next)
	{
		for (j = i-> next ; j != NULL ; j=j->next)
		{
			if (i->data > j-> data)
			{
				temp = i->data;
				i->data = j ->data;
				j->data =temp;
			}
		}
	}

}


void Task_enqueue(int numTask,int type,int value) { //allows tasks to add to queue.
	
	struct tsk_node_s* queueTask = (struct tsk_node_s*)malloc(sizeof(struct tsk_node_s*));

	queueTask -> task_num = numTask ;      // to queue properties 
	queueTask -> task_type = type ;
	queueTask -> value = value ;
	queueTask -> next = NULL ;

	if(front == NULL && rear == NULL){   // for empty queue
		front = rear = queueTask;
		return;
	}
	rear->next = queueTask;
	rear = queueTask;
}


void Task_dequeue(){       //allows tasks to be removed from the queue.

	struct tsk_node_s* queueTask = (struct tsk_node_s*)malloc(sizeof(struct tsk_node_s*));
	
	queueTask = front; //to store head node

	if(front == NULL) {   // if head is null 
		printf("Queue is Empty\n");
		return; 
	}

	if(front == rear) {  // if head and tail are same nome 
		front = rear = NULL;
	}
	else {
		front = front->next;     
	}

	if(queueTask-> task_type == 0){   //to  call insert process if type = 0 

		insert(queueTask -> task_num , queueTask -> value);

	}
	else if(queueTask -> task_type == 1){  //to  call delete process if type = 1 

		delete(queueTask -> task_num ,queueTask -> value);

	}
	else if(queueTask -> task_type == 2){

		search(queueTask -> task_num ,queueTask -> value); //to  call search process if type = 2 

	}



	free(queueTask);    // to free memory 
}

void outOfTask(int numOfTasks){   //to pull all tasks from queue 

	int temp_numTask = 0 ;
	
	while(temp_numTask < numOfTasks){   

		Task_dequeue();      
		temp_numTask = temp_numTask + 1;
	}
}


void Task_queue(int numOfTasks){    // To Enqueue an integer

	int temp_numTask = 0 ;
	
	srand(time(NULL));     // provide to generate different numbers. 

	while(temp_numTask < numOfTasks){    //allows us to create as many task as we get from the console.

		int rand_type = rand() % 3;    // generate 0,1,2 for insert , delete and search process
		int rand_value = rand() % 5;   // generate task value for each Task

		
		Task_enqueue(temp_numTask,rand_type,rand_value);  //for task_queue 

		temp_numTask = temp_numTask + 1;

	}

}



void display_Lists(){ // to show list Nodes Value


	struct lst_node_s* queueList = (struct lst_node_s*)malloc(sizeof(struct lst_node_s*));
	queueList = NULL;     // we create a null node
	
	printf("\n ************* Final List ************* \n");

	if(headList == NULL){    // if not contain any node 
		printf("\n 		Empty List\n");
		
	}else{

		queueList=headList;          
		while(queueList != NULL){      //to control each element and write it's data
			printf(" %d ", queueList -> data );
			queueList = queueList -> next ; 
		}

	}
	printf("\n ************************************** \n");
}



int main(int argc, char* argv[]) {


	if(argc == 2) {

		int numOfTasks = atoi(argv[1]); // to get value from console
		
		printf("\n ************************************** \n\n");
		printf("\n   Generated %d random list tasks... \n", numOfTasks);
		printf("\n\n ************************************** \n\n");

		Task_queue(numOfTasks);  //creates the task according to the number received from the consol.
		outOfTask(numOfTasks);   //removes the Task from the Taskqueue and starts the necessary operations.
		sortList();              //to sort the List Queue
		display_Lists();		 //to show the  List Queue after all operations
			
	}
	else {
		printf("We only need 2 arguments\n");
	}


return 0;
}

























		
		
		
