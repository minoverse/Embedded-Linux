#include "uart_hal.h"

// --- Replace these with your MCU's registers/driver ---
#define UART_SR_RXNE   (1u << 5)
#define UART_SR_TXE    (1u << 7)
volatile uint32_t UART_SR;
volatile uint8_t  UART_DR;
volatile uint32_t UART_CR1;
#define UART_CR1_RXNEIE (1u << 5)
// ------------------------------------------------------

ring_buffer_t uart_rx_buffer;

void uart_init(void) {
    ring_buffer_init(&uart_rx_buffer);
    UART_CR1 |= UART_CR1_RXNEIE; // enable RX IRQ
}

void uart_tx_byte(uint8_t b) {
    while (!(UART_SR & UART_SR_TXE)) { /* wait */ }
    UART_DR = b;
}

void uart_rx_isr_byte(uint8_t b) {
    (void)ring_buffer_push(&uart_rx_buffer, b);
}

// Example IRQ handler that reads DR then calls our ISR glue.
void USART1_IRQHandler(void) {
    if (UART_SR & UART_SR_RXNE) {
        uint8_t byte = UART_DR;   // read clears RXNE
        uart_rx_isr_byte(byte);
    }
}

void uart_echo_task(void) {
    uint8_t byte;
    while (ring_buffer_pop(&uart_rx_buffer, &byte)) {
        uart_tx_byte(byte);
    }
}

void uart_monitor_task(void) {
    ring_buffer_status_t st;
    ring_buffer_get_status(&uart_rx_buffer, &st);
    // Hook: if (st.high_wm_active) assert RTS / slow producer, etc.
}
