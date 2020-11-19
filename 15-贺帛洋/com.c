#include "com.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<memory.h>
#include<string.h>
#include<sys/types.h>
#include<pwd.h>
#include<errno.h>
#include"cmd.h"

#define MAX_LINE 80
#define MAX_NAME_LEN 100
#define MAX_PATH_LEN 1000


char *cmd_array[MAX_LINE/2+1];
int cmd_cnt;


void readcommand(){

	int cnt=0;
	char str[MAX_LINE];
	char* helper;
	memset(cmd_array,0,MAX_LINE/2+1);
	fgets(str,MAX_LINE,stdin);
	if(str[strlen(str)-1]=='\n'){
		str[strlen(str)-1]='\0';
	}
	helper=strtok(str," ");
	while(helper!=NULL){
			cmd_array[cnt]=(char*)malloc(sizeof(*helper));
			strcpy(cmd_array[cnt++],helper);
			helper=strtok(NULL," ");
	}
	cmd_cnt=cnt;
}

int is_internal_cmd(){

	if(cmd_array[0]==NULL){
		return 0;
	}
	else if(strcmp(cmd_array[0],"exit")==0){
		myexit();
	}
	else if(strcmp(cmd_array[0],"clr")==0){
		myclr();
		return 1;
	}

	else if(strcmp(cmd_array[0],"pwd")==0){
		mypwd();
		return 1;
	}
	else if(strcmp(cmd_array[0],"echo")==0){
		myecho();
		return 1;
	}
	else if(strcmp(cmd_array[0],"time")==0){
		mytime();
		return 1;
	}
	else if(strcmp(cmd_array[0],"environ")==0){
			myenviron();
			return 1;
		}
	else if(strcmp(cmd_array[0],"cd")==0){
		mycd();
		return 1;
	}

	else if(strcmp(cmd_array[0],"exec")==0){
		myexec();
		return 1;
	}

	else if(strcmp(cmd_array[0],"jobs")==0){
		myjobs();
		return 1;
	}

	else if(strcmp(cmd_array[0],"dir")==0){
			mydir();
			return 1;
	}

	else{
		return 0;
	}
}


void printprompt(){
	char hostname[MAX_NAME_LEN];
	char pathname[MAX_PATH_LEN];
	struct passwd *pwd;
	pwd=getpwuid(getuid());
	gethostname(hostname,MAX_NAME_LEN);
	getcwd(pathname,MAX_PATH_LEN);
	printf("\e[34mmyshell>%s@%s:\e[0m",pwd->pw_name,hostname);
	if (strncmp(pathname,pwd->pw_dir,strlen(pwd->pw_dir))==0){
		printf("~%s",pathname+strlen(pwd->pw_dir));
	}
	else {
		printf("%s",pathname);
	}
	if (geteuid()==0) {
		printf("#");
	}
	else {
		printf("$");
	}
}

int getcommandlen(){
	int tot_len=0;
	for(int i=0;i<cmd_cnt;i++){
		tot_len+=strlen(cmd_array[i]);
	}
	return tot_len+cmd_cnt-1;
}


void run_external_cmd(int pos){
	int res;
	res=execvp(cmd_array[pos],cmd_array+pos);
	if(res<0){
		printf("myshell: command not found\n");
	}
}
