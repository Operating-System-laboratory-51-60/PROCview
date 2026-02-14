/*
 * Description:
 * This program merges the output of 'ps aux' and 'ps -eLf'.
 * It extracts USER, PID, CPU, MEM, VSZ, RSS, TTY, STAT, START, TIME, COMMAND from 'ps aux'.
 * It extracts PPID, LWP (Thread ID), and NLWP (Thread Count) from 'ps -eLf'.
 * The merged result is saved to 'merged.txt' and displayed.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CMD_LEN 256
#define MAX_LINE_LEN 1024

// Structure to hold merged process data
typedef struct {
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
    char command[MAX_CMD_LEN];
    
    // Extended info
    int ppid;
    int lwp;
    int nlwp;
} ProcessInfo;

// Function to find extended info in ps -eLf output
// Returns 1 if found, 0 otherwise
int get_elf_info(int target_pid, int *ppid, int *lwp, int *nlwp) {
    FILE *fp = fopen("temp_elf.txt", "r");
    if (!fp) return 0;

    char line[MAX_LINE_LEN];
    // Skip header
    fgets(line, MAX_LINE_LEN, fp);

    char uid[32];
    int curr_pid, curr_ppid, curr_lwp, dummy_c, curr_nlwp;

    while (fgets(line, MAX_LINE_LEN, fp)) {
        // Parse: UID PID PPID LWP C NLWP ...
        if (sscanf(line, "%s %d %d %d %d %d", 
            uid, &curr_pid, &curr_ppid, &curr_lwp, &dummy_c, &curr_nlwp) == 6) {
            
            if (curr_pid == target_pid) {
                *ppid = curr_ppid;
                *lwp = curr_lwp;
                *nlwp = curr_nlwp;
                fclose(fp);
                return 1;
            }
        }
    }

    fclose(fp);
    return 0;
}

int main() {
    printf("Generating process snapshots...\n");
    
    // Execute system commands to dump process lists
    system("ps auxww > temp_aux.txt");
    system("ps -eLf > temp_elf.txt");

    FILE *aux_fp = fopen("temp_aux.txt", "r");
    FILE *out_fp = fopen("merged.txt", "w");

    if (!aux_fp || !out_fp) {
        perror("File open error");
        return 1;
    }

    // Write Header
    fprintf(out_fp, "%-10s %-6s %-6s %-6s %-6s %-5s %-5s %-7s %-7s %-8s %-6s %-6s %-6s %s\n",
        "USER", "PID", "PPID", "LWP", "NLWP", "%CPU", "%MEM", "VSZ", "RSS", "TTY", "STAT", "START", "TIME", "COMMAND");

    char line[MAX_LINE_LEN];
    // Skip aux header
    fgets(line, MAX_LINE_LEN, aux_fp);

    ProcessInfo p;
    int count = 0;

    printf("Merging data...\n");

    while (fgets(line, MAX_LINE_LEN, aux_fp)) {
        // Parse ps aux line
        // Format: USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
        // We use sscanf to parse standard fields and find the offset for COMMAND
        
        int offset = 0;
        int parsed = sscanf(line, "%s %d %lf %lf %ld %ld %s %s %s %s %n",
            p.user, &p.pid, &p.cpu, &p.mem, &p.vsz, &p.rss, 
            p.tty, p.stat, p.start, p.time, &offset);

        if (parsed < 10) continue;

        // Copy rest of line as command
        strncpy(p.command, line + offset, MAX_CMD_LEN);
        p.command[strcspn(p.command, "\n")] = 0; // Remove newline

        // Default extended values
        p.ppid = -1;
        p.lwp = -1;
        p.nlwp = 0;

        // Fetch info from -eLf file
        get_elf_info(p.pid, &p.ppid, &p.lwp, &p.nlwp);

        // Write to output file
        fprintf(out_fp, "%-10s %-6d %-6d %-6d %-6d %-5.1f %-5.1f %-7ld %-7ld %-8s %-6s %-6s %-6s %s\n",
            p.user, p.pid, p.ppid, p.lwp, p.nlwp, 
            p.cpu, p.mem, p.vsz, p.rss, p.tty, p.stat, p.start, p.time, p.command);
        
        count++;
    }

    fclose(aux_fp);
    fclose(out_fp);

    // Clean up temporary files
    system("rm temp_aux.txt temp_elf.txt");

    printf("Successfully merged %d processes.\n", count);
    printf("Output saved to 'merged.txt'.\n");
    printf("--------------------------------------------------\n");
    
    // Display first few lines
    system("head -n 10 merged.txt");

    return 0;
}
