// circular_buffer.c       
#include "circular_buffer.h"
#include <stdlib.h> // malloc, free

/* Opaque type defined here */
struct CircularBuffer {
    uint8_t* buffer;
    size_t   size;               // capacity (bytes)
    size_t   head;               // next write index
    size_t   tail;               // next read index
    size_t   count;              // number of stored bytes
    int      overflow_occurred;  // sticky overflow flag
};

/* Create a circular buffer of given size. Returns NULL incase of  error. */
CircularBuffer* cb_create(size_t size) {
    if (size == 0) {
        return NULL;
    }
    CircularBuffer* cb = malloc(sizeof(*cb));
    if (!cb) {
        return NULL;
    }
    cb->buffer = malloc(size);
    if (!cb->buffer) {
        free(cb);
        return NULL;
    }

    cb->size = size;
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
    cb->overflow_occurred = 0;
    return cb;
}

/* Free all allocated memory; safe to call with NULL. */
void cb_destroy(CircularBuffer* cb) {
    if (cb) {
        free(cb->buffer);
        free(cb);
    }
}

int cb_is_full(const CircularBuffer* cb) {
    if (!cb) return 0;
    return cb->count == cb->size;
}

int cb_is_empty(const CircularBuffer* cb) {
    if (!cb) return 1;
    return cb->count == 0;
}

/* Write len bytes; overwrite oldest when full. Returns bytes written or -1. */
int cb_write(CircularBuffer* cb, const uint8_t* data, size_t len) {
    if (!cb || !data) {
        return -1;
    }
    int written = 0;
    for (size_t i = 0; i < len; ++i) {
        if (cb->count == cb->size) {
            // Buffer is full: drop the oldest byte (advance tail)
            cb->overflow_occurred = 1;
            cb->tail = (cb->tail + 1) % cb->size;
        } else {
            cb->count++;
        }
        cb->buffer[cb->head] = data[i];
        cb->head = (cb->head + 1) % cb->size;
        written++;
    }
    return written;
}

/* Read up to len bytes into out; returns bytes read or -1. */
int cb_read(CircularBuffer* cb, uint8_t* out, size_t len) {
    if (!cb || !out) {
        return -1;
    }
    int read = 0;
    while (len > 0 && cb->count > 0) {
        out[read] = cb->buffer[cb->tail];
        cb->tail = (cb->tail + 1) % cb->size;
        cb->count--;
        read++;
        len--;
    }
    return read; // may be 0 if empty
}

size_t cb_available(const CircularBuffer* cb) {
    if (!cb) return 0;
    return cb->size - cb->count;
}

int cb_clear_overflow(CircularBuffer* cb) {
    if (!cb) return 0;
    int prev = cb->overflow_occurred;
    cb->overflow_occurred = 0;
    return prev;
}
