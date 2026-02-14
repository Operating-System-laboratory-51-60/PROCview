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

// Scans x2.txt to find the PPID and Thread info for a specific PID
int find_elf_info(int target_pid,int *ppid,int *lwp,int *nlwp)
{
    FILE *fp = fopen("x2.txt", "r");
    if(!fp) return 0;

    char buffer[1024];
    // Skip the header line
    fgets(buffer, 1024, fp);

    // ps -eLf format: UID PID PPID LWP C NLWP
    char d_uid[32], d_dummy[256];
    int curr_pid,curr_ppid,curr_lwp,dummy_c,curr_nlwp;

    int found = 0;
    while(fscanf(fp,"%s %d %d %d %d %d",d_uid,&curr_pid,&curr_ppid,&curr_lwp,&dummy_c,&curr_nlwp) == 6)
    {
        
        // Advance to next line
        fgets(buffer,1024,fp);
        if(curr_pid == target_pid)
        {
            *ppid = curr_ppid;
            *lwp = curr_lwp;
            *nlwp = curr_nlwp;
            found = 1;
            break; // Stop after finding match
        }
    }

    fclose(fp);
    return found;
}

