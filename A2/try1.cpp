#include<string.h>
#include<stdio.h>
void concat(char *arr, char *brr,char add){
	int an=strlen(arr); int bn=strlen(brr);
	if(add!='\0'){
		arr[an]=add; an++; arr[an]='\0';
	}
	for(int i=0;i<=bn;i++) arr[i+an]=brr[i];
}
int main(){
	char arr[1000]="\0";
	char brr[]="filename.txt";
	concat(arr,brr,'/');
	printf("%s\n",arr);
	// printf("GOT HERE  ALSO");
	return 0;
}



// 1 2 3 4 5 6 7 8 9 10
// arr[i%10]; arr[(i+9)%10];