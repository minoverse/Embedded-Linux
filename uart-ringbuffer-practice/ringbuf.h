#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "config.h"

#if (RING_BUFFER_SIZE & (RING_BUFFER_SIZE-1)) != 0
# error "RING_BUFFER_SIZE must be a power of 2"
#endif

#define RING_BUFFER_MASK (RING_BUFFER_SIZE - 1)

typedef struct {
    volatile uint8_t buffer[RING_BUFFER_SIZE];
    volatile uint8_t head;         // ISR producer
    volatile uint8_t tail;         // main consumer
    volatile uint32_t overflow_count;
    volatile bool high_wm_flag;
} ring_buffer_t;

typedef struct {
    uint8_t  level;                // 0..63
    uint32_t overflow_count;
    bool     high_wm_active;
    uint8_t  utilization_percent;  // 0..100
} ring_buffer_status_t;

// Core
void     ring_buffer_init(ring_buffer_t *rb);
bool     ring_buffer_is_empty(const ring_buffer_t *rb);
bool     ring_buffer_is_full (const ring_buffer_t *rb);
uint8_t  ring_buffer_level   (const ring_buffer_t *rb);
bool     ring_buffer_push    (ring_buffer_t *rb, uint8_t data);     // ISR
bool     ring_buffer_pop     (ring_buffer_t *rb, uint8_t *data);    // main
void     ring_buffer_get_status(const ring_buffer_t *rb, ring_buffer_status_t *st);
