# Circular Deque (Double-Ended Queue) in C

This folder provides a **fixed-size circular deque** (double-ended queue) implemented in C.

It is designed to live alongside a **ring buffer (circular queue)** implementation.  
Both use a circular array internally, but they have different semantics:

- **Ring buffer**: one end for write (`enqueue`), one end for read (`dequeue`)
- **Deque**: both ends can be used for read **and** write (`front` / `back`)

---

## 1. Concept: Ring Buffer vs Circular Deque

### Ring Buffer (Queue)

Typical ring buffer pointer meaning:

- `head` → **next write index** (enqueue here, then move forward)
- `tail` → **next read index** (dequeue here, then move forward)

Operations:

```text
Enqueue (push):
    data[head] = value
    head = (head + 1) % capacity

Dequeue (pop):
    value = data[tail]
    tail = (tail + 1) % capacity
