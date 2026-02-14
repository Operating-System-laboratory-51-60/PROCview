#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void clear_screen()
{
    printf("\033[H\033[J");
}

int main()
{
    int choice;
    
    while(1)
    {
        clear_screen();
        printf("==========================================\n");
        printf("    CUSTOM COMMAND BUILDER: my_ps         \n");
        printf("==========================================\n");
        printf("1. Compile 'my_ps' (Build Project)\n");
        printf("2. Run 'my_ps' (Generate Merged Output)\n");
        printf("3. View 'merged.txt' (Full Output)\n");
        printf("4. Clean & Exit\n");
        printf("==========================================\n");
        printf("Select Option: ");
        
        if(scanf("%d", &choice) != 1)
        {
            while(getchar() != '\n'); 
            continue;
        }

        if(choice == 1)
        {
            printf("\n[Build] Compiling sources...\n");
            // Compile all parts together: main, parser, and file handler
            int ret = system("gcc my_ps.c parser.c file_handler.c -o my_ps");
            
            if(ret == 0)
            printf("[Success] Executable 'my_ps' created.\n");
            else
            printf("[Error] Compilation failed. Check source files.\n");
            
            sleep(2);
        }
        else if(choice == 2)
        {
            // Check if executable exists
            if(access("my_ps", F_OK) != 0)
            printf("\n[Error] 'my_ps' not found. Please Compile first (Option 1).\n");
            else
            {
                printf("\n[Exec] Running custom command...\n");
                system("./my_ps");
            }
            printf("\nPress Enter to continue...");
            getchar();
            getchar();
        }
        else if(choice == 3)
        {
            printf("\n[View] Opening merged.txt...\n");
            system("cat merged.txt | head -n 20"); 
            printf("\n(Showing first 20 lines. Press Enter to return)\n");
            getchar();
            getchar();
        }
        else if(choice == 4)
        {
            printf("\n[Clean] Removing temporary files...\n");
            exit(0);
        }
    }
    return 0;
}
