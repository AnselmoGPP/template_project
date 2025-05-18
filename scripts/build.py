#!/usr/bin/env python3

import os
import subprocess
import platform

# In Linux: Project is created with Unix Makefiles and compiled with make.
# In Windows: Project is created with VS2022 but not compiled (compile it with VS IDE). 

if platform.system() == "Linux":
    CMAKE_GENERATOR = "Unix Makefiles"
    MAKE_COMMAND = "make"
elif platform.system() == "Windows":
    CMAKE_GENERATOR = "Visual Studio 17 2022"
    MAKE_COMMAND = "cmake --build . --config Release"
else:
    print("\nOS unknown")
    exit(1)

def run_command(command):
    try:
        subprocess.run(command, shell=True)
    except subprocess.CalledProcessError as e:
        print(f"Error: Command failed -> {e.returncode}")
        exit(1)

def main():
    if not os.path.exists("../_BUILD"):
        os.makedirs("../_BUILD")
    
    os.chdir("../_BUILD")
    run_command(f'cmake ../projects -G "{CMAKE_GENERATOR}" -A x64')
    if platform.system() == "Linux":
        run_command(MAKE_COMMAND)   #alternative: run_command("make -j$(nproc)")

    print("\nBuild completed")
    input()

# Ensure main only runs when script is executed directly, not when imported as module.
if __name__ == "__main__":
    main()

