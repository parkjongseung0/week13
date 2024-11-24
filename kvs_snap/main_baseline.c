//baseline
#include <stdio.h>
#include <string.h>
#include "kvs.h"

#define BUFFER_SIZE 4096 // 버퍼 크기 증가

int main() {
    // 1. KVS 초기화
    kvs_t* kvs = open_kvs();
    if (!kvs) {
        fprintf(stderr, "Error: Failed to initialize KVS\n");
        return -1;
    }

    // 2. 데이터 파일 열기
    FILE* queryFile = fopen("workbench.trc", "r");
    if (!queryFile) {
        fprintf(stderr, "Error: Failed to open workbench.trc\n");
        close_kvs(kvs);
        return -1;
    }

    char buffer[BUFFER_SIZE];
    char op[10], key_q[100], value_q[100];

    // 3. 데이터 삽입
    while (fgets(buffer, BUFFER_SIZE, queryFile)) {
        // 입력 데이터 파싱
        if (sscanf(buffer, "%9[^,],%99[^,],%99s", op, key_q, value_q) == 3) {
            if (strcmp(op, "get") == 0) {
                get(kvs, key_q); // 결과 확인 없음
            } else if (strcmp(op, "set") == 0) {
                put(kvs, key_q, value_q); // 데이터 삽입
            }
        }
    }
    fclose(queryFile); // 데이터 파일 닫기
    printf("Data insertion completed\n");

    // 4. 스냅샷 생성
    if (do_snapshot(kvs, "./kvs.img") == 0) {
        printf("Snapshot saved successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to save snapshot\n");
        close_kvs(kvs);
        return -1;
    }

    // 5. KVS 종료
    close_kvs(kvs);
    //printf("KVS closed successfully after snapshot\n");

    // 6. 복구 시도
    kvs = open_kvs(); // open_kvs 내부에서 do_recovery 호출
    if (!kvs) {
        fprintf(stderr, "Error: Failed to reopen KVS\n");
        return -1;
    }

    //printf("KVS reopened successfully\n");

    // 복구 성공 여부 확인
    if (do_recovery(kvs, "./kvs.img") == 0) {
        printf("KVS recovery successful\n");
    } else {
        printf("KVS recovery failed\n");
    }

    // KVS 종료
    close_kvs(kvs);
    //printf("KVS closed successfully after recovery\n");

    return 0;
}
