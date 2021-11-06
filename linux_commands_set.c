// gcc main.c -o myset
// ./myset tail filename.txt
// ./myset grep substring filename.txt
// ./myset mv filename.txt new_name.txt

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NULL ((void *)0)
#define MAX_STR_LEN 256

//TODO mv old_file new_file (rename) DONE
//TODO grep substring filename (-v inverting) print strings with substr DONE
//TODO tail filename (-n n last  +n from n to last) DONE


int get_str(int fd,char* buf,int buf_size){
    char c[1];int cnt =0;
    memset(buf,0,buf_size);
    do {
        if (cnt == buf_size-1) {perror("line is too long");return -1;}
        int offset = read(fd,&c,1);
        cnt+= offset;
        strcat(buf,c);
    } while (!(c[0] == '\n' || c[0] == '\0'));
    return cnt;
}



int main(int argc, const char* argv[])
{
/*
//Debug
    int fd = open("./tmp.txt",O_RDONLY);
    if (fd == -1) {perror("open file error");return 1;}
    char buf[MAX_STR_LEN];int ret=1;int cnt=1;
    while((ret = get_str(fd,buf,MAX_STR_LEN))>0){
        printf("%d : %d : %s",cnt,ret,buf);
        cnt++;
        memset(buf,0,MAX_STR_LEN);
    }
    close(fd);
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
    //mv
    if (strcmp(argv[1],"mv") == 0 && argc == 4) {
        if (rename(argv[2],argv[3])) perror("rename error");
    }
    //grep
    else if (strcmp(argv[1],"grep") == 0 && argc >= 4){
        char str[MAX_STR_LEN];
        int fd = open(argv[2],O_RDONLY);
        if (fd == -1) {perror("open file error");return 1;}

        while(get_str(fd,str,MAX_STR_LEN)>0){
            if ((strstr(str,argv[2]) != 0) ^ fl_v) printf("%s",str);
            memset(str,0,MAX_STR_LEN);
        }
        close(fd);
    }
    //tail
    else if (strcmp(argv[1],"tail") == 0 && argc >= 3) {
        char str[MAX_STR_LEN]; char c; int ret;
        int fd = open(argv[2],O_RDONLY);
        int lines =0;
        while ((ret = read (fd, &c, 1)) > 0) {if(c == '\n') lines++;}
        close(fd);
        fd = open(argv[2],O_RDONLY);
        if (fd == -1) {perror("open file error");return 1;}
        int cnt = 0,start = lines;
        if (!fl_n_plus && !fl_n_minus){
            start = lines - 10;
        } else if (fl_n_minus) {
            start = lines - atoi(argv[4]);
        } else if (fl_n_plus)  {
            start = atoi(argv[4])-1;
        }
        if(start < 0) start = 0;
        while((ret = get_str(fd,str,MAX_STR_LEN))>0){
            if (ret == 0) break;
            if (cnt >= start) printf("%s",str);
            cnt++;
            memset(str,0,MAX_STR_LEN);
        }
        close(fd);
    } else perror("no command found");
    return 0;
}
