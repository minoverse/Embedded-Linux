// circular_buffer.h
#pragma once
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CircularBuffer CircularBuffer;

/* Lifecycle */
CircularBuffer* cb_create(size_t size);
void            cb_destroy(CircularBuffer* cb);

/* State checks */
int     cb_is_full(const CircularBuffer* cb);
int     cb_is_empty(const CircularBuffer* cb);

/* Data path */
int     cb_write(CircularBuffer* cb, const uint8_t* data, size_t len); // returns bytes written or -1
int     cb_read (CircularBuffer* cb, uint8_t* out, size_t len);        // returns bytes read or -1

/* Introspection */
size_t  cb_available(const CircularBuffer* cb); // free capacity
int     cb_clear_overflow(CircularBuffer* cb);  // returns previous flag and clears

#ifdef __cplusplus
}
#endif
