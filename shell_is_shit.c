#include <stdio.h>
#include <stdlib.h>

#define NULL ((void *)0)
#define MAX_COMMAND_LEN 256

int main(int argc, char *argv[])
{
        FILE *pipe_fp, *in_file;
        char read_buf[MAX_COMMAND_LEN];

        if( argc != 2) {
                fprintf(stderr, "USAGE:  ./shell_is_garbage [filename]\n");
                exit(1);
        }

        /* Open up input file */
        if (( in_file = fopen(argv[1], "rt")) == NULL)
        {perror("fopen");exit(1);}

        /* Processing loop */
        do {
                fgets(read_buf, MAX_COMMAND_LEN, in_file);
                if(feof(in_file)) break;

                /* Create one way pipe line with call to popen() and pclose() after execution */
                if (( pipe_fp = popen(read_buf, "w")) == NULL)
                {perror("popen");exit(1);}
                pclose(pipe_fp);
        } while(!feof(in_file));
        fclose(in_file);

        return(0);
}
