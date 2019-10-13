#include<stdio.h> 
#include<stdlib.h>
#include<fcntl.h> 
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>

// ASSUMTIONS MADE:
//
// 1. we reverse entire string not line by line.
//     eg, "asdf\n as" -> "sa \nfdsa"
//
// 2. Code runs on fresh directory where directory
//    called "Assignment" does not exist
//
// 3. File size is less than max value of int,
//    if its greater, uncomment next line

// #define int long long int

void print(char *a){
    write(1,a,strlen(a));
}
// utility function to print using syscall write, and arg 1 for stdout to console.

char c[100005]; // write data in chunks of 10^5 bytes
void reverse(){
    int n=strlen(c); int i; char ch;
    for(i=0;i<n/2;i++){ ch=c[i]; c[i]=c[n-i-1]; c[n-i-1]=ch; }
}
// utility function to reverse chunk

int main() {
    fflush(stdout); // /r to go to start of line
    int infile, outfile; // infile=A.txt, outfile is  A.txt reversed;
    infile=open("A.txt",O_RDONLY);
    mkdir("Assignment", 0777);
    outfile=open("./Assignment/A.txt", O_WRONLY | O_CREAT | O_APPEND, 0777);

    int len=lseek(infile,0,SEEK_END); // number of bytes/charecters in A.txt
    int schunks=100000;  // size of chunk being copied at once
    // this should be less than 1/4th the ram size in bytes and less than sizeof(c).
    int tchunks=len/schunks;
    int extra=len-tchunks*schunks;
    lseek(infile,-extra-1,SEEK_END);
    read(infile,c,extra);
    reverse();
    write(outfile,c,strlen(c));
    int perc=0;
    char percf[]="\r000 \% written";
    char nums[]="0123456789";
    for(int i=1;i<tchunks;i++){
        perc=(i*100)/tchunks;
        print(percf);
        percf[1]=nums[perc/100];
        percf[2]=nums[(perc/10)%10];
        percf[3]=nums[perc%10];
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
    print("\r100 \% written\n");
}

