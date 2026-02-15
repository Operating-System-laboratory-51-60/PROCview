# Custom Process Viewer (`my_ps`)

This directory contains the custom implementation of the `my_ps` command, which merges output from `ps aux` and `ps -eLf` to provide a comprehensive view of system processes including thread details.

## File Descriptions

The solution is modularized into several files for better organization:

| File Name | Description |
|-----------|-------------|
| **`my_ps.c`** | The main entry point for the `my_ps` command. It orchestrates the process of reading temporary files, merging data, and writing to `merged.txt`. |
| **`parser.c`** | Contains logic to parse lines from `ps aux` and `ps -eLf` outputs. It handles extraction of specific fields like PID, CPU, MEM, LWP, etc. |
| **`file_handler.c`** | Manages system interactions. It executes the `ps` commands to generate raw data files (`x1.txt`, `x2.txt`) and handles cleanup. |
| **`interface.c`** | A user-friendly CLI wrapper. It provides a menu to compile the project, run the command, view the output, and clean up files. |
| **`ps_lib.h`** | Header file containing the `ProcessInfo` structure definition and function prototypes shared across the files. |

## How to Compile and Run

You can run this project using the provided interactive interface or by compiling the manual way.

### Option 1: Using the Interactive Interface (Recommended)

The interface automates the build and run process.

1.  **Compile the Interface:**
    ```bash
    gcc interface.c -o menu
    ```

2.  **Run the Interface:**
    ```bash
    ./menu
    ```

3.  **Use the Menu:**
    *   Select **Option 1** to compile the `my_ps` tool.
    *   Select **Option 2** to run the tool (generates `merged.txt`).
    *   Select **Option 3** to view the results.
    *   Select **Option 4** to clean up and exit.

### Option 2: Manual Compilation

If you prefer to compile the command yourself without the menu:

1.  **Compile the source files:**
    ```bash
    gcc my_ps.c parser.c file_handler.c -o my_ps
    ```

2.  **Run the executable:**
    ```bash
    ./my_ps
    ```

    This will:
    *   Generate temporary files `x1.txt` and `x2.txt`.
    *   Read and merge the data.
    *   Create `merged.txt`.
    *   Display the first 10 lines of the output.
    *   Clean up temporary files.

## Output

The final output is saved to **`merged.txt`** and contains the following columns:

*   `USER`: User owning the process
*   `PID`: Process ID
*   `PPID`: Parent Process ID (from `ps -eLf`)
*   `LWP`: Light Weight Process / Thread ID (from `ps -eLf`)
*   `NLWP`: Number of threads (from `ps -eLf`)
*   `%CPU`: CPU usage
*   `%MEM`: Memory usage
*   `VSZ`: Virtual Memory Size
*   `RSS`: Resident Set Size
*   `TTY`: Terminal type
*   `STAT`: Process Status
*   `START`: Start time
*   `TIME`: CPU Time
*   `COMMAND`: Command name/path
