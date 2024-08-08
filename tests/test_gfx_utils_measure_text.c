#include "unity.h"
#include "graphics_utils.h"

void setUp(void) {
}

void tearDown(void) {
}

/* *************************************************************************************************** */
/* *************************************************************************************************** */
/* *************************************************************************************************** */


/* *************************************************************
 *
 *   eeeLReee.............
 */
void test_gfx_utils_measure_text_null(void) {
    int width_o = 42;
    int height_o = 9001;

    TEST_ASSERT_FALSE(gfx_measure_text(NULL, &width_o, &height_o));
}

void test_gfx_utils_measure_text_empty(void) {
    int width_o = 0;
    int height_o = 0;

    TEST_ASSERT_TRUE(gfx_measure_text("", &width_o, &height_o));

    TEST_ASSERT_EQUAL(0, width_o);
    TEST_ASSERT_EQUAL(0, height_o);
}

void test_gfx_utils_measure_text_one_char(void) {
    int width_o = 0;
    int height_o = 0;

    TEST_ASSERT_TRUE(gfx_measure_text("a", &width_o, &height_o));

    TEST_ASSERT_EQUAL(1, width_o);
    TEST_ASSERT_EQUAL(1, height_o);
}

void test_gfx_utils_measure_text_single_line(void) {
    int width_o = 0;
    int height_o = 0;

    TEST_ASSERT_TRUE(gfx_measure_text("test", &width_o, &height_o));

    TEST_ASSERT_EQUAL(4, width_o);
    TEST_ASSERT_EQUAL(1, height_o);
}

void test_gfx_utils_measure_text_two_equal_lines(void) {
    int width_o = 0;
    int height_o = 0;

    TEST_ASSERT_TRUE(gfx_measure_text("test\ntest", &width_o, &height_o));

    TEST_ASSERT_EQUAL(4, width_o);
    TEST_ASSERT_EQUAL(2, height_o);
}

void test_gfx_utils_measure_text_two_unequal_lines_longest_first(void) {
    int width_o = 0;
    int height_o = 0;

    TEST_ASSERT_TRUE(gfx_measure_text("testing\ntest", &width_o, &height_o));

    TEST_ASSERT_EQUAL(7, width_o);
    TEST_ASSERT_EQUAL(2, height_o);
}

void test_gfx_utils_measure_text_two_unequal_lines_longest_last(void) {
    int width_o = 0;
    int height_o = 0;

    TEST_ASSERT_TRUE(gfx_measure_text("test\ntesting", &width_o, &height_o));

    TEST_ASSERT_EQUAL(7, width_o);
    TEST_ASSERT_EQUAL(2, height_o);
}

void test_gfx_utils_measure_text_blank_line_at_start(void) {
    int width_o = 0;
    int height_o = 0;

    TEST_ASSERT_TRUE(gfx_measure_text("\ntest\ntesting", &width_o, &height_o));

    TEST_ASSERT_EQUAL(7, width_o);
    TEST_ASSERT_EQUAL(3, height_o);
}

void test_gfx_utils_measure_text_blank_line_in_middle(void) {
    int width_o = 0;
    int height_o = 0;

    TEST_ASSERT_TRUE(gfx_measure_text("test\n\ntesting", &width_o, &height_o));

    TEST_ASSERT_EQUAL(7, width_o);
    TEST_ASSERT_EQUAL(3, height_o);
}

void test_gfx_utils_measure_text_blank_line_at_end(void) {
    int width_o = 0;
    int height_o = 0;

    TEST_ASSERT_TRUE(gfx_measure_text("test\ntesting\n", &width_o, &height_o));

    TEST_ASSERT_EQUAL(7, width_o);
    TEST_ASSERT_EQUAL(3, height_o);
}

void test_gfx_utils_measure_text_one_blank_line_only(void) {
    int width_o = 0;
    int height_o = 0;

    TEST_ASSERT_TRUE(gfx_measure_text("\n", &width_o, &height_o));

    TEST_ASSERT_EQUAL(0, width_o);
    TEST_ASSERT_EQUAL(2, height_o);
}

void test_gfx_utils_measure_text_two_blank_lines_only(void) {
    int width_o = 0;
    int height_o = 0;

    TEST_ASSERT_TRUE(gfx_measure_text("\n\n", &width_o, &height_o));

    TEST_ASSERT_EQUAL(0, width_o);
    TEST_ASSERT_EQUAL(3, height_o);
}


int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_gfx_utils_measure_text_null);
    RUN_TEST(test_gfx_utils_measure_text_empty);
    RUN_TEST(test_gfx_utils_measure_text_one_char);
    RUN_TEST(test_gfx_utils_measure_text_single_line);
    RUN_TEST(test_gfx_utils_measure_text_two_equal_lines);
    RUN_TEST(test_gfx_utils_measure_text_two_unequal_lines_longest_first);
    RUN_TEST(test_gfx_utils_measure_text_two_unequal_lines_longest_last);
    RUN_TEST(test_gfx_utils_measure_text_blank_line_at_start);
    RUN_TEST(test_gfx_utils_measure_text_blank_line_in_middle);
    RUN_TEST(test_gfx_utils_measure_text_blank_line_at_end);
    RUN_TEST(test_gfx_utils_measure_text_one_blank_line_only);
    RUN_TEST(test_gfx_utils_measure_text_two_blank_lines_only);

    return UNITY_END();
}