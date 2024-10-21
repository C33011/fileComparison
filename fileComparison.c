#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


// The purpose of this function is to compare 2 files byte by byte, writing differences to an output
void compareBytes(int file1, int file2, int output1) {

    // Initalizes the char variables to be used as byte indexing for both files
    char byte1, byte2;
    
    // Initalizes the signed integers representing the number of bytes read from each file
    ssize_t bytesRead1, bytesRead2;

    // While loop to compare the bytes from each file, writing the differing byte from file1
    while ((bytesRead1 = read(file1, &byte1, 1)) > 0 &&
           (bytesRead2 = read(file2, &byte2, 1)) > 0) {
        if (byte1 != byte2) {
            write(output1, &byte1, 1); 
        }
    }

    // If the bytes in file 1 are greater, write them to output
    if (bytesRead1 > 0) {
        write(output1, &byte1, 1);
        while ((bytesRead1 = read(file1, &byte1, 1)) > 0) {
            write(output1, &byte1, 1);
        }
    }

    // Skip remaining bytes in file2
    while ((bytesRead2 = read(file2, &byte2, 1)) > 0) {
        // Do nothing, effectively skipping extra bytes in file2
    }
}

// The purpose of this function is to compare 2 files by assigning their content to arrays, comparing them, writing differences to another array
// that will be written to the second output file
void compareContents(int file1, int file2, int output2) {
    // Get the sizes of both files using lseek
    off_t size1 = lseek(file1, 0, SEEK_END);
    off_t size2 = lseek(file2, 0, SEEK_END);

    // Reset file offsets to beginning
    lseek(file1, 0, SEEK_SET);
    lseek(file2, 0, SEEK_SET);

    // Dynamically allocate memory for both file contents
    char *file1Contents = (char *)malloc(size1);
    char *file2Contents = (char *)malloc(size2);

    // Read both files into their respective arrays, making sure there isn't an error or mismatch in bytes to data size
    if (read(file1, file1Contents, size1) != size1) {
        perror("Error reading the first file");
        free(file1Contents);
        free(file2Contents);
        exit(EXIT_FAILURE);
    }

    if (read(file2, file2Contents, size2) != size2) {
        perror("Error reading the second file");
        free(file1Contents);
        free(file2Contents);
        exit(EXIT_FAILURE);
    }

    // Dynamically allocate memory for the differences array
    char *differences = (char *)malloc(size2);

    // Compare files and collect differing bytes from file2
    off_t diffIndex = 0;
    for (off_t i = 0; i < size1 && i < size2; i++) {
        if (file1Contents[i] != file2Contents[i]) {
            differences[diffIndex++] = file2Contents[i];  
        }
    }

    // Handle remaining bytes if file2 is longer than file1
    if (size2 > size1) {
        for (off_t i = size1; i < size2; i++) {
            differences[diffIndex++] = file2Contents[i];  
        }
    }

    // Write the differences array into the output file
    if (write(output2, differences, diffIndex) != diffIndex) {
        perror("There was an error writing to a file");
    }

    // Free allocated memory
    free(file1Contents);
    free(file2Contents);
    free(differences);
}

/*
  MAIN STARTS HERE!
  The purpose of this project is to take 2 file inputs and follow 2 steps:
  Step 1. Compare both files by reading and comparing bytes individually, writing differences in file 1 to an output
  Step 2. Compare both files by using array, assigning differences in file 2 to a new array and writing the array to an output
  After doing this, the program returns the amount of time (in ms) taken to compare the files in the different methods.
  After all of the above is done, the program frees all allocated data and closes files.
*/
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: proj3.out <file1> <file2>\n");
        exit(EXIT_FAILURE);
    }

    struct timeval start, end;

    // Open files for reading and output files for writing
    int file1 = open(argv[1], O_RDONLY);
    int file2 = open(argv[2], O_RDONLY);
    
    // For the output, opens specific files, opening with read and write only, creating if they don't exist, truncating if they do, and giving them 0644 permissions
    int output1 = open("differencesFoundInFile1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int output2 = open("differencesFoundInFile2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    // Check if files opened successfully
    if (file1 < 0 || file2 < 0 || output1 < 0 || output2 < 0) {
        perror("There was an error reading a file");
        exit(EXIT_FAILURE);
    }

    // Call the function to compare bytes
    gettimeofday(&start, NULL);
    compareBytes(file1, file2, output1);
    gettimeofday(&end, NULL);

    // Adjust for possible negative microseconds
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    if (microseconds < 0) {
        seconds -= 1;
        microseconds += 1000000;  // Add one second (in microseconds) to microsecond part
    }
    double elapsed = seconds * 1000.0 + microseconds * 1e-3;
    printf("Time for comparison of bytes: %.6f milliseconds.\n", elapsed);

    // Start timing for comparing contents
    gettimeofday(&start, NULL);
    compareContents(file1, file2, output2);
    gettimeofday(&end, NULL);

    // Adjust for possible negative microseconds
    seconds = end.tv_sec - start.tv_sec;
    microseconds = end.tv_usec - start.tv_usec;
    if (microseconds < 0) {
        seconds -= 1;
        microseconds += 1000000;
    }
    elapsed = seconds * 1000.0 + microseconds * 1e-3;
    printf("Time for comparison of contents: %.6f milliseconds.\n", elapsed);

    // Close all files
    close(file1);
    close(file2);
    close(output1);
    close(output2);

    return 0;
}
