#include "header.h"

void c_ecex(char *command){
    char tcommand[1000]; strcpy(tcommand,command); ll n=strlen(command);
    ll ind=0;
    char *args[]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
	NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
    char *tcmd=strtok(tcommand," ");
    while(tcmd!=NULL){
        if(strlen(tcmd)>0) args[ind++] = tcmd; tcmd=strtok(NULL," ");
    }
    
    ll isbg=0;
    if(args[ind-1][0]=='&'&&strlen(args[ind-1])==1){
        isbg=1;
    }
    int pid=fork();
    if(pid<0){
        printf("FORK ERROR\n");
    } else if(pid){
        int status;
        printf("%d\n",pid);
        if(isbg==0){
            C_FG_P=pid;
            waitpid(pid, &status, WUNTRACED);
        } else {
            children[no_children++]=pid;
        }
    } else{
        if(execvp(args[0],args)==-1){
            printf("Error executing %s\n",command);
        }
    }
}