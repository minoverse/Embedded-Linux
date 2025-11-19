#include "deque.h"
#include <stdlib.h>

struct CircularDeque {
    int *data;
    int head;      // index of front element
    int tail;      // index of back element
    int size;      // current number of elements
    int capacity;  // maximum number of elements
};

CircularDeque* deque_create(int capacity) {
    if (capacity <= 0) {
        return NULL;
    }

    CircularDeque* dq = (CircularDeque*)malloc(sizeof(CircularDeque));
    if (!dq) {
        return NULL;
    }

    dq->data = (int*)malloc(sizeof(int) * capacity);
    if (!dq->data) {
        free(dq);
        return NULL;
    }

    dq->capacity = capacity;
    dq->head = 0;
    dq->tail = 0;
    dq->size = 0;

    return dq;
}

void deque_destroy(CircularDeque* dq) {
    if (!dq) return;
    free(dq->data);
    free(dq);
}

int deque_size(const CircularDeque* dq) {
    if (!dq) return 0;
    return dq->size;
}

int deque_capacity(const CircularDeque* dq) {
    if (!dq) return 0;
    return dq->capacity;
}

bool deque_is_empty(const CircularDeque* dq) {
    if (!dq) return true;
    return dq->size == 0;
}

bool deque_is_full(const CircularDeque* dq) {
    if (!dq) return false;
    return dq->size == dq->capacity;
}

bool deque_push_front(CircularDeque* dq, int value) {
    if (!dq) return false;
    if (dq->size == dq->capacity) {
        return false; // full
    }

    if (dq->size == 0) {
        // first element
        dq->head = dq->tail = 0;
    } else {
        // move head backward circularly
        dq->head = (dq->head - 1 + dq->capacity) % dq->capacity;
    }

    dq->data[dq->head] = value;
    dq->size++;
    return true;
}

bool deque_push_back(CircularDeque* dq, int value) {
    if (!dq) return false;
    if (dq->size == dq->capacity) {
        return false; // full
    }

    if (dq->size == 0) {
        // first element
        dq->head = dq->tail = 0;
    } else {
        // move tail forward circularly
        dq->tail = (dq->tail + 1) % dq->capacity;
    }

    dq->data[dq->tail] = value;
    dq->size++;
    return true;
}

bool deque_pop_front(CircularDeque* dq, int* out_value) {
    if (!dq) return false;
    if (dq->size == 0) {
        return false; // empty
    }

    if (out_value) {
        *out_value = dq->data[dq->head];
    }

    if (dq->size == 1) {
        // becomes empty
        dq->size = 0;
        // head/tail don't matter now
    } else {
        dq->head = (dq->head + 1) % dq->capacity;
        dq->size--;
    }
    return true;
}

bool deque_pop_back(CircularDeque* dq, int* out_value) {
    if (!dq) return false;
    if (dq->size == 0) {
        return false; // empty
    }

    if (out_value) {
        *out_value = dq->data[dq->tail];
    }

    if (dq->size == 1) {
        // becomes empty
        dq->size = 0;
    } else {
        dq->tail = (dq->tail - 1 + dq->capacity) % dq->capacity;
        dq->size--;
    }
    return true;
}

bool deque_front(const CircularDeque* dq, int* out_value) {
    if (!dq || !out_value) return false;
    if (dq->size == 0) {
        return false; // empty
    }
    *out_value = dq->data[dq->head];
    return true;
}

bool deque_back(const CircularDeque* dq, int* out_value) {
    if (!dq || !out_value) return false;
    if (dq->size == 0) {
        return false; // empty
    }
    *out_value = dq->data[dq->tail];
    return true;
}
