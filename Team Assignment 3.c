/** 408415001 ¼ï«~»ô 408415069 ³¯«TÄË **/ 
#include<stdio.h> 
#include<stdlib.h> 
#include<time.h>

/*linked list*/
typedef struct linkedlist
{
	int id;
	int type;
	int time;
	struct linkedlist * link;
} lType;

/*queues*/
typedef struct queues
{
	int id;
	int time;
	struct queues * next;
} qType;

lType * newL(); //new linked list
qType * newQ(); //new queue
void data_generator(char *); //data generator
void read_file(char *, lType **, int *, int *, int *); //read file
void add_data(lType **, int , int, int); //add data into linked list and sort
void process_time(int *, int *, int* ); //the process time for items
void print_list(lType *); //print list
void process(lType *, qType **, qType **, qType **, int, int, int, int, int, int); //data processing
void enqueue(qType **, lType *); //enqueue data
int dequeue(qType **); //dequeue data

int main()
{
	int processor1, processor2, processor3; //the process time for items
	int item1 = 0, item2 = 0, item3 = 0; //the number of items were processed for each queue
	lType * L = newL(); //linked list
	qType * Q1 = newQ(), * Q2 = newQ(), * Q3 = newQ(); //3 type of queues process
	
	char filename[10]; //filename
	
	data_generator(filename); //data generator
	
	read_file(filename, &L, &item1, &item2, &item3); //read file
	
	process_time(&processor1, &processor2, &processor3); //the process time for items
	
	print_list(L); //print list
	
	process(L, &Q1, &Q2, &Q3, item1, item2, item3, processor1, processor2, processor3); //data processing
	
	return 0;
}

/*new linked list*/
lType * newL()
{
	lType * l;
	
	l = malloc(sizeof(lType));
	l = NULL;
	
	return l;
}

/*new queue*/
qType * newQ()
{
	qType * q;
	
	q = malloc(sizeof(qType));
	q = NULL;
	
	return q;
}

/*data generator*/
void data_generator(char * filename)
{
	int num_of_data, i;
	
	FILE * fp;
	
	printf("Please enter the number of data to generate : ");
	scanf("%d", &num_of_data);
	
	printf("Please enter the name of file to store : ");
	scanf("%s", filename);
	
	fp = fopen(filename, "w");
	
	srand(time(NULL));
	
	for (i = 0; i < num_of_data; i++)
	{
		fprintf(fp, "%d %d %d\n", rand() % 1000, rand() % 3 + 1, rand() % 20);
	}
	
	fclose(fp);
}

/*read file*/
void read_file(char * filename, lType ** L, int * item1, int * item2, int * item3)
{
	FILE * fp;
	int id, type, time;
	
	fp = fopen(filename, "r");
	
	while(fscanf(fp, "%d %d %d", &id, &type, &time) != EOF)
	{
		if(type == 1)
		{
			(*item1)++;
		}
		else if(type == 2)
		{
			(*item2)++;
		}
		else if(type == 3)
		{
			(*item3)++;
		}
		
		add_data(L, id, type, time); //add data into linked list
	}
	
	fclose(fp);
}

/*add data into linked list and sort*/
void add_data(lType ** L, int id, int type, int time)
{
	lType * N, * T, * S;
	
	N = malloc(sizeof(lType));
	N -> id = id;
	N -> type = type;
	N -> time = time;
	N -> link = NULL;
	
	if(*L == NULL)
	{
		*L = N;
	}
	else if((*L) -> link == NULL)
	{			
		if(N -> time > (*L) -> time)
		{
			(*L) -> link = N;
		}
		else
		{
			N -> link = *L;
			*L = N;
		}
	}
	else
	{	
		if(N -> time <= (*L) -> time)
		{
			N -> link = *L;
			*L = N;
		}
		else
		{
			T = *L;
			
			while(T -> link != NULL && N -> time > T -> link -> time)
			{
				T = T -> link;
			}
			
			if(T -> link == NULL)
			{
				T -> link = N;
			}
			else
			{
				N -> link = T -> link;
				T -> link = N;
			}
		}
	}	
}

/*the process time for items*/
void process_time(int * processor1, int * processor2, int * processor3)
{
	printf("Please enter the processing time of Type 1 : ");
	scanf("%d", processor1);
	
	printf("Please enter the processing time of Type 2 : ");
	scanf("%d", processor2);
	
	printf("Please enter the processing time of Type 3 : ");
	scanf("%d", processor3);
}

/*print list*/
void print_list(lType * L)
{
	printf("\n List : \n");
	printf(" Item | Type | Time \n");
	printf(" ------------------ \n");
	while(L != NULL)
	{
		printf("  %3d | %3d  | %3d \n", L -> id, L -> type, L -> time);
		L = L -> link;
	}
}

/*data processing*/
void process(lType * L, qType ** Q1, qType ** Q2, qType ** Q3, int item1, int item2, int item3, int p1, int p2, int p3)
{
	int counter = 0, end = 0; //time, end
	int num1 = 0, num2 = 0, num3 = 0; //the number of items waiting in a queue
	int max1 = 0, max2 = 0, max3 = 0; //Maximum number of items waiting in a queue
	int * spent1 = malloc(sizeof(int) * item1), * spent2 = malloc(sizeof(int) * item2), * spent3 = malloc(sizeof(int) * item3);
	int time1 = 0, time2 = 0, time3 = 0;
	int processor1 = 0, processor2 = 0, processor3 = 0; //processing item
	int waiting_time1 = 0, waiting_time2 = 0, waiting_time3 = 0; //waiting time
	int done1 = 0, done2 = 0, done3 = 0; //is it done ?
	int i = 0, j = 0, k = 0;
	
	while(1)
	{
		while(L != NULL && L -> time < counter)
		{
			L = L -> link;
		}
		
		printf("\nTime %d : \n", counter);
		
		while(L != NULL && L -> time == counter)
		{
			switch (L -> type)
			{
				case 1:
					printf("Enqueue Item %3d to Q1 \n", L -> id);
					num1++;
					if(num1 > max1)
					{
						max1 = num1;
					}
					spent1[i++] = counter;
					enqueue(Q1, L);
					break;
					
				case 2:
					printf("Enqueue Item %3d to Q2 \n", L -> id);
					num2++;
					if(num2 > max2)
					{
						max2 = num2;
					}
					spent2[j++] = counter;
					enqueue(Q2, L);
					break;
				
				case 3:
					printf("Enqueue Item %3d to Q3 \n", L -> id);
					num3++;
					if(num3 > max3)
					{
						max3 = num3;
					}
					spent3[k++] = counter;
					enqueue(Q3, L);
					break;
			}
			
			L = L -> link;
		}
				
		/*Process time of item of Type 1*/
		if(counter == waiting_time1 && done1 == 1)
		{
			printf("Item %3d processing completed \n", processor1);
			done1 = 0;
			processor1 = 0;	
		}
		else if(counter != waiting_time1 && done1 == 1)
		{
			printf("Item %3d still processing in Q1. (remaining %d) \n", processor1, waiting_time1 - counter);
		}
		
		if(processor1 == 0)
		{
			if(*Q1 != NULL)
			{
				printf("Dequeue Q1 \n");
				num1--;
				spent1[time1] = counter - spent1[time1];
				time1++;
				processor1 = dequeue(Q1);
				done1 = 1;
				waiting_time1 = counter + p1;
			}
		}
		
		/*Process time of item of Type 2*/
		if(counter == waiting_time2 && done2 == 1)
		{
			printf("Item %3d processing completed \n", processor2);
			done2 = 0;
			processor2 = 0;			
		}
		else if(counter != waiting_time2 && done2 == 1)
		{
			printf("Item %3d still processing in Q2. (remaining %d) \n", processor2, waiting_time2 - counter);
		}
		
		if(processor2 == 0)
		{
			if(*Q2 != NULL)
			{
				printf("Dequeue Q2 \n");
				num2--;
				spent2[time2] = counter - spent2[time2];
				time2++;
				processor2 = dequeue(Q2);
				done2 = 1;
				waiting_time2 = counter + p2;
			}
		}
		
		/*Process time of item of Type 3*/
		if(counter == waiting_time3 && done3 == 1)
		{
			printf("Item %3d processing completed \n", processor3);
			done3 = 0;
			processor3 = 0;
		}
		else if(counter != waiting_time3 && done3 == 1)
		{
			printf("Item %3d still processing in Q3. (remaining %d) \n", processor3, waiting_time3 - counter);
		}
		
		if(processor3 == 0)
		{
			if(*Q3 != NULL)
			{
				printf("Dequeue Q3 \n");
				num3--;
				spent3[time3] = counter - spent3[time3];
				time3++;
				processor3 = dequeue(Q3);
				done3 = 1;
				waiting_time3 = counter + p3;
			}
		}
		
		if(L == NULL && *Q1 == NULL && *Q2 == NULL && *Q3 == NULL && done1 == 0 && done2 == 0 && done3 == 0)
		{
			break;
		}
		
		counter ++;
	}
	
	/*How many items were processed for each queue*/
	printf("\nQueue 1 processed %d items \n", item1);
	printf("Queue 2 processed %d items \n", item2);
	printf("Queue 3 processed %d items \n", item3);
	
	/*Maximum number of items waiting in a queue at one time*/
	printf("\nMaximum number of items waiting in Queue 1 at one time : %d \n", max1);
	printf("Maximum number of items waiting in Queue 2 at one time : %d \n", max2);
	printf("Maximum number of items waiting in Queue 3 at one time : %d \n", max3);
	
	/*Average amount of time spent in a queue for each type of items*/
	for(i = 0, time1 = 0; i < item1; i++)
	{
		time1 += spent1[i];
	}
	
	for(j = 0, time2 = 0; j < item2; j++)
	{
		time2 += spent2[j];
	}
	
	for(k = 0, time3 = 0; k < item3; k++)
	{
		time3 += spent3[k];
	}
	
	printf("\nAverage amount of time spent in a queue for each type of items : %d \n", time1 / item1);
	printf("Average amount of time spent in a queue for each type of items : %d \n", time2 / item2);
	printf("Average amount of time spent in a queue for each type of items : %d \n", time3 / item3);
}

/*enqueue data*/
void enqueue(qType ** Q, lType * L)
{
	qType * N, * T;
	
	N = malloc(sizeof(qType));
	N -> id = L -> id;
	N -> time = L -> time;
	N -> next = NULL;
	
	if(*Q == NULL)
	{
		*Q = N;
	}
	else
	{
		T = *Q;
		
		while(T -> next != NULL)
		{
			T = T -> next;
		}
		
		T -> next = N;
	}
}

/*dequeue data*/
int dequeue(qType ** Q)
{
	qType * N;
	int i;
	
	N = *Q;
	
	i = N -> id;
	
	*Q = (*Q) -> next;
	
	free(N);
	
	N = NULL;
	
	return i;
}
