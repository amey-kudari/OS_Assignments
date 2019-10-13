#include "header.h"

char HOME[1000],CDIR[1000],UNAME[1000],DESKTOP[1000];
pid_t cprocess;


void sigintHandler(int signum){
    int ppid=getpid();
    if(ppid!=SHELL_PID){
        kill(ppid,9);
    }
}

void sighandler(int sig_num) {
    printf("pushed pid %d to background\n",C_FG_P);
    children[no_children++]=C_FG_P;
    C_FG_P=0;
}

void correct_path(char *arr){
	int an=strlen(arr); int bn=strlen(HOME);
	if(an<bn) return;
	char newarr[1000];
	for(int i=0;i<bn;i++) newarr[i]=arr[i]; newarr[bn]='\0';
	if(strcmp(newarr,HOME)!=0) return;
	// printf("ITS FROM HOME\n");
	if(strlen(HOME)==1){
		for(int i=an;i>=0;i--) arr[i+1]=arr[i];
		arr[0]='~'; return;
	}
	arr[0]='~'; for(int i=bn;i<=an;i++) arr[i-bn+1]=arr[i];
}

void init(){
	char cdir[1000],cname[1000];
	getcwd(cdir,sizeof(cdir));
	correct_path(cdir);
	gethostname(cname,sizeof(cname));
    char* usname = getenv("USER");
    printf("<");
    printf(ANSI_COLOR_MAGENTA "%s@%s" ANSI_COLOR_RESET,usname,cname,cdir); 
    printf(":");
    printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET,cdir); 
    printf(">");
}

void child_remove(ll ch){
    ll ind=-1;
    for(ll i=0;i<no_children;i++){
        if(children[i]==ch) ind=i;
    }
    if(ind==-1) return;
    for(ll i=ind;i<no_children;i++) children[i]=children[i+1]; no_children--;
}

void exeloc(int gpid){
	char spid[1000]; char pfile[1000]="/proc/"; char efile[]="/exe\0";
	tostr(spid,gpid); concat(pfile,spid,'\0'); concat(pfile,efile,'\0');
	// printf("%s\n",pfile);
	char buf[1000]; for(int i=0;i<1000;i++) buf[i]='\0';
	int chk = readlink(pfile,buf,1000);
	if(chk!=0) printf("%s\n",buf);
}

void mypinfo(int gpid){
	char spid[1000]; char pfile[1000]="/proc/"; char efile[]="/status";
	tostr(spid,gpid); concat(pfile,spid,'\0'); concat(pfile,efile,'\0');
	printf("%s\n",pfile);
	FILE *pstatus = fopen(pfile,"r");
	if(pstatus==NULL){
		printf("Process does not exist\n");
		return;
	}
	char arr[1000];
	char rstatus[1000]; int itr=1;
	char vmsize[1000];
	printf("pid --- %s\n",spid);
	while(fgets(arr,1000,pstatus)){
		if(itr==3){
			printf("Process %s",arr);
		} 
		else if(arr[0]=='V'&&arr[1]=='m'&&arr[2]=='S'&&arr[3]=='i'&&arr[4]=='z'&&arr[5]=='e'){
			printf("memory");
			printf("%s",arr+6);
		}
		itr++;
	}
	fclose(pstatus);
	exeloc(gpid);
}

int exist_pid(int gpid){
	char spid[1000]; char pfile[1000]="/proc/"; char efile[]="/status";
	tostr(spid,gpid); concat(pfile,spid,'\0'); concat(pfile,efile,'\0');
	FILE *pstatus = fopen(pfile,"r");
	if(pstatus==NULL){
		return 0;
	} else return 1;
}


void printdirty(){
	printf("called dirty\n");
	FILE *datafile=fopen("/proc/meminfo","r");
	char arr[1000]; 
	if(datafile==NULL){
		printf("access denied\n");
	}
	while(fgets(arr,1000,datafile)){
		// Dirty
		if(arr[0]=='D'&&arr[1]=='i'&&arr[2]=='r'&&arr[3]=='t'&&arr[4]=='y'){
			printf("%s",arr+6);
		}
	}
}

void cron(char *command);

void command_handler(char *command) {
    // printf("%s\n",command)
    if(fmatch("echo",command)==1) {
        printf("%s\n",command+5);
    } else if(fmatch("pwd",command)==1){
        char cdir[1000]; getcwd(cdir,1000);
        printf("%s\n",cdir);
    } else if(fmatch("cd ",command)==1){
        char ndir[1000];
        abs_dir(command+3,HOME,ndir);
        if(chdir(ndir)==-1){
		    printf("could not change to %s\n",ndir);
	    }
    } else if(fmatch("ls",command)==1) {
        ls(command+2,HOME);
    } else if(fmatch("setenv ",command)==1) {
        msetenv(command+7);
    } else if(fmatch("unsetenv ",command)==1){
        munsetenv(command+9);
    } else if(fmatch("quit",command)==1){
        printf("Bye!\n"); exit(0);
    } else if(fmatch("kjob ",command)==1){
        kjob(command+5);
    } else if(fmatch("overkill",command)==1){
        for(ll i=0;i<no_children;i++) kill(children[i],9);
        no_children=0;
    } else if(fmatch("bg ",command)==1){
        kill(toint(command+3),SIGCONT);  
    } else if(fmatch("cronjob -c ",command)==1){
        cron(command+11);
    } else if(fmatch("fg ",command)==1){
        C_FG_P=(int)toint(command+3);
        kill(toint(command+3),SIGCONT);
        int status;
        waitpid(toint(command+3), &status, WUNTRACED);
        child_remove(C_FG_P);
    } else if(fmatch("jobs",command)==1){
        for(ll i=0;i<no_children;i++){
            if(exist_pid(children[i])==0) child_remove(children[i]);            
        }
        for(ll i=0;i<no_children;i++){
            printf("PROCESS %d\n",children[i]);
        }
    } else if(fmatch("pinfo",command)==1){
        int a;
        if(strlen(command)==5) a=getpid();
        else a=(int)toint(command+6);
        printf("Pinfo of %d\n",a);
        mypinfo(a);
    } else if(fmatch("dirty",command)==1){
		printdirty();
	} else {
        c_ecex(command);
        // printf("coming soon\n");
    }
}

void cron(char *command){
    char tcommand[1000]; strcpy(tcommand,command);
    if(noccur(command,' ')!=4){
        printf("invalid arguments\n"); return;
    }
    char *lpc=strtok(tcommand," ");
    char *ts=strtok(NULL," ");
    ts=strtok(NULL," ");
    char *ps=strtok(NULL," ");
    ps=strtok(NULL," ");
    ll t=toint(ts);
    ll p=toint(ps);
    printf("%s %lld %lld\n",lpc,t,p);
    // printf("%s\n",command);
    ll tt=0;
    while(tt<=p){
        if(tt%t==0) command_handler(lpc);
        tt+=1;
        sleep(1);
    }
}


void detect_red(char *command){
    // printf("%s\n",command);
    ll n=strlen(command); ll toapp=0;
    for(ll i=0;i<n;i++){
        if(command[i]=='>'){ 
            command[i]='\0';
            if(i+1<n&&command[i+1]=='>'){
                i++; toapp++; command[i]='\0';
            }
            char ofile[1000]; strcpy(ofile,command+i+1); sfix(ofile); sfix(command);
            if(toapp==0){
                freopen(ofile, "w", stdout);
            } else {
                freopen(ofile,"a+",stdout);
            }
        }
    }
}

void pipe_cmd(char *commands){
    // printf("piping coming soon!!\n");
    char **cmds=calloc(1000,sizeof(char*)); ll ci=0;
    char *command=strtok(commands,"|");
    while(command!=NULL){
        cmds[ci++]=command; command=strtok(NULL,"|"); sfix(cmds[ci-1]);
    }
    int pfd=0,fd[2];
    for(ll i=0;i<ci;i++){
        // printf("At step %lld\n",i);
        if(pipe(fd)==-1){
            printf("Piping failed\n"); return;
        }
        int pid=fork();
        if(pid<0){
            printf("Fork error\n"); return;
        } else if(pid==0){
            dup2(pfd,0);
            if(i+1<ci){
                dup2(fd[1],1);
                command_handler(cmds[i]);
                exit(1);
            } else {
                detect_red(cmds[i]);
                command_handler(cmds[i]);
                exit(1);
            }
        } else {
            wait(NULL); close(fd[1]); pfd=fd[0];
        }
        // printf("finished step %lld\n",i);
    }
}


int main(){
    signal(SIGINT, sigintHandler);
    // signal(SIGKILL, sigkillHandler);
    SHELL_PID=getpid();
    signal(SIGTSTP, sighandler); 
    getcwd(HOME,1000);
    for(;;){
        init();
        char *commands = readline(ANSI_COLOR_RED "\nFL>>> " ANSI_COLOR_RESET);
        // printf("%s\n",commands);
        if(strlen(commands)>0){
            add_history(commands);
            char **cmds = calloc(1000, sizeof(char*)); ll ci=0;
            char *command=strtok(commands,";");
            while(command!=NULL){
                cmds[ci]=command; command=strtok(NULL,";"); ci++;
            }
            for(ll i=0;i<ci;i++){
                freopen("/dev/tty", "w", stdout); // reset stdout to stdout
                sfix(cmds[i]);
                if(noccur(cmds[i],'<')>1){
                    printf("only 1 < alZlowed\n");
                    continue;
                } else if(noccur(cmds[i],'<')==1){
                    inp_fix(cmds[i]);
                }
                // printf("%s\n",cmds[i]);
                if(noccur(cmds[i],'|')!=0){
                    pipe_cmd(cmds[i]);
                } else {
                    detect_red(cmds[i]);
                    command_handler(cmds[i]);
                    // freopen("/dev/tty", "w", stdout); // reset stdout to stdout
                }
                freopen("/dev/tty", "w", stdout); // reset stdout to stdout
            }
        }
    }
}