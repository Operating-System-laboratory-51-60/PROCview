import os
import sys
import subprocess

def main():
    print("[AI Runner] Starting build process...")
    
    # Compile the C code
    source_file = "solution.c"
    exe_file = "my_ps_ai"
    
    compile_cmd = ["gcc", source_file, "-o", exe_file]
    
    try:
        subprocess.run(compile_cmd, check=True)
        print(f"[AI Runner] Compilation successful: {exe_file}")
    except subprocess.CalledProcessError:
        print("[Error] Compilation failed.")
        sys.exit(1)
        
    print("[AI Runner] Running my_ps_ai...")
    print("="*60)
    
    # Run the executable
    try:
        subprocess.run([f"./{exe_file}"], check=True)
    except Exception as e:
        print(f"[Error] Execution failed: {e}")
        
    print("="*60)
    print("[AI Runner] Done.")

if _name_ == "_main_":
    main()
