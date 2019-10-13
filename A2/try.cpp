#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>


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
    printf(" %lld ",fileStat.st_nlink);   

    // file owner and group
    struct passwd *pw = getpwuid(fileStat.st_uid);
    struct group  *gr = getgrgid(fileStat.st_gid);
    printf(" %s %s ", pw->pw_name, gr->gr_name);

    // file size
    printf("%10d ",fileStat.st_size);

    // file modification time
    // printf(" %d",fileStat.st_mtime);

    // file name
    char filen[1000];
    int fsl=-1; int n=strlen(argv);
    for(int i=0;i<n;i++) if(argv[i]=='/') fsl=i;
    for(int i=fsl+1;i<=n;i++) filen[i-(fsl+1)]=argv[i];
    printf("%s\n",filen);


}
int main(){
    char argv[]="/home/f/assignment/MAIN/OS/A2/check.txt";
    // char argv[]="check.txt";
    _ls_l(argv);
    return 0;
}
