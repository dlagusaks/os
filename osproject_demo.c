#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include<stdlib.h>
#include"calc_times.h"

unsigned int secretBox = 100000;
#define customerNumber 1000


int safeBox[customerNumber] = { 0, };
int loan[customerNumber] = { 0, };//전역변수
int jangboo[customerNumber] = { 0, };
sem_t bin_sem;

//Variables for clock and time
struct tms tmsstart, tmsend;
clock_t start, end;

struct range{
	int start;
	int end;
	int flag;
};

typedef struct information1{
   int customerNum;
   int box;
   int put;
   int amount;
}information;


void* runner(void* arg);

int main() {
	
   

   pthread_t threads[customerNumber];
   void* result_t = NULL;
   int state = sem_init(&bin_sem,0,1); 
   if(state != 0)  printf("sem_init Error\n");
   
 
   
  /* information customer1;
   customer1.customerNum = 0;
   customer1.box = 0;
   customer1.put = 0;
   customer1.amount = 1000;
   information customer2;
   customer2.customerNum = 1;
   customer2.box = 0;
   customer2.put = 1;
   customer2.amount = 2000;
   information customer3;
   customer3.customerNum = 2;
   customer3.box = 1;
   customer3.put = 0;
   customer3.amount = 3000;
   information customer4;
   customer4.customerNum = 3;
   customer4.box = 1;
   customer4.put = 1;
   customer4.amount = 5000;*/
   
   information info[1000];
   int i;

   srand(time(NULL));

   for (i = 0; i < 1000; i++) {
      info[i].customerNum = i % 100; // 0부터 99까지 순차적으로 증가
      info[i].box = rand() % 2; // 0 또는 1로 랜덤 설정
      info[i].put = rand() % 2; // 0 또는 1로 랜덤 설정
      info[i].amount = (rand() % 40 + 10) * 1000; // 10000부터 50000까지 1000 단위로 증가
   }
   	//Start time of calculation
   start = times(&tmsstart);
   for (i = 0; i < 1000; i++) {
   	pthread_create(&threads[i], NULL, runner, (void*)&info[i]);
   }

   // 스레드 조인
   for (i = 0; i < 1000; i++) 
   	pthread_join(threads[i], NULL);
   
   pid_t pid;
   //   int status;

   pid = fork();
 
   int id = 0;
   if (pid < 0) {
       fprintf(stderr, "Fork failed");
       return -1;
   }
   else if (pid == 0) {
 
       int id = 0;
       for (int i = 0; i < customerNumber; i++) {
           if (safeBox[i] < 0) {
               loan[i] = -safeBox[i];
               safeBox[i] = 0;
           }
       }
       sleep(10);
       exit(3);
   }
   else {

       printf("put your id: ");
       scanf("%d", &id);
       if (safeBox[id - 1] >= 0)
           printf("your safeBox is: %d \n", safeBox[id - 1]);
       else
           printf("you have loan: %d \n", loan[id - 1]);
       printf("your jangboo is: %d \n", jangboo[id - 1]);
   }

	
	//End time of Calculation
   end = times(&tmsend);
   pr_times(end-start, &tmsstart, &tmsend);
	
  	
   return 0;
}
   
   




   /*pthread_create(&t1, NULL, runner, (void *)&customer1); //쓰레드 1 생성
   pthread_create(&t2, NULL, runner, (void*)&customer2); //쓰레드 2 생성
   pthread_create(&t3, NULL, runner, (void*)&customer3); //스레드 3 생성
   pthread_create(&t4, NULL, runner, (void*)&customer4); //스레드 4 생성
   pthread_join(t1, &result_t); //쓰레드가 종료되기를 기다린 후 메인 프로세스가 종료되도록
   pthread_join(t2, &result_t); //pthread_join 을 호출
   pthread_join(t3, &result_t);
   pthread_join(t4, &result_t);
   printf("alldone");
   return 0; */


void* runner(void *arg) {
   information *customer = (information *)arg;
   if (customer->box == 0) {	
      printf("<INFO>\n customerID : %d\n box access : %d\n put : %d\n amount : %d\n-------------------------------------------\n",customer->customerNum,customer->box,customer->put,customer->amount);
      /*
      printf("<customer INFO>\n");
      printf("customerID : %d\n",customer->customerNum);
      printf("box access : %d\n",customer->box);
      printf("put : %d\n",customer->put);
      printf("amount : %d\n",customer->amount);
      */
      
      if (customer->put == 0) {
         safeBox[customer->customerNum] += customer->amount;
         //printf("safeBox[%d] : + %d\n ",customer->customerNum,customer->amount);
      }
      else if (customer->put == 1) {
         if (customer->amount > safeBox[customer->customerNum]) {
            //printf("can't not withdraw(safe)\n");
           // printf("-------------------------------------------\n");
         }
         else {
            safeBox[customer->customerNum] -= customer->amount;
            //printf("safeBox[%d] : - %d\n",customer->customerNum,customer->amount);
            //printf("-------------------------------------------\n");
         }
      }
      else {
         printf("incorrect put value");
         //printf("-------------------------------------------\n");
      }
   }
   else{
      sem_wait(&bin_sem);
      printf("<SECRET BOX INFO>\n");
      printf(" customerID : %d\n",customer->customerNum);
      printf(" box access : %d\n",customer->box);
      printf(" put : %d\n",customer->put);
      printf(" amount : %d\n",customer->amount);
      if (customer->put == 0) {
         secretBox += customer->amount;
         jangboo[customer->customerNum] += customer->amount;
         printf(" jangboo[%d] + %d\n",customer->customerNum,customer->amount);
         printf(" CURRENT SECRET BOX : %d\n",secretBox);
         printf("-------------------------------------------\n");
      }
      else if (customer->put == 1) {
         if (customer->amount > secretBox) {
            printf(" can't not withdraw(secret)\n");
            printf("-------------------------------------------\n");
         }
         else {
            secretBox -= customer->amount;
            jangboo[customer->customerNum] += customer->amount;
            printf(" jangboo[%d] - %d\n",customer->customerNum,customer->amount);
            printf(" CURRENT SECRET BOX : %d\n",secretBox);
            
             printf("-------------------------------------------\n");
         }
      }
      else {
         printf("incorrect put value");
         printf("-------------------------------------------\n");        
      }
      sem_post(&bin_sem);
   }

}
