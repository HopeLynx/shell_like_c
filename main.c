//had to make :
//in pdf, done - file_to_shell bridge

//gcc main.c -o file_to_shell
// ./file_to_shell test.txt

//насколько я понял подобные решения имеют право на жизнь

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NULL ((void *)0)
#define MAX_COMMAND_LEN 256

int main(int argc, const char *argv[])
{
    char command[MAX_COMMAND_LEN];

    if (argc!=2) {perror("filename passed incorrectly");return 1;}
    FILE* fl = fopen(argv[1],"r");
    if (fl == NULL) {perror("open file error");return 1;}

    while(fgets(command,MAX_COMMAND_LEN,fl)){
        system(command);
        memset(command,0,MAX_COMMAND_LEN);
    }
    fclose(fl);
    //TESTFILE worked just fine;

    return 0;
}
