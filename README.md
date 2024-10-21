# File Comparison 

## Overview

This C program compares two input text files byte by byte and identifies any differences between them. The differences are written to two separate output files: 
- `differencesFoundInFile1.txt`: Contains bytes from the first file that differ from the second file.
- `differencesFoundInFile2.txt`: Contains bytes from the second file that differ from the first file.
The purpose of this process is to demonstrate the differences in efficiency that come from the two differing steps.

## Features

- Uses system calls (`open`, `close`, `read`, `write`) to handle file operations.
- Compares files in two steps:
  - **Step 1**: Compares the files byte by byte and writes differing bytes from the first file.
  - **Step 2**: Reads both files into dynamically allocated arrays, compares them, and writes differing bytes from the second file.
- Dynamically allocates memory for file contents and differences to optimize RAM usage.
- Measures and reports the time taken for each comparison step.

## How to Use

The program only accepts two text file pathways as command-line arguments.

### Example Usages:

1. **Comparing two files**:
   ```bash
   ./fileComparison.out input1.txt input2.txt

2. **Comparing two files and printing the differences to console**:
   ```bash
   ./fileComparison.out input1.txt input2.txt; cat differencesFoundInText1.txt differencesFoundInFile2.txt;
   
## Compilation

To compile the program, run the Makefile, or alternatively use:
  ```bash
  gcc fileComparison.c -o fileComparison.out
  ```
## License

This project is licensed under the MIT License.
