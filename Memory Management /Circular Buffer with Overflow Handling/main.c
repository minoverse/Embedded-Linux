// main.c - minimal demo for the circular buffer
#include <stdio.h>
#include <stdint.h>
#include "circular_buffer.h"

static void print_bytes(const char* tag, const uint8_t* b, int n) {
    printf("%s (%d):", tag, n);
    for (int i = 0; i < n; ++i) printf(" %u", b[i]);
    printf("\n");
}

int main(void) {
    CircularBuffer* cb = cb_create(4);
    if (!cb) {
        printf("Failed to create buffer\n");
        return 1;
    }

    printf("Initial: avail=%zu, empty=%d, full=%d\n",
           cb_available(cb), cb_is_empty(cb), cb_is_full(cb));

    uint8_t a[] = {1,2,3,4};
    int w = cb_write(cb, a, 4);
    printf("After write a: wrote=%d, avail=%zu, empty=%d, full=%d\n",
           w, cb_available(cb), cb_is_empty(cb), cb_is_full(cb));

    uint8_t out[8] = {0};
    int r = cb_read(cb, out, 4);
    print_bytes("Read a", out, r);

    // Overflow demo
    uint8_t b[] = {10,11,12,13,14};
    w = cb_write(cb, b, 5); // capacity 4 -> should overflow once
    printf("After write b: wrote=%d, overflow(prev)=%d\n", w, cb_clear_overflow(cb)); // expect 1
    r = cb_read(cb, out, 4); // expect 11,12,13,14
    print_bytes("Read after overflow", out, r);

    // Partial read / wrap demo
    uint8_t c[] = {21,22,23};
    cb_write(cb, c, 3);
    r = cb_read(cb, out, 2); // consume 2
    print_bytes("Partial read", out, r);
    printf("Now: avail=%zu, empty=%d, full=%d\n",
           cb_available(cb), cb_is_empty(cb), cb_is_full(cb));

    cb_destroy(cb);
    return 0;
}
