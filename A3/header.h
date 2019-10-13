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

#define ll long long int

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

// STRING FUNCTIONS
char* sadd(char *arr,char *brr,char c);
ll toint(char *str);
void tostr(char *arr,ll a);
ll noccur(char *arr,char f);
ll *toccur(char *arr,char f);
void fstrip(char *arr);
void sfix(char *arr);
ll fmatch(char *arr, char *brr);  // match till strlen(arr)
void concat(char *arr, char *brr,char add);
void inp_fix(char *arr);

// LL UTILITY
ll max(ll a,ll b);
ll min(ll a,ll b);

void msetenv(char *command);
void munsetenv(char *command);

void abs_dir(char *tarr,char *home,char *narr);
void ls(char *command,char *home);
void kjob(char *arr);
void c_ecex(char *command);

int SHELL_PID,C_FG_P;

int children[1000]; // increase if 1000+ processes in background
ll no_children;
// ll chs=0; // size of children