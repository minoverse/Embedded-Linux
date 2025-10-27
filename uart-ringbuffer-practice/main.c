#include "uart_hal.h"

int main(void) {
    uart_init();

    for (;;) {
        uart_echo_task();     // drain RX -> TX (echo)
        uart_monitor_task();  // optional backpressure hook
        // ...other work...
    }
}
