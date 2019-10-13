#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <signal.h>
#include <stdbool.h>
bool sig_caught=false;
bool in_nightwatch=false;
int hasex=1;

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

int toint(char *str){
	int n=strlen(str); int ret=0;
	for(int i=0;i<n;i++) if(str[i]<='9'&&str[i]>='0'){
		ret*=10; ret+=(str[i]-'0');
	} return ret;
}

void tostr(char *arr,int a){
    int n=0;
    while(a>0){
        int tmp=a%10; a=a/10; arr[n]=tmp+'0'; n++;
    } 
    for(int i=0;i<n/2;i++){
        char tm=arr[i]; arr[i]=arr[n-i-1]; arr[n-i-1]=tm;
    }
}

int max(int a,int b){ if(a>b) return a; return b; }
int min(int a,int b){ if(a<b) return a; return b; }

void concat(char *arr, char *brr,char add){
	int an=strlen(arr); int bn=strlen(brr);
	if(add!='\0'){
		arr[an]=add; an++; arr[an]='\0';
	}
	for(int i=0;i<=bn;i++) arr[i+an]=brr[i];
}

int isFile(const char* name){
    DIR* directory = opendir(name);
    if(directory != NULL){
     	closedir(directory);
     	return 0;
    }
    if(errno == ENOTDIR) return 1;    
}

char HOME[1000],Clist[1000][1000];
char history_commands[25][1000]; int hpointer;

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

void correct(char *arr){
	int cn=0; arr[cn]=arr[0]; cn++; int n=strlen(arr);
	for(int i=1;i<n;i++) 
		if(arr[i]!=' '||(arr[i-1]!=' '&&arr[i-1]!=';')) 
			arr[cn++]=arr[i];
	arr[cn]='\0';
	if(arr[0]==' '){
		for(int i=1;i<=cn;i++) arr[i-1]=arr[i];
	}
}
void print_history(int n){
	int curr=hpointer-1;
	for(int i=0;i<n;i++){
		if(curr<0) curr=19; printf("%s\n",history_commands[curr]); curr--;
	}
}

void nls(char *dir, int a){
	struct dirent **namelist;
	int n=scandir(dir,&namelist,NULL,alphasort);
	if(n<0) return;
	if(a==1){ 
		while (n--) {
			if(isFile(namelist[n]->d_name)==0) {
				printf(ANSI_COLOR_CYAN "%s\n" ANSI_COLOR_RESET,namelist[n]->d_name);
			} else {
				printf("%s\n",namelist[n]->d_name);
			}
			free(namelist[n]); 
		} 
	}
	else{ 
		while (n--) { 
			if(*(namelist[n]->d_name)!='.'){
				if(isFile(namelist[n]->d_name)==0) {
					printf(ANSI_COLOR_CYAN "%s\n" ANSI_COLOR_RESET,namelist[n]->d_name);
				} else {
					printf("%s\n",namelist[n]->d_name);
				} 			
			}
			free(namelist[n]); 
		} 
	}
}

void _ls_l(char* argv){
    struct stat fileStat;
    if(stat(argv,&fileStat) < 0) return ;

    // File Permissions
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");

    // number of links 
    printf(" %ld ",fileStat.st_nlink);   

    // file owner and group
    struct passwd *pw = getpwuid(fileStat.st_uid);
    struct group  *gr = getgrgid(fileStat.st_gid);
    printf(" %s %s ", pw->pw_name, gr->gr_name);

    // file size
    printf("%10ld ",fileStat.st_size);

    // file modification time
    char *ntime=ctime(&fileStat.st_mtime);
    ntime[strlen(ntime)-1]='\0';
    printf("%11s ",ntime);

    // file name
    char filen[1000];
    int fsl=-1; int n=strlen(argv);
    for(int i=0;i<n;i++) if(argv[i]=='/') fsl=i;
    for(int i=fsl+1;i<=n;i++) filen[i-(fsl+1)]=argv[i];
    printf("%s\n",filen);
}

void lsl(char *dir, int a){
	struct dirent **namelist;
	int n=scandir(dir,&namelist,NULL,alphasort);
	if(n<0) return;
	if(a==1){ 
		while (n--) {
			char arr[1000]="\0"; concat(arr,dir,'\0');
			if(arr[strlen(arr)-1]=='/') concat(arr,namelist[n]->d_name,'\0');
			else concat(arr,namelist[n]->d_name,'/');
			_ls_l(arr);
		}
	}
	else{ 
		while (n--) { 
			if(*(namelist[n]->d_name)!='.'){
				char arr[1000]="\0"; concat(arr,dir,'\0');
				if(arr[strlen(arr)-1]=='/') concat(arr,namelist[n]->d_name,'\0');
				else concat(arr,namelist[n]->d_name,'/');
				_ls_l(arr);	
				free(namelist[n]);
			} 
		} 
	}
}

void mycd(char *newdir_raw){
	// printf("Called cd to %s\n",newdir_raw);
	char newdir[1000];
	if((newdir_raw[0]=='.')&&(newdir_raw[1]=='/')){
		getcwd(newdir,1000);
		int cn=strlen(newdir);
		int an=strlen(newdir_raw);
		for(int i=1;i<=an;i++) newdir[cn+i-1]=newdir_raw[i]; newdir[cn+an]='\0';
	} else if(strlen(newdir_raw)>1&&((newdir_raw[0]=='~')&&(newdir_raw[1]=='/'))){
		int cn=strlen(HOME);
		for(int i=0;i<=cn;i++) newdir[i]=HOME[i];
		int an=strlen(newdir_raw);
		for(int i=1;i<=an;i++) newdir[cn+i-1]=newdir_raw[i]; newdir[cn+an]='\0';
	} else if(newdir_raw[0]=='/'){
		int cn=strlen(newdir_raw); for(int i=0;i<=cn;i++) newdir[i]=newdir_raw[i];
	} else if(newdir_raw[0]=='~'){
		int cn=strlen(HOME);
		for(int i=0;i<=cn;i++) newdir[i]=HOME[i];
	} else{
		getcwd(newdir,1000); int cn=strlen(newdir); newdir[cn]='/'; cn++;
		int tn=strlen(newdir_raw); for(int i=0;i<tn;i++) newdir[cn+i]=newdir_raw[i];
		newdir[tn+cn]='\0';
	}
	// printf("switch to %s\n",newdir);
	if(chdir(newdir)==-1){
		printf("could not change to %s\n",newdir);
	}
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

void exec_command(char *command);

void sigintHandler(int sig_num) { 
    signal(SIGINT, sigintHandler); 
    if(in_nightwatch){
    	sig_caught=true;
    } else {
    	exit(0);
    }
}

void nightwatch(char *command,int tint){
	in_nightwatch=true;
	if(command[0]==' '){
		int cn=strlen(command); for(int i=0;i<cn;i++) command[i]=command[i+1];
	}
	printf("%s every %d\n",command,tint);
	for(;;){
		// char line[100];
		exec_command(command);
		for(int i=0;i<tint;i++){
			sleep(1);
			if(sig_caught){
				printf("\nExiting nightwatch\n");
				in_nightwatch=false; sig_caught=false; return;
			}
		}
	}
}


int bg(char** argv){
	int p1=fork();
	if(p1!=0) return -1;
	
	int p2=fork();
	if(p2<0) return -1;
	else if(p2==0){
		execvp(argv[0],argv);
	}
	else {
		int status,cid;
		cid=waitpid(p2,&status,0);
		if(WIFEXITED(status)==1) printf("process %s with pid %d exited normally\n",argv[0],cid);
		else printf("process %s with pid %d exited abnormally\n",argv[0],cid); 		
	}
}

void exec_command(char *command){
	char* mainc=strtok(command," ");
	// printf("||%s||\n",mainc);
	if(strcmp(mainc,"cd")==0){
		char *argc=strtok(NULL," ");
		if(argc!=NULL) if(!(strlen(argc)==1&&argc[0]=='.')) mycd(argc);
	} else if(strcmp(mainc,"history")==0){
		char *argc=strtok(NULL," ");
		int no=0;
		if(argc!=NULL) no=min(toint(argc),10);
		print_history(no);
	} else if(strcmp(mainc,"echo")==0){
		char *argc=strtok(NULL," ");
		while(argc!=NULL){ printf("%s ",argc); argc=strtok(NULL," "); } printf("\n"); 
	} else if(strcmp(mainc,"pwd")==0){
		char tmp[1000]; getcwd(tmp,sizeof(tmp)); printf("%s\n",tmp);
	} else if(strcmp(mainc,"sleep")==0){
		char *argc=strtok(NULL," ");
		sleep(toint(argc));
	} else if(strcmp(mainc,"ls")==0){
		char *argc=strtok(NULL," ");
		if(argc==NULL) nls(".",0);
		else{
			if(argc[0]=='-'){
				if((argc[1]=='a')&&(strlen(argc)==2)){
					char *argc=strtok(NULL," ");
					if(argc==NULL) nls(".",1);
					else nls(argc,1);
				} else if((argc[1]=='l')&&strlen(argc)==2){
					char *argc=strtok(NULL," ");
					if(argc==NULL) lsl(".",0);
					else lsl(argc,0);
				} else if((strlen(argc)==3)&&((argc[1]=='a'||argc[2]=='a')&&(argc[1]=='l'||argc[2]=='l'))){
					char *argc=strtok(NULL," ");
					if(argc==NULL) lsl(".",1);
					else lsl(argc,1);
				} else {
					printf("invalid argument %s",argc);
				}
			} else{
				nls(argc,0);
			}
		}
	} else if(strcmp(mainc,"pinfo")==0){
		int a; char *argc=strtok(NULL," ");
		if(argc==NULL) a=getpid();
		else a=toint(argc);
		mypinfo(a);
	} else if(strcmp(mainc,"dirty")==0){
		printdirty();
	} else if(strcmp(mainc,"nightwatch")==0){
		char arr[1000]="\0";
		char *argc=strtok(NULL," ");
		int t=0;
		if(argc==NULL){
			printf("invalid arguments\n"); return;
		} argc=strtok(NULL," ");
		if(argc==NULL){
			printf("invalid arguments\n"); return;
		} else {
			t=toint(argc);
			printf("%s %d\n",argc,t);
		} argc=strtok(NULL," ");
		if(argc==NULL){
			printf("No arguments given\n"); return;
		}
		while(argc!=NULL){
			concat(arr,argc,' '); 
			argc=strtok(NULL," ");
		}
		nightwatch(arr,t);
	} else {
		int pid=fork();
		if(pid){
			wait(NULL);
		} else {
			char tmp[1000];
			char *argc=strtok(NULL," ");
			int cit=1;
			char *args[]={mainc,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
						  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
						  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
			while(argc!=NULL){
				args[cit]=argc; argc=strtok(NULL," "); cit++; if(cit==33) break;
			}
			// printf("||%c||\n",args[cit-1][0]);
			if(args[cit-1][0]!='&'){
				if(execvp(args[0],args)==-1)
					printf("invalid command\n");
			} else {
				printf("BACKGROUND PROCESS!!!\n");
				bg(args);
			}
		}
	}
}

int main(){
    signal(SIGINT, sigintHandler);
	getcwd(HOME,1000);
	for(;;){
		init();
		char *command = readline(ANSI_COLOR_RED "\nFL>>> " ANSI_COLOR_RESET);
		if(strlen(command)!=0){
			add_history(command); correct(command);
			strcpy(history_commands[hpointer],command); hpointer=(hpointer+1)%20;
		}
		int n=strlen(command); int nc=0,ind=0;
        for(int i=0;i<n;i++){
            if(command[i]==';'){
				Clist[nc][ind]='\0';
                nc++; ind=0;
            }
            else{
                Clist[nc][ind]=command[i];
                ind++;
            }
        }
        if(n>0){ Clist[nc][ind]='\0'; nc++; }
        if(nc>0&&strlen(Clist[nc-1])==0) nc--;
        for(int i=0;i<nc;i++) if(strlen(Clist[i])!=0) exec_command(Clist[i]);
   	
	}
}