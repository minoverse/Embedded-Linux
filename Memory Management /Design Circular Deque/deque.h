#ifndef CIRCULAR_DEQUE_H
#define CIRCULAR_DEQUE_H

#include <stdbool.h>

typedef struct CircularDeque CircularDeque;

/**
 * Create a circular deque with a fixed capacity.
 *
 * @param capacity  Maximum number of elements the deque can hold.
 *                  Must be > 0. Returns NULL if allocation fails or capacity <= 0.
 */
CircularDeque* deque_create(int capacity);

/**
 * Destroy the deque and free all associated memory.
 * Safe to call with NULL.
 */
void deque_destroy(CircularDeque* dq);

/**
 * Get the current number of elements stored in the deque.
 */
int deque_size(const CircularDeque* dq);

/**
 * Get the maximum capacity of the deque (set at creation time).
 */
int deque_capacity(const CircularDeque* dq);

/**
 * Returns true if the deque is empty.
 */
bool deque_is_empty(const CircularDeque* dq);

/**
 * Returns true if the deque is full.
 */
bool deque_is_full(const CircularDeque* dq);

/**
 * Insert an element at the front of the deque.
 *
 * @return true on success, false if the deque is full or dq is NULL.
 */
bool deque_push_front(CircularDeque* dq, int value);

/**
 * Insert an element at the back of the deque.
 *
 * @return true on success, false if the deque is full or dq is NULL.
 */
bool deque_push_back(CircularDeque* dq, int value);

/**
 * Remove an element from the front of the deque.
 *
 * @param out_value  If non-NULL, the removed value will be stored here.
 * @return true on success, false if the deque is empty or dq is NULL.
 */
bool deque_pop_front(CircularDeque* dq, int* out_value);

/**
 * Remove an element from the back of the deque.
 *
 * @param out_value  If non-NULL, the removed value will be stored here.
 * @return true on success, false if the deque is empty or dq is NULL.
 */
bool deque_pop_back(CircularDeque* dq, int* out_value);

/**
 * Read (without removing) the front element.
 *
 * @param out_value  Must be non-NULL to receive the value.
 * @return true on success, false if the deque is empty or dq/out_value is NULL.
 */
bool deque_front(const CircularDeque* dq, int* out_value);

/**
 * Read (without removing) the back element.
 *
 * @param out_value  Must be non-NULL to receive the value.
 * @return true on success, false if the deque is empty or dq/out_value is NULL.
 */
bool deque_back(const CircularDeque* dq, int* out_value);

#endif /* CIRCULAR_DEQUE_H */
