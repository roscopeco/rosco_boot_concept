#include "unity.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_todo(void) {
    int i = 1;
    TEST_ASSERT_EQUAL(i, 1);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_todo);

    return UNITY_END();
}