#include "header.h"


void msetenv(char *command){
    char tcommand[1000]; strcpy(tcommand,command);
    sfix(tcommand);
    printf("%s\n",tcommand);
    ll n=strlen(tcommand);
    if(n==0||noccur(tcommand,' ')!=1){
        printf("Invalid arguments\n");
    } else {
        char *name=tcommand; char *value;
        for(ll i=0;i<n;i++){
            if(tcommand[i]==' '){
                value=tcommand+i+1; tcommand[i]='\0';
            }
        }
        if(setenv(name,value,1)!=0){
            printf("Setenv failed\n");
        }
    }
}

void munsetenv(char *command){
    char tcommand[1000]; strcpy(tcommand,command);
    sfix(tcommand);
    if(strlen(tcommand)!=0&&noccur(tcommand,' ')==0){
        if(unsetenv(tcommand)!=0){
            printf("unsetenv failed\n");
        }
    } else {
        printf("Invalid arguments\n");
    }
}