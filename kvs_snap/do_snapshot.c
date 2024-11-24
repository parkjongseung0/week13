#include "kvs.h"
#include <stdio.h>
#include <unistd.h> // for fsync

int do_snapshot(kvs_t* kvs, const char* path) {
    // 스냅샷 저장 로직
    FILE* file = fopen(path, "w");
    if (!file) {
        printf("Failed to open %s for snapshot\n", path);
        return -1;
    }

    node_t* node = kvs->header->next[0];
    while (node) {
        fprintf(file, "%s,%s\n", node->key, node->value);
        //printf("Writing to snapshot: Key=%s, Value=%s\n", node->key, node->value);
        node = node->next[0];
    }

    fflush(file);
    fsync(fileno(file));
    fclose(file);
    return 0;
}

int do_recovery(kvs_t* kvs, const char* path) {
    // 복구 로직
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("No recovery file found at %s\n", path);
        return -1;
    }

    char key[100], value[100];
    while (fscanf(file, "%[^,],%s\n", key, value) != EOF) {
        put(kvs, key, value);
    }

    fclose(file);
    return 0;
}
