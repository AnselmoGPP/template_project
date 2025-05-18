#!/usr/bin/env python3

import os
import subprocess

def run_command(command):
    try:
        subprocess.run(command, shell=True)
    except subprocess.CalledProcessError as e:
        print(f"Error: Command failed -> {e.returncode}")
        exit(1)

def main():
    message = input("<commit description>")
    os.chdir("..")
    run_command(f'git status')
    run_command(f'git add --all')
    run_command(f'git commit -m"{message}"')
    run_command(f'git push')
    run_command(f'git status')
    input()

# Ensure main only runs when script is executed directly, not when imported as module.
if __name__ == "__main__":
    main()

