//gcc main.c -o file_to_shell
// ./file_to_shell test.txt

//насколько я понял подобные решения имеют право на жизнь


// TODO pr1 | pr2  N>=2
// TODO pr1<pr2 pr1>pr2 pr1>>pr2
// TODO pr1 || pr2 (if pr1 failed run pr2)
// TODO ... & (background mode)


#include <assert.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NULL ((void *)0)
#define MAX_COMMAND_LEN 256
#define MAX_NUM_PARAMS 20

int parse_command(char* cmd, char** params) { //split cmd into array of params
    int i,n=-1;
    for(i=0; i<MAX_NUM_PARAMS; i++) {
        params[i] = strsep(&cmd, " ");
        n++;
        if(params[i] == NULL) break;
    }
    return(n);
};

int execute_command(char** params) {
    pid_t pid = fork(); //fork process

    if (pid == -1) {perror("error fork!!\n");return 1;
    } else if (pid == 0) { // child process
        execvp(params[0], params); //exec cmd
        perror("unknown command\n");
        return 0;
    } else { // parent process
        int childstatus;
        waitpid(pid, &childstatus, 0);
        return 1;
    }
};

int execute_pipeline(char ** argv1, char ** argv2) {
    int fds[2];
    pipe(fds);
    pid_t pid = fork();
    if (pid == -1) {perror("error fork!!\n");return 1;}
    if (pid == 0) { // child process
        close(fds[1]);
        dup2(fds[0], 0);
        //close(fds[0]);
        execvp(argv2[0], argv2); // run command AFTER pipe character in File_input
        perror("unknown command\n");
        return 0;
    } else { // parent process
        close(fds[0]);
        dup2(fds[1], 1);
        //close(fds[1]);
        execvp(argv1[0], argv1); // run command BEFORE pipe character in File_input
        perror("unknown command\n");
        return 0;
    }
};

int main(int argc, const char *argv[])
{
    char command[MAX_COMMAND_LEN+1];
    char* params[MAX_NUM_PARAMS+1];
    char* argv1[MAX_NUM_PARAMS+1] = {0};
    char* argv2[MAX_NUM_PARAMS+1] = {0};
    int k, y, x;
    int f = 1;

    if (argc!=2) {perror("filename passed incorrectly");return 1;}
    FILE* fl = fopen(argv[1],"r");
    if (fl == NULL) {perror("open file error");return 1;}

    while(fgets(command,sizeof(command),fl)) {
        //remove newline char (extra security)
        if(command[strlen(command)-1] == '\n') {command[strlen(command)-1] = '\0';}
        int parsed_cnt=parse_command(command, params); //split cmd into array of params
        if (strcmp(params[0], "exit") == 0) break; //exit
        for (k=0; k < parsed_cnt; k++) {
            if (strcmp(params[k], "|") == 0) {
                f = 0; y = k;
                printf("pipe found\n");break;
            }
        }
        if (f==0) {
            for (x=0; x<k; x++) {argv1[x]=params[x];}
            int tmp = 0;
            for (x=k+1; x< parsed_cnt; x++) {
                argv2[tmp]=params[x];
                tmp++;
            }
            if (execute_pipeline(argv1, argv2) == 0) break;
        } else if (f==1) {
            if (execute_command(params) == 0) break;
        }
    }

    //memset(command,0,sizeof(command));

    fclose(fl);
    //TESTFILE worked just fine;
    return 0;
}
