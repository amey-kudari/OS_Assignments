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
#define ld long double
#define drep(l,r) for(ll i=l;i<=r;i++)
#define pll(a) printf("%lld ",a);
#define swap(a,b) ll tasd=a;a=b;b=tasd

ll *shareMem(size_t size){
    key_t mem_key = IPC_PRIVATE;
    ll shm_id = shmget(mem_key, size, IPC_CREAT | 0666);
    return (ll*)shmat(shm_id, NULL, 0);
}


ll fixl(ll arr[],ll l,ll r){
    ll piv=arr[r],ind=l-1;
    drep(l,r-1) if(arr[i]<=piv){
        ind++; swap(arr[ind],arr[i]);
    } swap(arr[ind+1],arr[r]); return ind+1;
}

void qs(ll arr[],ll l,ll r){     // NORMAL QS
    if(r<=l) return;
    ll p=fixl(arr,l,r);
    qs(arr,l,p-1); qs(arr,p+1,r);
}

void qs_mp(ll arr[],ll l,ll r){  // MULTI-PROCESS
    if(r<=l) _exit(1);
    ll p=fixl(arr,l,r);
    int pid=fork();
    int pid2;
    if(pid==0){
        qs(arr,l,p-1); _exit(1);
    } else{
        pid2=fork();
        if(pid2==0){
            qs(arr,p+1,r); _exit(1);
        } else{
            int status;
            waitpid(pid,&status,0);
            waitpid(pid2,&status,0);
        }
    }
}

struct arg{
    ll l,r; ll *arr;
};

void *qs_mt(void *fargs){        // MULTI-THREADED
    struct arg *args=(struct arg*)fargs;
    ll l=args->l; ll r=args->r; ll *arr=args->arr;
    if(l>=r) return NULL;
    ll p=fixl(arr,l,r);
    struct arg a1={l,p-1,arr};
    pthread_t tid1;
    pthread_create(&tid1,NULL,qs_mt,&a1);
    struct arg a2={p+1,r,arr};
    pthread_t tid2;
    pthread_create(&tid2,NULL,qs_mt,&a2);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
}

int main(){
    ld ns,ne,mps,mpe,mts,mte;    // normal ,mul-pro, mul-th;
    struct timespec ts;
    ll n; scanf("%lld",&n);
    ll *arr = shareMem(sizeof(ll)*(n+1));
    ll *brr = shareMem(sizeof(ll)*(n+1));
    ll *crr = shareMem(sizeof(ll)*(n+1));
    drep(0,n-1) scanf("%lld",&arr[i]);
    drep(0,n-1) crr[i]=brr[i]=arr[i];
    
    
    printf("NORMAL QUICK SORT\n");
    clock_gettime(CLOCK_MONOTONIC_RAW,&ts);
    ns=ts.tv_nsec/(1e9)+ts.tv_sec;
    qs(arr,0,n-1);
    ne=ts.tv_nsec/(1e9)+ts.tv_sec;
    printf("TIME TAKEN: %Lf\n",ne-ns);
    //drep(0,n-1) pll(arr[i]) printf("\n"); 

    printf("MULTI-PROCESS QUICK SORT\n");
    clock_gettime(CLOCK_MONOTONIC_RAW,&ts);
    ns=ts.tv_nsec/(1e9)+ts.tv_sec;
    qs_mp(brr,0,n-1);
    ne=ts.tv_nsec/(1e9)+ts.tv_sec;
    printf("TIME TAKEN: %Lf\n",ne-ns);
    //drep(0,n-1) pll(brr[i]) printf("\n"); 

    printf("MULTI-THREADED QUICK SORT\n");
    clock_gettime(CLOCK_MONOTONIC_RAW,&ts);
    ns=ts.tv_nsec/(1e9)+ts.tv_sec;
    struct arg ar={0,n-1,crr};
    pthread_t tid;
    pthread_create(&tid,NULL,qs_mt,&ar);
    pthread_join(tid,NULL);
    ne=ts.tv_nsec/(1e9)+ts.tv_sec;
    printf("TIME TAKEN: %Lf\n",ne-ns);
    //drep(0,n-1) pll(crr[i]) printf("\n");
    
}





