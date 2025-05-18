#!/usr/bin/env python3

import os
import stat

hook_file_path = "../.git/hooks/pre-commit"

script_content = '''#!/bin/sh
find . -name "*.cpp" -o -name "*.h" | xargs clang-format -i
git add .
'''

# Create hook file and write the script into it.
with open(hook_file_path, 'w') as hook_file:
    hook_file.write(script_content)

# Make the hook file executable
os.chmod(hook_file_path, stat.S_IRWXU)

print(f"Pre-commit hook set up at {hook_file_path}")

