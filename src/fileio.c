//
//  fileio.c
//  graphixs
//
//  Created by Lars Höll on 24.08.25.
//

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>

#include "../include/fileio.h"


char* read_file(const char* path) {
    char* output = NULL;
    FILE* file;

    errno = 0;

    // open file
    if (!(file = fopen(path, "r"))) {
        // errno is set
        perror("Error: Failed to open file.\n");
        return NULL;
    }

    struct stat statbuf;

    // load the file stats into statbuf
    if (fstat(fileno(file) , &statbuf)) {
        // errno is set
        perror("Error: Failed to load file statistics.\n");
        goto cleanup;
    }

    // check if the file is valid
    if (!S_ISREG(statbuf.st_mode) || statbuf.st_size <= 0) {
        // errno may not be set
        fprintf(stderr, "Error: File is not regular or too short.\n");
        goto cleanup;
    }

    // allocate memory for read string
    // size + 1 because of 0 terminator
    if (!(output = malloc(statbuf.st_size + 1))) {
        // errno is set to ENOMEM
        perror("Error: Failed to allocate enough memory while reading from file.\n");
        goto cleanup;
    }

    if (fread(output, 1, statbuf.st_size, file) != (size_t) statbuf.st_size) {
        // I dont think errno is set so use fprintf
        fprintf(stderr, "Error: Failed to read file.\n");
        free (output);
        output = NULL;
        goto cleanup;
    }

    // set \0 terminator
    output[statbuf.st_size] = '\0';

cleanup:
    // close file if it was opened
    if (file) fclose(file);

    return output;
}

void write_file(const char* path, const char* string) {
    FILE* file;

    errno = 0;

    // open file
    if (!(file = fopen(path, "w"))) {
        perror("Error: Failed to open file.\n");
        return;
    }

    // calculate string length so to not call strlen(string) twice
    const size_t stringlen = strlen(string);

    // write to file
    if (fwrite(string, 1, stringlen, file) != stringlen) {
        perror("Error: Failed to write to file correctly.\n");
    }

    // close file
    fclose(file);
}
