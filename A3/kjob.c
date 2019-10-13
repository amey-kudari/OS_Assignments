#include "header.h"

void kjob(char *arr){
    ll n=strlen(arr); ll sn=-1;
    for(ll i=0;i<n;i++){
        if(arr[i]==' '){
            arr[i]='\0';
            if(sn!=-1){
                printf("invalid arguments to kill\n"); return;
            }
            sn=i+1;
        }
    }
    if(sn==-1){
        printf("invalid arguments to kill\n"); return;
    }
    ll jn=toint(arr);
    ll sen=toint(arr+sn);
    if(kill(jn,sen)==-1){
        printf("Error passing signal\n");
    }
}