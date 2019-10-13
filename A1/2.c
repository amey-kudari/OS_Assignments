#include<stdio.h> 
#include<stdlib.h>
#include<fcntl.h> 
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>

// 1. File size is less than size of int,
//    if its greater, uncomment next line

//#define int long long int

void print(char *a){
    write(1,a,strlen(a));
}
// utility function to print using syscall write, and arg 1 for stdout to console.

char c[100005],d[100005]; // write data in chunks of 10^5 bytes
void reverse(){
    int n=strlen(c); int i; char ch;
    for(i=0;i<n/2;i++){ ch=c[i]; c[i]=c[n-i-1]; c[n-i-1]=ch; }
}
// utility function to reverse chunk

int main() {
    fflush(stdout); // /r to go to start of line
    int infile, outfile,chkfile; // infile=A.txt, outfile is  A.txt reversed;
    infile=open("A.txt",O_RDONLY);
    chkfile = open("./Assignment/A.txt",O_RDONLY);
    outfile=open("tmp.txt", O_WRONLY | O_CREAT, 0777);

    if(infile<0||outfile<0||chkfile<0){ return -1; }
    int len=lseek(infile,0,SEEK_END); // number of bytes/charecters in A.txt
    int schunks=100000;  // size of chunk being copied at once
                         // this should be less than 1/4th the ram size in bytes.
    int tchunks=len/schunks;
    int extra=len-tchunks*schunks;
    lseek(infile,-extra-1,SEEK_END);
    read(infile,c,extra);
    reverse();
    write(outfile,c,strlen(c));
    int perc=0;
    for(int i=1;i<tchunks;i++){
        perc=(i*100)/tchunks;
        lseek(infile,-schunks*i-extra-1,SEEK_END);
        read(infile,c,schunks);
        c[schunks]='\0';
        reverse();
        write(outfile,c,strlen(c));
    }
    lseek(infile,0,SEEK_SET);
    read(infile,c,schunks-1);
    c[schunks-1]='\0';
    reverse();
    write(outfile,c,strlen(c));

    char folder[]="./Assignment";
    struct stat sb;
    if (stat(folder, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        write(1,"Directory is created: Yes\n",strlen("Directory is created: Yes\n"));
    } else {
        write(1,"Directory is created: No\n",strlen("Directory is created: No\n"));
        printf("NO\n"); return 0;
    }
    int clen=lseek(chkfile,0,SEEK_END);
    close(outfile);
    outfile=open("tmp.txt",O_RDONLY);
    len=lseek(outfile,0,SEEK_END);
    if(clen==len){
        int isc=1;
        tchunks=len/schunks;
        for(int i=0;i<tchunks;i++){
            lseek(chkfile,i*schunks,SEEK_SET);
            lseek(outfile,i*schunks,SEEK_SET);
            read(outfile,c,schunks);
            read(chkfile,d,schunks);
            for(int j=0;j<schunks;j++){ if(c[i]!=d[i]){ isc=0; } }
        }
        lseek(chkfile,tchunks*schunks,SEEK_SET);
        lseek(outfile,tchunks*schunks,SEEK_SET);
        int left=len-tchunks*schunks;
        read(outfile,c,left);
        read(chkfile,d,left);
        for(int j=0;j<left;j++){ if(c[j]!=d[j]){ isc=0; }}
        if(isc==1){
            print("Whether file contents are reversed in newfile: Yes\n");
        } else{
            print("Whether file contents are reversed in newfile: No\n");
        }
        remove("tmp.txt");
    } else {
        print("Whether file contents are reversed in newfile: No\n");
    }
    struct stat file2;
    if(fstat(chkfile,&file2) < 0) return 1;

    print("PERMISSIONS FOR REVERSED FILE\n");
    print("User has read permissions on newfile: ");
    if(file2.st_mode & S_IRUSR) print("Yes\n"); else print("No\n");

    print("User has write permission on newfile: ");
    if(file2.st_mode & S_IWUSR) print("Yes\n"); else print("No\n");

    print("User has execute permission on newfile: ");
    if(file2.st_mode & S_IXUSR) print("Yes\n"); else print("No\n");

    print("Group has read permissions on newfile: ");
    if(file2.st_mode & S_IRGRP) print("Yes\n"); else print("No\n");

    print("Group has write permission on newfile: ");
    if(file2.st_mode & S_IWGRP) print("Yes\n"); else print("No\n");

    print("Group has execute permission on newfile: ");
    if(file2.st_mode & S_IXGRP) print("Yes\n"); else print("No\n");

    print("Others has read permissions on newfile: ");
    if(file2.st_mode & S_IROTH) print("Yes\n"); else print("No\n");

    print("Others has write permission on newfile: ");
    if(file2.st_mode & S_IWOTH) print("Yes\n"); else print("No\n");

    print("Others has execute permission on newfile: ");
    if(file2.st_mode & S_IXOTH) print("Yes\n"); else print("No\n");

    if(fstat(infile,&file2) < 0) return 1;

    print("PERMISSIONS FOR ORIGINAL FILE\n");
    print("User has read permissions on newfile: ");
    if(file2.st_mode & S_IRUSR) print("Yes\n"); else print("No\n");

    print("User has write permission on newfile: ");
    if(file2.st_mode & S_IWUSR) print("Yes\n"); else print("No\n");

    print("User has execute permission on newfile: ");
    if(file2.st_mode & S_IXUSR) print("Yes\n"); else print("No\n");

    print("Group has read permissions on newfile: ");
    if(file2.st_mode & S_IRGRP) print("Yes\n"); else print("No\n");

    print("Group has write permission on newfile: ");
    if(file2.st_mode & S_IWGRP) print("Yes\n"); else print("No\n");

    print("Group has execute permission on newfile: ");
    if(file2.st_mode & S_IXGRP) print("Yes\n"); else print("No\n");

    print("Others has read permissions on newfile: ");
    if(file2.st_mode & S_IROTH) print("Yes\n"); else print("No\n");

    print("Others has write permission on newfile: ");
    if(file2.st_mode & S_IWOTH) print("Yes\n"); else print("No\n");

    print("Others has execute permission on newfile: ");
    if(file2.st_mode & S_IXOTH) print("Yes\n"); else print("No\n");

    print("PERMISSIONS FOR ASSIGNMENT DIRECTORY\n");
    struct stat fold;
    if(stat("./Assignment",&fold)<0) return 1;
    print("User has read permissions on newfile: ");
    if(fold.st_mode & S_IRUSR) print("Yes\n"); else print("No\n");

    print("User has write permission on newfile: ");
    if(fold.st_mode & S_IWUSR) print("Yes\n"); else print("No\n");

    print("User has execute permission on newfile: ");
    if(fold.st_mode & S_IXUSR) print("Yes\n"); else print("No\n");

    print("Group has read permissions on newfile: ");
    if(fold.st_mode & S_IRGRP) print("Yes\n"); else print("No\n");

    print("Group has write permission on newfile: ");
    if(fold.st_mode & S_IWGRP) print("Yes\n"); else print("No\n");

    print("Group has execute permission on newfile: ");
    if(fold.st_mode & S_IXGRP) print("Yes\n"); else print("No\n");

    print("Others has read permissions on newfile: ");
    if(fold.st_mode & S_IROTH) print("Yes\n"); else print("No\n");

    print("Others has write permission on newfile: ");
    if(fold.st_mode & S_IWOTH) print("Yes\n"); else print("No\n");

    print("Others has execute permission on newfile: ");
    if(fold.st_mode & S_IXOTH) print("Yes\n"); else print("No\n");

}
