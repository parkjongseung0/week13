//custom

#include "kvs.h"
#include <fcntl.h>  // for open, O_RDONLY
#include <unistd.h> // for close, write
#include <string.h> // for strcmp
#include <stdio.h>  // for printf

#define BUFFER_SIZE 409600

int main() {
    // 1. KVS 초기화
    kvs_t* kvs = open_kvs();
    if (!kvs) {
        printf("Failed to initialize KVS\n");
        return -1;
    }

    // 2. 데이터 파일 열기
    int fd = open("workbench.trc", O_RDONLY);
    if (fd < 0) {
        printf("Failed to open workbench.trc\n");
        close_kvs(kvs);
        return -1;
    }

    char buffer[BUFFER_SIZE];
    char op[10], key[100], value[100];
    ssize_t bytes_read;
    size_t start = 0;

    // 3. 파일에서 데이터를 읽고 KVS에 삽입
    while ((bytes_read = read(fd, buffer + start, BUFFER_SIZE - start - 1)) > 0) {
        buffer[bytes_read + start] = '\0'; // NULL 종료
        char* line = buffer;
        char* newline;

        while ((newline = strchr(line, '\n'))) {
            *newline = '\0'; // 줄 끝 처리
            if (sscanf(line, "%9[^,],%99[^,],%99s", op, key, value) == 3) {
                if (strcmp(op, "set") == 0) {
                    put(kvs, key, value);
                }
            }
            line = newline + 1; // 다음 줄로 이동
        }

        start = strlen(line);
        memmove(buffer, line, start);
    }

    close(fd); // 파일 닫기

    // 4. 스냅샷 생성
    if (do_snapshot(kvs, "./kvs.img") == 0) {
        printf("Snapshot saved successfully\n");
    } else {
        printf("Failed to save snapshot\n");
        close_kvs(kvs);
        return -1;
    }

    // 5. KVS 종료
    close_kvs(kvs);
    printf("KVS closed successfully\n");

    // 6. KVS 재시작 및 복구
    kvs = open_kvs(); // open_kvs 내부에서 do_recovery 호출
    if (!kvs) {
        printf("Failed to reopen KVS\n");
        return -1;
    }

    //printf("KVS recovered successfully\n");

    // 7. KVS 종료
    close_kvs(kvs);
    //printf("KVS closed after recovery\n");

    return 0;
}
