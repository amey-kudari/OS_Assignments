#include "header.h"

#define ll long long int

// add brr to arr
char* sadd(char *arr,char *brr,char c){
    ll na=strlen(arr);
    ll nb=strlen(brr);
    if(sizeof(arr)<na+nb+2){
        char *narr=(char *) malloc(2+na+nb);
        for(ll i=0;i<=na;i++) narr[i]=arr[i];
        arr=narr;
    }
    if(c!='\0'){
        arr[na]=c; na++;
    }
    for(ll i=na;i<na+nb;i++) arr[i]=brr[i-na];
    arr[na+nb]='\0'; 
    return arr;
}

ll toint(char *str){
	ll n=strlen(str); ll ret=0;
	for(ll i=0;i<n;i++) if(str[i]<='9'&&str[i]>='0'){
		ret*=10; ret+=(str[i]-'0');
	} return ret;
}

void tostr(char *arr,ll a){
    ll n=0;
    while(a>0){
        ll tmp=a%10; a=a/10; arr[n]=tmp+'0'; n++;
    } 
    for(ll i=0;i<n/2;i++){
        char tm=arr[i]; arr[i]=arr[n-i-1]; arr[n-i-1]=tm;
    }
}

ll noccur(char *arr,char f){
    ll n=strlen(arr); ll no=0;
    for(ll i=0;i<n;i++){
        if(arr[i]==f) no++;
    } return no;
}

ll *toccur(char *arr,char f){
    ll to=noccur(arr,f);
    ll *occur=(ll *) malloc(sizeof(ll)*(to+2));
    ll ind=0; ll n=strlen(arr);
    printf("%s\n",arr);
    for(ll i=0;i<n;i++){
        if(arr[i]==f) occur[ind++]=i;
    } return occur;
}

ll max(ll a,ll b){ if(a>b) return a; return b; }
ll min(ll a,ll b){ if(a<b) return a; return b; }

void sfix(char *arr){
    char narr[1000]; ll np=0;
    ll n=strlen(arr);
    char prev=' ';
    for(ll i=0;i<n;i++){
        if(prev!=' '||arr[i]!=' '){
            narr[np++]=arr[i]; prev=arr[i];
        }
    } narr[np]='\0';
    if(np>0&&narr[np-1]==' ') arr[np-1]='\0';
    strcpy(arr,narr);
}

void fstrip(char *arr){
    char narr[1000]; ll np=0;
    ll n=strlen(arr);
    char prev=' ';
    for(ll i=0;i<n;i++){
        if(prev!=' '||arr[i]!=' '){
            narr[np++]=arr[i]; prev='f';
        }
    } narr[np]='\0';
    strcpy(arr,narr);
    n=strlen(arr);
    if(n>0&&arr[n-1]==' ') arr[n-1]='\0';
}

ll fmatch(char *arr, char *brr){  // match till strlen(arr)
    ll n=strlen(arr);
    if(strlen(brr)<strlen(arr)){
        return 0;
    } else {
        for(ll i=0;i<n;i++){
            if(arr[i]!=brr[i]){
                return 0;
            }
        }
    }
    return 1;
}

void concat(char *arr, char *brr,char add){
	int an=strlen(arr); int bn=strlen(brr);
	if(add!='\0'){
		arr[an]=add; an++; arr[an]='\0';
	}
	for(int i=0;i<=bn;i++) arr[i+an]=brr[i];
}

void inp_fix(char *arr){
    char **sparr=calloc(1000,sizeof(char *)); ll ci=0;
    char *arr_c=strtok(arr," ");
    while(arr_c!=NULL){
        sparr[ci++]=arr_c; arr_c=strtok(NULL," ");
    }
    ll iri=-1;
    for(ll i=1;i<ci;i++){
        if(i>0&&strcmp(sparr[i-1],"<")==0){
            iri=i-1;
            char trarr[1000]="cat ";
            ll n=strlen(sparr[i]);
            for(ll j=0;j<=n;j++){
                trarr[j+4]=sparr[i][j];
            }
            sparr[i]=trarr;
        }
    }
    if(iri<1||iri==ci-1){
        strcpy(arr,"echo invalid_redirection");
        return;
    }
    char narr[1000]; ll ni=0;
    sparr[iri][0]='|';
    char *brr=sparr[iri-1];
    sparr[iri-1]=sparr[iri+1];
    sparr[iri+1]=brr;
    for(ll i=0;i<ci;i++){
        ll tn=strlen(sparr[i]);
        for(ll j=0;j<tn;j++){
            narr[ni++]=sparr[i][j];
        }
        if(i+1<ci) narr[ni++]=' ';
        else narr[ni++]='\0';
    }
    strcpy(arr,narr);
}

