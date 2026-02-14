#include <stdio.h>
#include <stdlib.h>
#include "ps_lib.h"

// Executes the linux ps commands and directs output to temp files
void generate_raw_data()
{
    // 1. Run ps aux and save to x1.txt
    // 'w' flag ensures output isn't truncated
    system("ps auxw > x1.txt");

    // 2. Run ps -eLf and save to x2.txt
    system("ps -eLf > x2.txt");
}

// Removes the temporary files after processing
void cleanup_temp_files()
{
    system("rm x1.txt x2.txt");
}
