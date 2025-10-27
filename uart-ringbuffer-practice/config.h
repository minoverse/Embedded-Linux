#pragma once

#define RING_BUFFER_SIZE 64              // must be power of 2
#define HIGH_WATERMARK   (RING_BUFFER_SIZE * 3 / 4)   // 75% (48)
#define LOW_WATERMARK    (RING_BUFFER_SIZE / 4)       // 25% (16)

// Overflow policy: default = drop newest.
// Uncomment to overwrite oldest when full.
// #define OVERWRITE_OLDEST
