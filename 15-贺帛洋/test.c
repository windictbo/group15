#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<signal.h>
#include"com.h"
#include"cmd.h"

#define MAX_LINE 80
#define MAX_NAME_LEN 100
#define MAX_PATH_LEN 1000


char *cmd_array[MAX_LINE/2+1];
int cmd_cnt;

int main(){
	int should_run=1;
	pid_t pid;
	int cmd_len=0;
	while(should_run){
		printprompt();
		readcommand();
		cmd_len=getcommandlen();
		if(cmd_len>MAX_LINE){
			printf("the length of your input is too long to be read in\n");
			exit(1);
		}
		if(is_internal_cmd()){
			continue;
		}
		if((pid=fork())==0){
			int thispid=getpid();
			signal(SIGINT,SIG_DFL);
			signal(SIGTSTP,SIG_DFL);
			signal(SIGCONT,SIG_DFL);
			run_external_cmd(0);
			break;
		}
		else if(pid>0){
			signal(SIGINT,SIG_IGN);
			signal(SIGTSTP,SIG_IGN);
			signal(SIGCONT,SIG_DFL);
		}
		else{
			perror("myshell: fork");
			break;
		}
	}
	return 0;
}

