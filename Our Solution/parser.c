#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ps_lib.h"

// Reads a single line from ps aux output (x1.txt)
int parse_aux_line(FILE *fp,ProcessInfo *p)
{
    char tty_dummy[32]; 
    // ps aux format: USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
    int result = fscanf(fp,"%s %d %lf %lf %ld %ld %s %s %s %s",p->user,&p->pid,&p->cpu,&p->mem,&p->vsz,&p->rss,p->tty,p->stat,p->start, p->time);

    if(result != 10) return 0; // End of file or empty line

    // Read the rest of the line (COMMAND column can contain spaces)
    if(fgets(p->command,256,fp) != NULL)
    {
        // Remove trailing newline char
        p->command[strcspn(p->command,"\n")] = 0;
    }
    return 1;
}


