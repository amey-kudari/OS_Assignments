#include "header.h"

int isFile(const char* name){
    DIR* directory = opendir(name);
    if(directory != NULL){
     	closedir(directory);
     	return 0;
    }
    if(errno == ENOTDIR) return 1;    
}

void plain(char *tdir, ll a) {
    printf("ls on %s\n",tdir);
    struct dirent **namelist;
	int n=scandir(tdir,&namelist,NULL,alphasort);
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
	        free(namelist[n]);
		}
	}
	else{ 
		while (n--) { 
			if(*(namelist[n]->d_name)!='.'){
				char arr[1000]="\0"; concat(arr,dir,'\0');
				if(arr[strlen(arr)-1]=='/') concat(arr,namelist[n]->d_name,'\0');
				else concat(arr,namelist[n]->d_name,'/');
				_ls_l(arr);	
			} 
	        free(namelist[n]);
		}
	}
}


void ls(char *command,char *home) {
    char tcommand[1000],tdir[1000]; strcpy(tcommand,command); sfix(tcommand);
    printf("%s\n",tcommand);
    if(noccur(tcommand,'-')==0){
        if(strlen(tcommand)==0){
            tcommand[0]='.'; tcommand[1]='\0';
        }
        abs_dir(tcommand,home,tdir);
        plain(tdir,0);
    } else {
        if(fmatch("-l -a",tcommand)||fmatch("-a -l",tcommand)){
            if(strlen(tcommand)<=5){
                tcommand[5]=' '; tcommand[6]='.'; tcommand[7]='\0';
            }
            abs_dir(tcommand+5,home,tdir);
            lsl(tdir,1);
        } else if((tcommand[1]=='a'&&tcommand[2]=='l')||(tcommand[1]=='l'&&tcommand[2]=='a')){
            if(strlen(tcommand)<=4){
                tcommand[3]=' '; tcommand[4]='.'; tcommand[5]='\0';
            }
            abs_dir(tcommand+4,home,tdir);
            lsl(tdir,1);
        } else if(tcommand[1]=='l'){
            if(strlen(tcommand)<=2){
                tcommand[2]=' '; tcommand[3]='.'; tcommand[4]='\0';
            }
            abs_dir(tcommand+3,home,tdir);
            lsl(tdir,0);
        } else if(tcommand[1]=='a'){
            if(strlen(tcommand)<=2){
                tcommand[2]=' '; tcommand[3]='.'; tcommand[4]='\0';
            }
            abs_dir(tcommand+3,home,tdir);
            plain(tdir,1);
        } else {
            printf("Invalid arguments\n");
        }
    }
}