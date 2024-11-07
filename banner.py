#!/usr/bin/env python3
import sys
import os

argv = sys.argv

name = os.path.basename(argv[0])

if len(argv) not in [2, 3]:
    print("Error: Invalid command")
    print(f"Usage: {name} target [output]")
    exit(1)


# os.path.exists()
with open(argv[1]) as f:
    lines = f.readlines()
    for i, line in enumerate(lines):
        line = line.strip("\n")
        lines[i] = f'"{line}\\n",'


for line in lines:
    print(line)