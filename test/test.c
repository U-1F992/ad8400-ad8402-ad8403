#include <assert.h>
#include <stdio.h>
#include <string.h>

#define TEST_TEXT_RED "\e[31m"
#define TEST_TEXT_GREEN "\e[32m"
#define TEST_TEXT_RESET "\e[0m"

static unsigned char test_recent_buffer[128];
static size_t test_recent_count = 0;

static void test_reset(void) { test_recent_count = 0; }

#define ad8400_ad8402_ad8403_spi_write(buffer, count)                          \
    do {                                                                       \
        memcpy(test_recent_buffer, (buffer), (count));                         \
        test_recent_count = (count);                                           \
    } while (0)

#include <ad8400_ad8402_ad8403.h>

typedef struct test_case_t {
    ad8400_ad8402_ad8403_address_t address;
    unsigned char data;

    size_t expected_count;
    unsigned char *expected_buffer;
} test_case_t;

int test_set(void) {
    size_t i = 0;
    size_t j = 0;
    test_case_t test_cases[] = {
        {.address = AD8400_AD8402_AD8403_RDAC1,
         .data = 0,
         .expected_count = 2,
         .expected_buffer =
             (unsigned char[]){/* 0b______00'00000000 */ 0x00, 0x00}},
        {.address = AD8400_AD8402_AD8403_RDAC1,
         .data = 128,
         .expected_count = 2,
         .expected_buffer =
             (unsigned char[]){/* 0b______00'10000000 */ 0x00, 0x80}},
        {.address = AD8400_AD8402_AD8403_RDAC1,
         .data = 255,
         .expected_count = 2,
         .expected_buffer =
             (unsigned char[]){/* 0b______00'11111111 */ 0x00, 0xFF}},
        {.address = AD8400_AD8402_AD8403_RDAC2,
         .data = 0,
         .expected_count = 2,
         .expected_buffer =
             (unsigned char[]){/* 0b______01'00000000 */ 0x01, 0x00}},
        {.address = AD8400_AD8402_AD8403_RDAC3,
         .data = 0,
         .expected_count = 2,
         .expected_buffer =
             (unsigned char[]){/* 0b______10'00000000 */ 0x02, 0x00}},
        {.address = AD8400_AD8402_AD8403_RDAC4,
         .data = 0,
         .expected_count = 2,
         .expected_buffer =
             (unsigned char[]){/* 0b______11'00000000 */ 0x03, 0x00}}};
    int test_failure_count = 0;

    printf("* %s\n", __func__);
    for (i = 0; i < sizeof(test_cases) / sizeof(test_case_t); i++) {
        test_case_t *test_case = &test_cases[i];
        size_t actual_count = 0;
        unsigned char *actual_buffer = test_recent_buffer;

        test_reset();

        ad8400_ad8402_ad8403_set(test_case->address, test_case->data);

        if (test_case->expected_count != (actual_count = test_recent_count)) {
            fprintf(stderr,
                    "%sindex: %d\n  expected_count: %d\n  actual_count: %d%s\n",
                    TEST_TEXT_RED, i, test_case->expected_count, actual_count,
                    TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        if (memcmp(test_case->expected_buffer, actual_buffer, actual_count) !=
            0) {
            fprintf(stderr, "%sindex: %d\n  expected_buffer: {", TEST_TEXT_RED,
                    i);
            for (j = 0; j < actual_count; j++) {
                fprintf(stderr, "%d%s", test_case->expected_buffer[j],
                        j != actual_count - 1 ? ", " : "");
            }
            fprintf(stderr, "}\n  actual_buffer: {");
            for (j = 0; j < actual_count; j++) {
                fprintf(stderr, "%d%s", actual_buffer[j],
                        j != actual_count - 1 ? ", " : "");
            }
            fprintf(stderr, "}%s\n", TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
    }

    return test_failure_count;
}

int main(void) {
    int test_failure_count = 0;

    test_failure_count += test_set();

    printf("\n");
    if (test_failure_count == 0) {
        printf("%sOK%s\n", TEST_TEXT_GREEN, TEST_TEXT_RESET);
    } else {
        fprintf(stderr, "%s%d error%s found.%s\n", TEST_TEXT_RED,
                test_failure_count, test_failure_count != 1 ? "s" : "",
                TEST_TEXT_RESET);
    }
    return test_failure_count;
}
