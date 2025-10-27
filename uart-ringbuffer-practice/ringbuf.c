#include "ringbuf.h"

void ring_buffer_init(ring_buffer_t *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->overflow_count = 0;
    rb->high_wm_flag = false;
}

bool ring_buffer_is_empty(const ring_buffer_t *rb) {
    return rb->head == rb->tail;
}

bool ring_buffer_is_full(const ring_buffer_t *rb) {
    return (uint8_t)((rb->head + 1u) & RING_BUFFER_MASK) == rb->tail;
}

uint8_t ring_buffer_level(const ring_buffer_t *rb) {
    uint8_t h = rb->head;  // single-byte snapshots (atomic on small MCUs)
    uint8_t t = rb->tail;
    return (uint8_t)((h - t) & RING_BUFFER_MASK);
}

bool ring_buffer_push(ring_buffer_t *rb, uint8_t data) {
    uint8_t next_head = (uint8_t)((rb->head + 1u) & RING_BUFFER_MASK);

    if (next_head == rb->tail) {
        rb->overflow_count++;
#ifdef OVERWRITE_OLDEST
        rb->buffer[rb->head] = data;
        rb->head = next_head;
        rb->tail = (uint8_t)((rb->tail + 1u) & RING_BUFFER_MASK);
        // level unchanged, flags unaffected
        return true;
#else
        return false; // drop newest
#endif
    }

    rb->buffer[rb->head] = data;
    rb->head = next_head;

    // watermark set (upward cross)
    uint8_t level = ring_buffer_level(rb);
    if (!rb->high_wm_flag && level >= HIGH_WATERMARK) {
        rb->high_wm_flag = true;
        // optional: callback/flow control here
    }
    return true;
}

bool ring_buffer_pop(ring_buffer_t *rb, uint8_t *data) {
    if (ring_buffer_is_empty(rb)) return false;

    *data = rb->buffer[rb->tail];
    rb->tail = (uint8_t)((rb->tail + 1u) & RING_BUFFER_MASK);

    // watermark clear (downward cross)
    uint8_t level = ring_buffer_level(rb);
    if (rb->high_wm_flag && level <= LOW_WATERMARK) {
        rb->high_wm_flag = false;
        // optional: callback/flow control release here
    }
    return true;
}

void ring_buffer_get_status(const ring_buffer_t *rb, ring_buffer_status_t *st) {
    st->level = ring_buffer_level(rb);
    st->overflow_count = rb->overflow_count;
    st->high_wm_active = rb->high_wm_flag;
    st->utilization_percent = (uint8_t)((st->level * 100u) / RING_BUFFER_SIZE);
}
