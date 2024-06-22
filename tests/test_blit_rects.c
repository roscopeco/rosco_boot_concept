#include "unity.h"
#include "blit_rects.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_blit_lines_case_1(void) {
    BlitLines result;

    blit_lines(1, 9, &result);

    TEST_ASSERT_EQUAL(result.left.start, 0);
    TEST_ASSERT_EQUAL(result.left.words, 1);
    TEST_ASSERT_EQUAL(result.left.value, 0x0FFF);

    TEST_ASSERT_EQUAL(result.main.start, 1);
    TEST_ASSERT_EQUAL(result.main.words, 1);
    TEST_ASSERT_EQUAL(result.main.value, 0xFFFF);

    TEST_ASSERT_EQUAL(result.right.start, 2);
    TEST_ASSERT_EQUAL(result.right.words, 1);
    TEST_ASSERT_EQUAL(result.right.value, 0xFF00);
}

void test_blit_lines_case_2(void) {
    BlitLines result;

    blit_lines(1, 13, &result);

    TEST_ASSERT_EQUAL(result.left.start, 0);
    TEST_ASSERT_EQUAL(result.left.words, 1);
    TEST_ASSERT_EQUAL(result.left.value, 0x0FFF);

    TEST_ASSERT_EQUAL(result.main.start, 1);
    TEST_ASSERT_EQUAL(result.main.words, 2);
    TEST_ASSERT_EQUAL(result.main.value, 0xFFFF);

    TEST_ASSERT_EQUAL(result.right.start, 3);
    TEST_ASSERT_EQUAL(result.right.words, 1);
    TEST_ASSERT_EQUAL(result.right.value, 0xFF00);
}

void test_blit_lines_case_3(void) {
    BlitLines result;

    blit_lines(0, 4, &result);

    TEST_ASSERT_EQUAL(result.left.words, 0);        // no left rect, other values undef

    TEST_ASSERT_EQUAL(result.main.start, 0);
    TEST_ASSERT_EQUAL(result.main.words, 1);
    TEST_ASSERT_EQUAL(result.main.value, 0xFFFF);

    TEST_ASSERT_EQUAL(result.right.words, 0);        // no right rect, other values undef
}

void test_blit_lines_case_4(void) {
    BlitLines result;

    blit_lines(3, 2, &result);

    TEST_ASSERT_EQUAL(result.left.start, 0);
    TEST_ASSERT_EQUAL(result.left.words, 1);
    TEST_ASSERT_EQUAL(result.left.value, 0x000F);

    TEST_ASSERT_EQUAL(result.main.words, 0);        // no main rect, other values undef

    TEST_ASSERT_EQUAL(result.right.start, 1);
    TEST_ASSERT_EQUAL(result.right.words, 1);
    TEST_ASSERT_EQUAL(result.right.value, 0xF000);
}

void test_blit_lines_case_5(void) {
    BlitLines result;

    blit_lines(3, 1, &result);

    TEST_ASSERT_EQUAL(result.left.start, 0);
    TEST_ASSERT_EQUAL(result.left.words, 1);
    TEST_ASSERT_EQUAL(result.left.value, 0x000F);

    TEST_ASSERT_EQUAL(result.main.words, 0);        // no main rect, other values undef

    TEST_ASSERT_EQUAL(result.right.words, 0);        // no right rect, other values undef
}

void test_blit_lines_case_6(void) {
    BlitLines result;

    blit_lines(4, 4, &result);

    TEST_ASSERT_EQUAL(result.left.words, 0);        // no left rect, other values undef

    TEST_ASSERT_EQUAL(result.main.start, 1);
    TEST_ASSERT_EQUAL(result.main.words, 1);
    TEST_ASSERT_EQUAL(result.main.value, 0xFFFF);

    TEST_ASSERT_EQUAL(result.right.words, 0);        // no right rect, other values undef
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_blit_lines_case_1);
    RUN_TEST(test_blit_lines_case_2);
    RUN_TEST(test_blit_lines_case_3);
    RUN_TEST(test_blit_lines_case_4);
    RUN_TEST(test_blit_lines_case_5);
    RUN_TEST(test_blit_lines_case_6);

    return UNITY_END();
}