#include "kvs.h"

int put(kvs_t* kvs, const char* key, const char* value) {
    node_t* prev_node[MAX_LEVEL];
    node_t* node = kvs->header;

    // 키 위치 탐색
    for (int i = kvs->kvs_mx_level; i >= 0; i--) {
        while (node->next[i] && strcmp(node->next[i]->key, key) < 0) {
            node = node->next[i];
        }
        prev_node[i] = node;
    }
    node = node->next[0];

    // 이미 존재하는 키 처리
    if (node && strcmp(node->key, key) == 0) {
        if (strcmp(node->value, value) != 0) { // 값이 달라야 업데이트
            free(node->value);
            node->value = strdup(value);
        }
        return 0;
    }

    // 새로운 노드 삽입
    int level = rand_lv();
    if (level > kvs->kvs_mx_level) {
        for (int i = kvs->kvs_mx_level + 1; i <= level; i++) {
            prev_node[i] = kvs->header;
        }
        kvs->kvs_mx_level = level;
    }

    node = (node_t*)malloc(sizeof(node_t));
    strcpy(node->key, key);
    node->value = strdup(value);

    for (int i = 0; i <= level; i++) {
        node->next[i] = prev_node[i]->next[i];
        prev_node[i]->next[i] = node;
    }
    kvs->items++;
    return 0;
}
