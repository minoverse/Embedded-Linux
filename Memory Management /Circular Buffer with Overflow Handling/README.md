//MAKEFILE
CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -O2

SRC = circular_buffer.c
HDR = circular_buffer.h

all: cb_demo tests

cb_demo: $(SRC) main.c $(HDR)
	$(CC) $(CFLAGS) $(SRC) main.c -o $@

tests: $(SRC) tests.c $(HDR)
	$(CC) $(CFLAGS) $(SRC) tests.c -o $@

run: cb_demo
	./cb_demo

test: tests
	./tests

san: clean
	$(CC) -std=c11 -Wall -Wextra -O1 -g -fsanitize=address,undefined $(SRC) tests.c -o tests_san
	./tests_san

clean:
	rm -f cb_demo tests tests_san

.PHONY: all run test san clean


//README.md
# Circular Buffer (Ring Buffer) with Overwrite-on-Overflow

This project implements a fixed-size circular byte buffer in C. When full, new writes **overwrite the oldest data** and set a **sticky overflow flag** that can be queried and cleared. 
head = index where the next write (push) will go

tail = index where the next read (pop) will come from

## What you asked for (the task/prompt)

**“Task: Circular Buffer with Overflow Handling”**

- Implement a circular buffer of bytes in C.
- API:
  - `cb_create`, `cb_destroy`
  - `cb_is_full`, `cb_is_empty`
  - `cb_write`, `cb_read`
  - `cb_available`, `cb_clear_overflow`
- Behavior:
  - All ops O(1).
  - On write when full: **overwrite oldest** (advance `tail`) and set overflow flag.
  - `cb_clear_overflow()` returns previous flag and clears it.
  - Return `-1` on invalid args; `cb_create(0)` returns `NULL`.
- Deliverables: header + source, small demo (`main.c`), quick tests, README.

This repository contains exactly that, plus a small `Makefile`.

## Build

```bash
make           # builds cb_demo and tests
make run       # runs the demo
make test      # runs the tests
make san       # sanitizer build + run
make clean

//API
typedef struct CircularBuffer CircularBuffer;

CircularBuffer* cb_create(size_t size);
void            cb_destroy(CircularBuffer* cb);

int     cb_is_full(const CircularBuffer* cb);
int     cb_is_empty(const CircularBuffer* cb);

int     cb_write(CircularBuffer* cb, const uint8_t* data, size_t len);
int     cb_read (CircularBuffer* cb, uint8_t* out, size_t len);

size_t  cb_available(const CircularBuffer* cb);
int     cb_clear_overflow(CircularBuffer* cb);
