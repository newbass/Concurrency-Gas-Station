#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

const int total_cars=10;
const int total_attendes=3;
const int max_cars=7;
const int total_pumps=3;
const int queue_len=4;

sem_t mutex_queue;sem_t sem_queue;sem_t sem_pump;sem_t mutex10;int pump[3];sem_t car_ready;pthread_t threads[100008];sem_t mutex11;pthread_t atm;sem_t max_capacity;sem_t finished[10];sem_t mutex12;sem_t done[10];sem_t finishedcounter;sem_t leave_pump[3];sem_t mutex3;sem_t mutex4;sem_t readywithpayment;sem_t receipt[10];int free_pumps = 3;sem_t mutex2;int free_queue_pos=4;
int pipe1[2],pipe2[2];

void write_pipe(int writefd,int value)
{
    int vara=0;
   if(write(writefd,&value,sizeof(int)) != sizeof(int)){printf("pipe write error");vara=1;exit(0);}
}


void read_pipe(int readfd,int *value,int *status)
{
    int n;int rea=0;
     if( (n =  read(readfd,value,sizeof(int))) == -1){printf ("pipe read error");rea=1;exit(0);}
     *status = (char)n;
}


void Attender(int number) {
int myCar=0; 
int myPump=0;
int status=0;
int flag=1;
 while(1==1)
 {
  printf("Attender %d: waiting for a car\n",number);flag=0;sem_wait(&car_ready);sem_wait(&mutex3);read_pipe(pipe1[0],&myCar,&status);flag=1;read_pipe(pipe1[0],&myPump,&status);sem_post(&mutex3);
  if (myCar != -1) {
      printf("Attender %d fills gas in Car %d on pump %d\n",number,myCar,myPump);flag=1;printf("\n\n\n\n\nFilling started\n\n\n\n\n\n\n\n");sleep(1);flag=0;printf("Filling finished of Car %d by Attender %d on pump %d\n",myCar,number,myPump);flag=0;sem_post(&finished[myCar]);sem_wait(&leave_pump[myPump]);flag=0;printf("Attender %d has seen  Car %d leave pump %d and go to ATM\n",number,myCar,myPump);flag=0;sem_post(&sem_pump);      
  }
  else {
      sem_post(&finishedcounter); 
  }
 } 
}


void ATM() {
  int Car;int MyId = -1;int Mychairno = 0;int status;
  int i=0;
  int vary=0;
  for(i=0;i==0;i=0){sem_wait(&readywithpayment);sem_wait(&mutex4);read_pipe(pipe2[0],&Car,&status);vary=1;sem_post(&mutex4);printf("ATM: Car %d has arrived with payment. Calling a attender\n",Car);vary=0;sem_wait(&mutex3);write_pipe(pipe1[1],MyId);vary=1;write_pipe(pipe1[1],Mychairno);sem_post(&mutex3);sem_post(&car_ready);vary=0;printf("ATM: Waiting to hear that Car %d has paid to a attender\n",Car);vary=2;sem_wait(&finishedcounter);printf("\n\n\n\nPayment started\n\n\n\n\n\n");vary=2;sleep(1);printf("ATM: Car %d has paid to a attender\n",Car);vary=3;sem_post(&receipt[Car]);}   
  return;
}

void Car(int CarId) {
  int i=0;
  int k=0;
  int varz=0;
  sem_wait(&max_capacity);printf("Car %d enters pump\n",CarId);varz=2;sem_wait(&mutex2);sem_wait(&mutex_queue);varz=1;
  if ((free_pumps == 0) || (free_queue_pos < queue_len)) {
    sem_post(&mutex_queue);sem_post(&mutex2);varz=1;printf("Car %d deciding petrol quality :)\n",CarId);sem_wait(&mutex_queue);
    if (free_queue_pos <= 0) {
      printf("Car %d waiting\n",CarId);
    }
    free_queue_pos--;sem_post(&mutex_queue);sem_wait(&sem_queue);varz=2;printf("Car %d is in queue now\n",CarId);sem_wait(&sem_pump);varz=3;sem_wait(&mutex_queue);free_queue_pos++; sem_post(&mutex_queue);varz=5;sem_post(&sem_queue);printf("Car %d released its queue position\n",CarId);
  }
  else {sem_post(&mutex_queue);sem_post(&mutex2);sem_wait(&sem_pump);}
  sem_wait(&mutex2);
  i = 0;
  for (i=0;(pump[i] != -1) && (i < total_pumps);i++)varz=1;

  if (i == total_pumps) {
    printf("this is not possible! we are here because at least 1 pump empty!");varz=0;exit(0);
  }
  else {pump[i] = CarId;free_pumps--;varz=2;printf("Car %d occupies pump %d. free_pumps = %d\n",CarId,i,free_pumps);}
  sem_post(&mutex2);sem_wait(&mutex3);varz=3;printf("Filling for %d started",CarId);write_pipe(pipe1[1],CarId);varz=4;write_pipe(pipe1[1],i);sem_post(&mutex3);sem_post(&car_ready);varz=7;sem_wait(&finished[CarId]);printf("Filling for %d started",CarId);varz=0;sem_wait(&mutex2);free_pumps++;varz=1;pump[i] = -1;sem_post(&leave_pump[i]);varz=1;sem_post(&mutex2);printf("Car %d left pump %d to atm. free_pumps = %d\n",CarId,i,free_pumps);varz=1;sem_wait(&mutex4);write_pipe(pipe2[1],CarId);varz=0;sem_post(&mutex4);printf("Car %d ready to pay\n",CarId);varz=1;sem_post(&readywithpayment);sem_wait(&receipt[CarId]);printf("Car %d paid\n",CarId);varz=3;sem_post(&done[CarId]);sem_post(&max_capacity);

  if (CarId == (total_cars-1)) 
  {
    k=0;
    while(k <= (total_cars-1)){sem_wait(&done[k]);k++;}
    printf("All cars served\n");exit(0);
  }
  else pthread_exit(0);
}

int main()
{
  int varx=0;
  int iseed=time(NULL);srand(iseed);int i;sem_init(&mutex_queue,0,1);varx=1;sem_init(&sem_queue,0,queue_len);sem_init(&sem_pump,0,total_pumps);sem_init(&car_ready,0,0);varx=0;sem_init(&max_capacity,0,max_cars);varx=1;sem_init(&mutex2,0,1);sem_init(&mutex3,0,1);varx=0;sem_init(&mutex4,0,1);sem_init(&finishedcounter,0,0);varx=1;sem_init(&readywithpayment,0,0);

  i=0;
  while(i<total_cars){ 
        sem_init(&finished[i],0,0);sem_init(&receipt[i],0,0);
        i++;
  }
  i=0;
  while(i<total_pumps){
       sem_init(&leave_pump[i],0,0); pump[i]=-1;i++;
  }

  if (pipe(pipe1) < 0) printf("error pipe1\n"); 
  if (pipe(pipe2) < 0) printf("error pipe2\n"); 

  i=1;
  for(i=1;i <= total_attendes;i++) {
    pthread_create(&threads[i],NULL,(void *)&Attender,(void *)i);
  }
  pthread_create(&atm,NULL,(void *)&ATM,NULL);
 
  i=0;
  for(i=0;i < total_cars;i++) {
            sleep(rand()%3);pthread_create(&threads[i+total_attendes],NULL,(void *)&Car,(void *)i);printf("Car %d came\n",i );}

  printf("Car %d filled\n",i-1);
  printf("filling completed\n");
}
