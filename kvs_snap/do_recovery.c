#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kvs.h"

int do_recovery(kvs_t* kvs, const char* path) {
    printf("[DEBUG] do_recovery called with path: %s\n", path);
    fflush(stdout);

    FILE* file = fopen(path, "r");
    if (!file) {
        printf("[DEBUG] fopen failed: No recovery file found at %s\n", path);
        fflush(stdout);
        return -1;
    }
    printf("[DEBUG] fopen succeeded: Recovery file opened at %s\n", path);
    fflush(stdout);

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size <= 0) {
        printf("[DEBUG] File is empty or invalid: size = %ld\n", file_size);
        fclose(file);
        fflush(stdout);
        return -1;
    }
    printf("[DEBUG] File size: %ld bytes\n", file_size);
    fflush(stdout);

    char* buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        printf("[DEBUG] Memory allocation failed\n");
        fclose(file);
        fflush(stdout);
        return -1;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read != file_size) {
        printf("[DEBUG] fread mismatch: expected %ld, got %ld\n", file_size, bytes_read);
        free(buffer);
        fclose(file);
        fflush(stdout);
        return -1;
    }
    buffer[file_size] = '\0';
    printf("[DEBUG] fread successful: %ld bytes read\n", bytes_read);
    fflush(stdout);

    char* line = strtok(buffer, "\n");
    int recovered_count = 0;

    while (line) {
        printf("[DEBUG] Processing line: %s\n", line);
        fflush(stdout);

        char key[100], value[100];
        int sscanf_result = sscanf(line, "%99[^,],%99s", key, value);

        if (sscanf_result == 2) {
            printf("[DEBUG] Parsed key=%s, value=%s\n", key, value);
            fflush(stdout);

            if (put(kvs, key, value) == 0) {
                printf("[DEBUG] Successfully inserted key=%s\n", key);
                recovered_count++;
            } else {
                printf("[DEBUG] Failed to insert key=%s\n", key);
            }
        } else {
            printf("[DEBUG] Failed to parse line: %s\n", line);
        }
        fflush(stdout);

        line = strtok(NULL, "\n");
    }

    free(buffer);
    fclose(file);

    if (recovered_count > 0) {
        printf("[DEBUG] KVS recovered successfully with %d items from %s\n", recovered_count, path);
        fflush(stdout);
        return 0;
    } else {
        printf("[DEBUG] Recovery file was empty or invalid\n");
        fflush(stdout);
        return -1;
    }
}
