// tests.c - simple acceptance tests using assert
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include "circular_buffer.h"

#define ASSERT_TRUE(x)  assert((x))
#define ASSERT_EQ(a,b)  assert((a) == (b))
#define ASSERT_MEMEQ(a,b,n) assert(memcmp((a),(b),(n))==0)

static void test_basic_write_read(void) {
    CircularBuffer* cb = cb_create(5);
    ASSERT_TRUE(cb);

    uint8_t in[] = {1,2,3};
    ASSERT_EQ(cb_write(cb, in, 3), 3);
    ASSERT_TRUE(!cb_is_empty(cb));
    ASSERT_EQ(cb_available(cb), 2);

    uint8_t out[3] = {0};
    ASSERT_EQ(cb_read(cb, out, 3), 3);
    ASSERT_MEMEQ(out, in, 3);
    ASSERT_TRUE(cb_is_empty(cb));

    cb_destroy(cb);
}

static void test_wrap_and_overflow(void) {
    CircularBuffer* cb = cb_create(4);
    ASSERT_TRUE(cb);

    uint8_t a[] = {10,11,12,13};
    ASSERT_EQ(cb_write(cb, a, 4), 4);
    ASSERT_TRUE(cb_is_full(cb));

    // This write should overwrite oldest (10)
    uint8_t b[] = {99};
    ASSERT_EQ(cb_write(cb, b, 1), 1);
    ASSERT_EQ(cb_clear_overflow(cb), 1);  // overflow happened
    ASSERT_EQ(cb_clear_overflow(cb), 0);  // now cleared

    uint8_t out[4] = {0};
    ASSERT_EQ(cb_read(cb, out, 4), 4);    // reads 11,12,13,99
    uint8_t exp[] = {11,12,13,99};
    ASSERT_MEMEQ(out, exp, 4);

    cb_destroy(cb);
}

static void test_partial_read_and_available(void) {
    CircularBuffer* cb = cb_create(3);
    ASSERT_TRUE(cb);
    uint8_t in[] = {1,2,3};
    ASSERT_EQ(cb_write(cb, in, 3), 3);

    uint8_t out[2];
    ASSERT_EQ(cb_read(cb, out, 2), 2);    // now one left
    ASSERT_EQ(cb_available(cb), 2);
    ASSERT_TRUE(!cb_is_full(cb));

    cb_destroy(cb);
}

static void test_len_gt_capacity(void) {
    CircularBuffer* cb = cb_create(3);
    ASSERT_TRUE(cb);
    uint8_t in[] = {7,8,9,10,11};
    ASSERT_EQ(cb_write(cb, in, 5), 5); // write more than capacity

    uint8_t out[3] = {0};
    ASSERT_EQ(cb_clear_overflow(cb), 1);
    ASSERT_EQ(cb_read(cb, out, 3), 3);
    uint8_t exp[] = {9,10,11};
    ASSERT_MEMEQ(out, exp, 3);

    cb_destroy(cb);
}

int main(void) {
    test_basic_write_read();
    test_wrap_and_overflow();
    test_partial_read_and_available();
    test_len_gt_capacity();
    return 0;
}
