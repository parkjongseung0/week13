#include "kvs.h"
#include <stdlib.h>
#include <stdio.h>

int rand_lv() {
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }
    int kvs_mx_level = 0;
    while (rand() < RAND_MAX / 2 && kvs_mx_level < MAX_LEVEL - 1) {
        kvs_mx_level++;
    }
    return kvs_mx_level;
}

kvs_t* open_kvs() {
    printf("[DEBUG] open_kvs called\n");
    fflush(stdout);

    kvs_t* kvs = (kvs_t*)malloc(sizeof(kvs_t));
    if (!kvs) {
        printf("[DEBUG] Allocating kvs Failed\n");
        fflush(stdout);
        return NULL;
    }

    kvs->kvs_mx_level = 0;
    kvs->items = 0;

    kvs->header = (node_t*)malloc(sizeof(node_t));
    if (!kvs->header) {
        printf("[DEBUG] Allocating header failed\n");
        fflush(stdout);
        free(kvs);
        return NULL;
    }

    for (int i = 0; i < MAX_LEVEL; i++) {
        kvs->header->next[i] = NULL;
    }

    printf("[DEBUG] Before calling do_recovery\n");
    fflush(stdout);

    int recovery_status = do_recovery(kvs, "./kvs.img");
    if (recovery_status != 0) {
        printf("[DEBUG] No recovery file found, starting fresh\n");
    } else {
        printf("[DEBUG] do_recovery succeeded with %d items\n", kvs->items);
    }
    fflush(stdout);

    printf("[DEBUG] Open: kvs has %d items\n", kvs->items);
    fflush(stdout);

    return kvs;
}
