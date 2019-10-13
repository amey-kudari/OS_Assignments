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
#include <semaphore.h> 

#define ll long long int

ll getll(){ ll a; scanf("%lld",&a); return a; }

sem_t qmut; // only one driver can see queue
sem_t pmut; // k payment servers

ll RIDETIME[10000],PRIDETIME[10000];
ll n_index,pn_index;

ll queue[1000],qf,qb;
void add(ll a){ queue[qb++]=a; }
ll front(){ return queue[qf]; }
ll empty(){ return qb-qf; }
void pop(){ qf++; }
ll fnrem(ll a){
    for(ll i=qf;i<qb;i++){
        if(queue[i]==a){
            for(ll j=i;j<qb;j++){
                queue[j]=queue[j+1];
            } qb--; return 1;
        }
    } return 0;
}


ll pqueue[1000],pqf,pqb;
void padd(ll a){ pqueue[pqb++]=a; }
ll pfront(){ return pqueue[pqf]; }
ll pempty(){ return pqb-pqf; }
void ppop(){ pqf++; }
ll pfnrem(ll a){
    for(ll i=pqf;i<pqb;i++){
        if(pqueue[i]==a){
            for(ll j=i;j<pqb;j++){
                pqueue[j]=pqueue[j+1];
            } pqb--; return 1;
        }
    } return 0;
}


void *MakePayment(void *args){
	sem_wait(&pmut);
	sleep(2);
	printf("PAID!!!\n");
	sem_post(&pmut);
}

typedef struct rider_arg{
	ll wt,rt,type;
}rider_arg;


void *rider(void *arg){
	printf("Rider created\n");
	rider_arg * garg=(rider_arg *)arg;
	ll type=garg->type; ll wt=garg->wt; ll rt=garg->rt;
	if(type==1){
		ll pri=pn_index; pn_index++; PRIDETIME[pri]=rt;
		padd(pri);
		sleep(wt);
	    sem_wait(&qmut);
		if(pfnrem(pri)==1){
			printf("RIDER: %lld TIMED OUT\n",pri);
		}
		sem_post(&qmut);
	} else {
		ll ri=n_index; n_index++; RIDETIME[ri]=rt;
		// printf("ADDED %lld %lld %lld\n",pri,wt,rt);
		add(ri);
		sleep(wt);
	    sem_wait(&qmut);
		if(fnrem(ri)==1){
			printf("RIDER: %lld TIMED OUT\n",ri);
		}
		sem_post(&qmut);
	}
}

void *driver(void *arg){
	printf("Driver Created !\n");
	ll nr=0; // number of riders
	ll r1=-1,r2=-1; // wait time of wach rider
	ll same=0;
	for(;;){
		if(nr==0){
		    sem_wait(&qmut);
		    // printf("%lld %lld\n",empty(),pempty());
			if(pempty()!=0){
				ll ri=pfront(); ppop();
				printf("RIDER [PREMIUM] %lld ASSIGNED CAB\n",ri);
				r1=r2=(PRIDETIME[ri]-1); nr=2; same=1;
			} else if(empty()!=0){
				ll ri=front(); pop();
				printf("RIDER [NON_PREMIUM] %lld ASSIGNED CAB\n",ri);
    			r1=RIDETIME[ri]-1; nr=1; same=0;
			}
			sem_post(&qmut);
			sleep(1);
		} else if(nr==1){
			if(r1==0){
				nr=0; printf("CAB COMPLETED RIDE\n"); r1--;
				pthread_t tp;
				sem_wait(&pmut);
				if(pthread_create(&tp,NULL,MakePayment,NULL)!=0) printf("ERROR IN PAYMENT\n");
				sem_post(&pmut);
				continue;
			}
		    sem_wait(&qmut);
		    if(empty()!=0){
		    	ll ri=front(); pop(); nr=2;
				printf("RIDER [NON_PREMIUM] %lld ASSIGNED CAB\n",ri);  same=0;
				r2=RIDETIME[ri]-1;
		    }
			sem_post(&qmut);
			printf("%lld %lld => \n",r1,r2);
			r1--;
			printf("%lld %lld\n\n",r1,r2);
			sleep(1);
		} else if(nr==2){
			if(r1<r2){
				ll tmp=r1; r1=r2; r2=tmp; // r2 allways finished first
			}
			if(r2==0){
				pthread_t tp;
				sem_wait(&pmut);
				if(pthread_create(&tp,NULL,MakePayment,NULL)!=0) printf("ERROR IN PAYMENT\n");
				sem_post(&pmut);
				printf("CAB COMPLETED RIDE\n"); r2--; nr=1;
				if(same==1){
					nr=0; r1=-1; same=0;
				}
				printf("%lld \n",nr);
				continue;
			}
			// printf("%lld %lld => \n",r1,r2);
			r1--; r2--;
			// printf("%lld %lld\n\n",r1,r2);
			sleep(1);
		}
	}
}

int main(){
	sem_init(&qmut, 0, 1);
	ll k; k=getll();	
	sem_init(&pmut, 0, k); 
	ll nd=getll(); pthread_t drivers[nd+1];
	while(nd--){
		if(pthread_create(&drivers[nd],NULL,driver,NULL)!=0) printf("ERROR CREATING DRIVER %lld\n",nd);
	}
	ll nr=getll(); pthread_t riders[nr+1];
	for(ll i=0;i<nr;i++){
		ll wt=getll(); ll rt=getll(); ll type=getll();
		rider_arg * arg=(rider_arg *)malloc(sizeof(rider_arg));
		arg->wt=wt; arg->rt=rt; arg->type=type;
		// printf("CREATING RIDER %lld %lld %lld\n",arg->wt,arg->rt,arg->type);
		ll a=pthread_create(&riders[i],NULL,rider,(void *)arg);
		if(a!=0) printf("ERROR CREATING RIDER %lld\n",i);
		// printf("%lld \n",a);
	}
	pthread_join(riders[0],NULL);
}