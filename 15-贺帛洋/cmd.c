#include "cmd.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<pwd.h>
#include<errno.h>
#include<time.h>
#include<fcntl.h>
#include<dirent.h>
#include <stddef.h>

#define MAX_LINE 80
#define MAX_NAME_LEN 100
#define MAX_PATH_LEN 1000


extern char **environ;
char *cmd_array[MAX_LINE/2+1];
int cmd_cnt;

void myexit(){
	exit(0);
}

void myclr(){
	printf("\033[2J");
	printf("\033[H");
}



void mypwd(){
	char pathname[MAX_PATH_LEN];
	if(getcwd(pathname,MAX_PATH_LEN)){
		printf("%s\n",pathname);
	}
	else{
		perror("myshell: getcwd");
		exit(1);
	}
}

void myecho(){
	for(int i=1;i<cmd_cnt;i++){
		printf("%s",*(cmd_array+i));
		if(i==cmd_cnt-1){
			break;
		}
		printf(" ");
	}
	printf("\n");
}



void mytime(){
	int weekday;
	int month;
	time_t tvar;
	struct tm *tp;
	time(&tvar);
	tp=localtime(&tvar);
	weekday=tp->tm_wday;
	switch(weekday){
	case 1:
		printf("Mon ");
		break;
	case 2:
		printf("Tues ");
		break;
	case 3:
		printf("Wed ");
		break;
	case 4:
		printf("Thur ");
		break;
	case 5:
		printf("Fri ");
		break;
	case 6:
		printf("Sat ");
		break;
	case 7:
		printf("Sun ");
		break;
	default:
		break;
	}
	month=1+tp->tm_mon;
	switch(month){
	case 1:
		printf("Jan ");
		break;
	case 2:
		printf("Feb ");
		break;
	case 3:
		printf("Mar ");
		break;
	case 4:
		printf("Apr ");
		break;
	case 5:
		printf("May ");
		break;
	case 6:
		printf("Jun ");
		break;
	case 7:
		printf("Jul ");
		break;
	case 8:
		printf("Aug ");
		break;
	case 9:
		printf("Sep ");
		break;
	case 10:
		printf("Oct ");
		break;
	case 11:
		printf("Nov ");
		break;
	case 12:
		printf("Dec ");
		break;
	default:
		break;
	}
	printf("%d ",tp->tm_mday);
	printf("%d:",tp->tm_hour);
	printf("%d:",tp->tm_min);
	printf("%d ",tp->tm_sec);
	printf("CST ");
	printf("%d\n",1900+tp->tm_year);
}

void myenviron(){
	for(int i=0;environ[i]!=NULL;i++){
		printf("%s\n",environ[i]);
	}
}

void mycd(){
	struct passwd *pwd;
	char pathname[MAX_PATH_LEN];
	pwd=getpwuid(getuid());
	if(cmd_cnt==1){
		strcpy(pathname,pwd->pw_dir);
		if(chdir(pathname)==-1){
			perror("myshell: chdir");
			exit(1);
		}
	}
	else{
		if(chdir(cmd_array[1])==-1){
			printf("myshell: cd: %s :No such file or directory\n",cmd_array[1]);
		}
	}
}

void myexec(){
	int res;
	pid_t pid;
	if(cmd_cnt==1){
		return;
	}
	else{
		pid=fork();
		if(pid<0){
			perror("fork");
			exit(1);
		}
		else if(pid==0){
			exit(0);
		}
		else{
			res=execvp(cmd_array[1],cmd_array+1);
			if(res<0){
				printf("myshell: command not found\n");
			}
			exit(0);
		}
	}
}

void myjobs(){
	
	pid_t pid;
	pid=fork();
	if(pid<0){
		perror("myshell: fork");
	}
	else if(pid==0){
		if(cmd_cnt>1){
			printf("myshell: jobs: incorrect use of jobs\n");
		}
		else{
			execlp("ps","ps","ax",NULL);
		}
	}
	else{
		waitpid(pid,NULL,0);
	}
}


void mydir(){
	char pathname[MAX_PATH_LEN];
	DIR *dir;
	struct dirent *dp;
	if(!getcwd(pathname,MAX_PATH_LEN)){
		perror("myshell: getcwd");
		exit(1);
	}
	dir=opendir(pathname);
	printf("the directory(ies) under the current path is(are):\n");
	while((dp=readdir(dir))!=NULL){
		printf("%s\n",dp->d_name);
	}
}
