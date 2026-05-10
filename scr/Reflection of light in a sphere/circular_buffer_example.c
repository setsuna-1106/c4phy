#include <stdio.h>

#define BUF_SIZE 8

typedef struct {
    int data[BUF_SIZE];
    int head;   // 写入位置
    int count;  // 当前元素数量
} CircularBuffer;

void cb_init(CircularBuffer *cb) {
    cb->head = 0;
    cb->count = 0;
}

void cb_push(CircularBuffer *cb, int value) {
    cb->data[cb->head] = value;
    cb->head = (cb->head + 1) % BUF_SIZE;
    if (cb->count < BUF_SIZE) cb->count++;
}

// 按插入顺序遍历：最旧 → 最新
// index 0 = 最旧, index count-1 = 最新
int cb_get(const CircularBuffer *cb, int index) {
    int oldest = (cb->head - cb->count + BUF_SIZE) % BUF_SIZE;
    return cb->data[(oldest + index) % BUF_SIZE];
}

int main() {
    CircularBuffer cb;
    cb_init(&cb);

    for (int i = 1; i <= 12; i++) {
        cb_push(&cb, i);
        printf("push %2d → [", i);
        for (int j = 0; j < cb.count; j++) {
            printf("%d%s", cb_get(&cb, j), j < cb.count - 1 ? ", " : "");
        }
        printf("]\n");
    }

    return 0;
}
