#include <stdio.h>
#include <stdlib.h>
#include "ps_lib.h"

int main()
{
    FILE *aux_file, *merged_file;
    ProcessInfo p;
    
    // Snapshot the system processes
    generate_raw_data();

    // Open our working files
    aux_file = fopen("x1.txt", "r");
    merged_file = fopen("merged.txt", "w");

    if(!aux_file || !merged_file)
    {
        perror("Error opening temporary files");
        return 1;
    }

    // Write a clean header to merged.txt
    fprintf(merged_file,"%-10s %-6s %-6s %-6s %-6s %-5s %-5s %-6s %-6s %-6s %-6s %-6s %-6s %s\n",
            "USER","PID","PPID","LWP","NLWP","%CPU","%MEM","VSZ","RSS","TTY","STAT","START","TIME","COMMAND");
    
    printf("Processing process list... Please wait.\n");

    // Skip the header row in x1.txt
    char buffer[1024];
    fgets(buffer,1024,aux_file);

    // Iterate through every process
    while(parse_aux_line(aux_file,&p))
    {
        // Default values if not found in -eLf
        p.ppid = -1;
        p.lwp = -1;
        p.nlwp = -1;

        // Fetch details from the second file
        find_elf_info(p.pid,&p.ppid,&p.lwp,&p.nlwp);

        // Write the combined row
        fprintf(merged_file,"%-10s %-6d %-6d %-6d %-6d %-5.1f %-5.1f %-6ld %-6ld %-6s %-6s %-6s %-6s %s\n",
                p.user,p.pid,p.ppid,p.lwp,p.nlwp,p.cpu,p.mem,p.vsz,p.rss,p.tty,p.stat,p.start,p.time,p.command);
    }


    // Cleanup
    fclose(aux_file);
    fclose(merged_file);
    cleanup_temp_files();

    printf("Success! Merged data written to 'merged.txt'.\n");
    printf("Displaying first 10 lines:\n");
    printf("----------------------------------------\n");
    system("head -n 10 merged.txt");

    return 0;
}