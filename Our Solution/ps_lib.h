#ifndef PS_LIB_H
#define PS_LIB_H

#include <stdio.h>
#include <stdlib.h>

// Structure to hold merged information
typedef struct
{
    // From ps aux
    char user[32];
    int pid;
    double cpu;
    double mem;
    long vsz;
    long rss;
    char tty[16];
    char stat[8];
    char start[16];
    char time[16];
    char command[256];

    // From ps -eLf
    int ppid;
    int lwp;   // Light Weight Process (Thread ID)
    int nlwp;  // Number of threads
}ProcessInfo;

// Function Prototypes
void generate_raw_data();
void cleanup_temp_files();
int parse_aux_line(FILE *fp, ProcessInfo *p);
int find_elf_info(int target_pid,int *ppid,int *lwp,int *nlwp);

#endif
