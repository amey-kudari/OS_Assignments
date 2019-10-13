#include "header.h"

void abs_dir(char *tarr,char *home,char *narr){
    char cdir[1000],cname[1000],arr[1000];
    strcpy(arr,tarr);
	getcwd(cdir,sizeof(cdir));
    sfix(arr);
    if(strlen(arr)==0) return;
    ll n=strlen(cdir); ll an=strlen(arr);
    if(arr[0]=='.'&&(strlen(arr)==1||arr[1]!='.')){
        for(ll i=1;i<=an;i++){
            cdir[i+n-1]=arr[i];
        }
        strcpy(arr,cdir);
    } else if(arr[0]=='~'){
        strcpy(cdir,home);
        for(ll i=1;i<=an;i++){
            cdir[i+n-1]=arr[i];
        strcpy(arr,cdir);
        }
    } else if(arr[0]!='/'){
        cdir[n++]='/';
        for(ll i=0;i<=an;i++) cdir[n+i]=arr[i];
        strcpy(arr,cdir);
    }
    strcpy(narr,arr);
}