#define _POSIX_C_SOURCE 199309L //required for clock
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <limits.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <inttypes.h>
#include <math.h>
#define ll long long int
#define true 1
#define bool int
#define false 0
ll P=3;

ll randint(ll a,ll b){
    ll up=rand()%(b-a+1);
    return a+up;
}


pthread_mutex_t mutex_fill; // only 1 can table can fill at once 
pthread_mutex_t mutex_wft; // only 1 can slot assigned at once 

// ROBOT
ll robot_a[1000],m;  // food left from each robot, table_a: avail food, table avail slots;

typedef struct robot_inp{
	ll id,w,r;
} robot_inp;

void biryani_ready(ll id){
	while(robot_a[id]) sleep(0.1);
	printf("OVER!! %lld %lld\n",id,robot_a[id]); 
	return;
}

void *robot(void *inp){
	robot_inp *ginp=(robot_inp *) inp;
	ll w=ginp->w; ll id=ginp->id; ll r=ginp->r;
	for(;;){
		printf("ROBOT %lld MAKING FOOD\n",id);
		sleep(w); robot_a[id]=r;
		printf("ROBOT %lld MADE %lld VESSELS IN %lld TIME\n",id,robot_a[id],w);
		biryani_ready(id);
	}
}
// END-ROBOT

// SERVING TABLE
ll table_slots[1000],table_a[1000],n;
typedef struct table_inp{
	ll id,mxs;  // id, max slots
}table_inp;

void fill(ll id){
    printf("WAITING TO FILL TABLE %lld\n",id);
    pthread_mutex_lock(&mutex_fill);
    for(ll i=0;;i++){ 
    	if(i>=m){
    		sleep(0.1); i=i%m;
    	}
    	if(robot_a[i]!=0){
    		robot_a[i]--; table_a[id]=P;
    		printf("ROBOT CHEF %lld FILLED TABLE %lld\n",i,id);
    		break;
    	}
    }
    pthread_mutex_unlock(&mutex_fill);
}

void ready_to_serve_table(ll id){
	printf("TABLE %lld READY TO SERVE\n",id);
	while(table_slots[id]>0&&table_a[id]>0) sleep(0.1);
	printf("TABLE %lld NOT READY TO SERVE %lld %lld\n",id,table_slots[id],table_a[id]);
}

void *table(void *inp){
	table_inp *ginp=(table_inp *) inp;
	ll id=ginp->id; ll mxs=ginp->mxs;
	table_slots[id]=mxs;
	for(;;){
		if(table_a[id]>0&&table_slots[id]>0) ready_to_serve_table(id);
		else if(table_slots[id]==mxs&&table_a[id]==0) fill(id);
		sleep(0.1);
	}
}
// END-SERVING TABLE

// STUDENT
ll wait_for_slot(ll sid){
	printf("Student %lld is waiting to be allocated a slot on the serving table\n",sid);
	pthread_mutex_lock(&mutex_wft);
	for(ll i=0;;i++){
		if(i>=n){
			sleep(0.1); i=i%n;
		}
		if(table_slots[i]>0&&table_a[i]>0){
			table_slots[i]--; table_a[i]--;
			printf("Student %lld assigned a slot on the serving table %lld and waiting to be served\n",sid,i);
			pthread_mutex_unlock(&mutex_wft);
			return i;
		}
	}
}
void student_in_slot(ll slot,ll sid){
	printf("Student %lld on serving table %lld has been served.\n",sid,slot);
	table_slots[slot]++;
}
typedef struct student_inp{
	ll sid,tte; // time to go to table and eat;
}student_inp;
void *student(void *inp){
	student_inp* ginp=(student_inp *) inp;
	ll tte=ginp->tte; ll sid=ginp->sid;
	printf("Student %lld has arrived\n",sid);
	ll slot=wait_for_slot(sid);
	sleep(tte);
	student_in_slot(slot,sid);
}
// END-STUDENT

int main(){
    pthread_mutex_init(&mutex_fill, NULL);
    pthread_mutex_init(&mutex_wft, NULL);
    scanf("%lld",&m);
    scanf("%lld",&n);
    ll k; scanf("%lld",&k);

    pthread_t robots[m+1];
    for(ll i=0;i<m;i++){
		robot_inp* tinp=(robot_inp *)malloc(sizeof(robot_inp));
		tinp->id=i; tinp->w=randint(2,5); tinp->r=randint(1,10);
    	pthread_create(&robots[i], NULL, robot, (void*)(tinp));
    }

    pthread_t tables[n+1];
    for(ll i=0;i<n;i++){
    	table_inp* ttinp=(table_inp *)malloc(sizeof(table_inp));
    	ttinp->id=i; ttinp->mxs=randint(1,10);
	    pthread_create(&tables[i],NULL,table,(void*)(ttinp));	
    }


    pthread_t students[k+1];
    for(ll i=0;i<k;i++){
    	ll tte; scanf("%lld",&tte);
    	student_inp * sinp=(student_inp *)malloc(sizeof(student_inp));
    	sinp->tte=tte; sinp->sid=i;
    	pthread_create(&students[i],NULL,student,(void *)(sinp));
    }
    // pthread_join(tid2, NULL);
    pthread_join(students[k-1], NULL);
    pthread_mutex_destroy(&mutex_fill);
    pthread_mutex_destroy(&mutex_wft);
    printf("Simulation Over.\n");
}