//TODO mv old_file new_file (rename) DONE
//TODO grep substring filename (-v inverting) print strings with substr DONE
//TODO tail filename (-n n last  +n from n to last) DONE



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NULL ((void *)0)
#define MAX_STR_LEN 256

int main(int argc, const char* argv[])
{
    /*
    int a = 0 ^ 0;
    printf("a = %d\n",a); //0
    a = 0 ^ 1;
    printf("a = %d\n",a); //1
    a = 1 ^ 0;
    printf("a = %d\n",a); //1
    a = 1 ^ 1;
    printf("a = %d\n",a); //0
    */
    int fl_n_minus=0,fl_n_plus=0,fl_v=0;
    if (argc < 2) return 1;
    for (int i=2;i<argc;i++){
        if (argv[i][0] == '-'){
            if (argv[i][1]=='v') fl_v =1;
            if (argv[i][1]=='n') fl_n_minus =1;
        } else if (argv[i][0] == '+') {
            if (argv[i][1]=='n') fl_n_plus =1;
        }
    }

    if (strcmp(argv[1],"mv") == 0 && argc == 4) {
        if (rename(argv[2],argv[3])) perror("rename error");
    }
    else if (strcmp(argv[1],"grep") == 0 && argc >= 4){
        char str[MAX_STR_LEN];
        FILE* fl = fopen(argv[3],"r");
        if (fl == NULL) {perror("open file error");return 1;}

        while(fgets(str,MAX_STR_LEN,fl)){
            if ((strstr(str,argv[2]) != 0) ^ fl_v) printf("%s",str);
            memset(str,0,MAX_STR_LEN);
        }
        fclose(fl);
    }
    else if (strcmp(argv[1],"tail") == 0 && argc >= 3) {
        char str[MAX_STR_LEN];
        FILE* fl = fopen(argv[2],"r");
        if (fl == NULL) {perror("open file error");return 1;}
        int lines =0;
        while(!feof(fl)) if(fgetc(fl) == '\n') lines++; // oneliner str_counter
        fclose(fl);
        fl = fopen(argv[2],"r");
        if (fl == NULL) {perror("open file error");return 1;}
        int cnt = 0,start = lines;
        if (!fl_n_plus && !fl_n_minus){
            start = lines - 10;
        } else if (fl_n_minus) {
            start = lines - atoi(argv[4]);
        } else if (fl_n_plus)  {
            start = atoi(argv[4])-1;
        }
        if(start < 0) start = 0;
        while(fgets(str,MAX_STR_LEN,fl)){
            if (cnt >= start) printf("%s",str);
            cnt++;
            memset(str,0,MAX_STR_LEN);
        }
        fclose(fl);
    } else perror("no command found");
    return 0;
}



