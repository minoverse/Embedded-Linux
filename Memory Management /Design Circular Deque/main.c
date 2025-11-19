#include <stdio.h>
#include "deque.h"

static void print_deque_state(CircularDeque* dq, const char* label) {
    printf("=== %s ===\n", label);
    printf("size = %d, capacity = %d, empty = %s, full = %s\n",
           deque_size(dq),
           deque_capacity(dq),
           deque_is_empty(dq) ? "true" : "false",
           deque_is_full(dq) ? "true" : "false");

    int front_val, back_val;
    if (deque_front(dq, &front_val)) {
        printf("front = %d\n", front_val);
    } else {
        printf("front = (none)\n");
    }

    if (deque_back(dq, &back_val)) {
        printf("back  = %d\n", back_val);
    } else {
        printf("back  = (none)\n");
    }
    printf("\n");
}

int main(void) {
    CircularDeque* dq = deque_create(3);
    if (!dq) {
        fprintf(stderr, "Failed to create deque\n");
        return 1;
    }

    print_deque_state(dq, "Initial");

    printf("push_back(10)\n");
    deque_push_back(dq, 10);
    print_deque_state(dq, "After push_back(10)");

    printf("push_back(20)\n");
    deque_push_back(dq, 20);
    print_deque_state(dq, "After push_back(20)");

    printf("push_front(5)\n");
    deque_push_front(dq, 5);
    print_deque_state(dq, "After push_front(5)");

    printf("Try push_back(99) on full deque -> %s\n",
           deque_push_back(dq, 99) ? "success" : "failed (full)");
    print_deque_state(dq, "After push_back(99)");

    int value;

    printf("pop_front -> ");
    if (deque_pop_front(dq, &value)) {
        printf("%d\n", value);
    } else {
        printf("failed (empty)\n");
    }
    print_deque_state(dq, "After pop_front");

    printf("pop_back -> ");
    if (deque_pop_back(dq, &value)) {
        printf("%d\n", value);
    } else {
        printf("failed (empty)\n");
    }
    print_deque_state(dq, "After pop_back");

    printf("push_front(42)\n");
    deque_push_front(dq, 42);
    print_deque_state(dq, "After push_front(42)");

    printf("Clearing all elements with pop_front...\n");
    while (!deque_is_empty(dq)) {
        if (deque_pop_front(dq, &value)) {
            printf("  popped %d\n", value);
        }
    }
    print_deque_state(dq, "After clearing");

    deque_destroy(dq);
    return 0;
}
