Circular Deque (Double-Ended Queue) in C

This project implements a fixed-size circular deque (double-ended queue) in pure C.
It works similarly to a ring buffer, but with one key difference:

A deque allows insertion and removal at both the front and the back,
while a ring buffer (circular queue) supports operations at only one end.

This implementation is lightweight, dependency-free, and written to be reusable in embedded, systems, or algorithmic projects.

Overview

A deque (“deck”) is a data structure that supports:

push_front

push_back

pop_front

pop_back

front

back

All operations run in O(1) time.

Internally it uses:

a fixed-size array

a head index (front element)

a tail index (back element)

a size counter

Both ends grow and shrink using circular arithmetic.

How It Differs from a Ring Buffer

Most ring buffers use:

head → next write
tail → next read


Both pointers only move forward. This works because a queue only pushes at one end and pops at the other.

A deque, however, needs to:

Write at front or back

Read at front or back

Move head and tail both forward and backward

So the classic “head = write pointer, tail = read pointer” model does not work.

Instead:

head = index of current front element
tail = index of current back element


Both move forward/backward depending on operation.

 File Structure

│
├── deque.h        # header
├── deque.c        # Implementation
└── main.c         # Example usage / test program

deque.h 
CircularDeque* deque_create(int capacity);
void           deque_destroy(CircularDeque* dq);

bool deque_push_front(CircularDeque* dq, int value);
bool deque_push_back (CircularDeque* dq, int value);

bool deque_pop_front(CircularDeque* dq, int* out_value);
bool deque_pop_back (CircularDeque* dq, int* out_value);

bool deque_front(const CircularDeque* dq, int* out_value);
bool deque_back (const CircularDeque* dq, int* out_value);

bool deque_is_empty(const CircularDeque* dq);
bool deque_is_full (const CircularDeque* dq);

int  deque_size    (const CircularDeque* dq);
int  deque_capacity(const CircularDeque* dq);

Example (main.c)
#include <stdio.h>
#include "deque.h"

int main(void) {
    CircularDeque* dq = deque_create(3);

    deque_push_back(dq, 10);     // [10]
    deque_push_back(dq, 20);     // [10, 20]
    deque_push_front(dq, 5);     // [5, 10, 20]

    int value;
    deque_front(dq, &value);     // 5
    printf("Front: %d\n", value);

    deque_back(dq, &value);      // 20
    printf("Back : %d\n", value);

    deque_pop_front(dq, &value); // removes 5
    deque_pop_back(dq, &value);  // removes 20

    deque_push_front(dq, 42);    // [42, 10]

    deque_destroy(dq);
    return 0;
}

Example Program Output

Running the provided main.c prints:

=== Initial ===
size = 0, capacity = 3, empty = true, full = false
front = (none)
back  = (none)

push_back(10)
=== After push_back(10) ===
size = 1, capacity = 3, empty = false, full = false
front = 10
back  = 10

push_back(20)
=== After push_back(20) ===
size = 2, capacity = 3, empty = false, full = false
front = 10
back  = 20

push_front(5)
=== After push_front(5) ===
size = 3, capacity = 3, empty = false, full = true
front = 5
back  = 20

Try push_back(99) on full deque -> failed (full)
=== After push_back(99) ===
size = 3, capacity = 3, empty = false, full = true
front = 5
back  = 20

pop_front -> 5
=== After pop_front ===
size = 2, capacity = 3, empty = false, full = false
front = 10
back  = 20

pop_back -> 20
=== After pop_back ===
size = 1, capacity = 3, empty = false, full = false
front = 10
back  = 10

push_front(42)
=== After push_front(42) ===
size = 2, capacity = 3, empty = false, full = false
front = 42
back  = 10

Clearing all elements with pop_front...
  popped 42
  popped 10
=== After clearing ===
size = 0, capacity = 3, empty = true, full = false
front = (none)
back  = (none)

Build Instructions
gcc -std=c11 -Wall -Wextra -o deque_demo main.c deque.c
./deque_demo
