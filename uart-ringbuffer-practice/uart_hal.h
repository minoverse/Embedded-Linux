#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "ringbuf.h"

// Global RX buffer instance used by ISR + main
extern ring_buffer_t uart_rx_buffer;

// Init UART (configure, enable RX interrupt, init ring buffer)
void uart_init(void);

// Blocking TX (demo)
void uart_tx_byte(uint8_t b);

// ISR entry (call from your actual IRQ handler)
void uart_rx_isr_byte(uint8_t b);

// Echo task (drain buffer to TX)
void uart_echo_task(void);

// Optional: status poll
void uart_monitor_task(void);
