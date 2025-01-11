#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    // ECN 정책 맵 열기
    int map_fd = bpf_obj_get("/sys/fs/bpf/tc/globals/ecn_policy_map");
    if (map_fd < 0) {
        perror("bpf_obj_get");
        return 1;
    }

    // 사용자 입력을 통해 ECN 정책 값을 설정
    __u32 key = 0;  // 맵에서 사용할 키 (단일 엔트리 맵이므로 0 사용)
    __u8 value;     // 사용자로부터 ECN 값을 받을 변수

    // 사용자로부터 ECN 값을 입력받기
    printf("Enter ECN policy value (0, 1, 2, or 3): ");
    int input = scanf("%hhu", &value);  // ECN 값은 2 비트이므로 0-3 범위의 값을 받음

    // 입력값 검증 (0~3 이외의 값 방지)
    if (input != 1 || value > 3) {
        fprintf(stderr, "Invalid ECN policy value. Please enter a value between 0 and 3.\n");
        close(map_fd);
        return 1;
    }

    // BPF 맵에 ECN 정책 업데이트
    if (bpf_map_update_elem(map_fd, &key, &value, BPF_ANY) < 0) {
        perror("bpf_map_update_elem");
        close(map_fd);
        return 1;
    } else {
        // 성공 메시지 출력
        printf("Successfully updated ECN policy to %d in BPF map\n", value);
        close(map_fd);
    }

    return 0;
}

