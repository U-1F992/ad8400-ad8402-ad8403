#include <ad8400_ad8402_ad8403.h>

#include <assert.h>
#include <stdio.h>

#define TEST_TEXT_RED "\e[31m"
#define TEST_TEXT_GREEN "\e[32m"
#define TEST_TEXT_RESET "\e[0m"

typedef struct test_spi_main_t
{
    ad8400_ad8402_ad8403_spi_main_t parent;
    size_t recent_size;
    uint8_t recent_data[128];
} test_spi_main_t;

ad8400_ad8402_ad8403_error_t test_spi_main_transmit(ad8400_ad8402_ad8403_spi_main_t *parent, uint8_t data[], size_t size)
{
    test_spi_main_t *spi_main = (test_spi_main_t *)parent;
    assert(spi_main != NULL);

    spi_main->recent_size = size;
    for (size_t i = 0; i < size; i++)
    {
        spi_main->recent_data[i] = data[i];
    }

    return AD8400_AD8402_AD8403_SUCCESS;
}

void test_spi_main_init(test_spi_main_t *spi_main)
{
    assert(spi_main != NULL);

    spi_main->parent.transmit = test_spi_main_transmit;
}

typedef enum test_digital_output_state_t
{
    TEST_DIGITAL_OUTPUT_LOW,
    TEST_DIGITAL_OUTPUT_HIGH
} test_digital_output_state_t;

typedef struct test_digital_output_t
{
    ad8400_ad8402_ad8403_digital_output_t parent;
    test_digital_output_state_t state;
} test_digital_output_t;

ad8400_ad8402_ad8403_error_t test_digital_output_set_low(ad8400_ad8402_ad8403_digital_output_t *parent)
{
    test_digital_output_t *digital_output = (test_digital_output_t *)parent;
    assert(digital_output != NULL);

    digital_output->state = TEST_DIGITAL_OUTPUT_LOW;

    return AD8400_AD8402_AD8403_SUCCESS;
}

ad8400_ad8402_ad8403_error_t test_digital_output_set_high(ad8400_ad8402_ad8403_digital_output_t *parent)
{
    test_digital_output_t *digital_output = (test_digital_output_t *)parent;
    assert(digital_output != NULL);

    digital_output->state = TEST_DIGITAL_OUTPUT_HIGH;

    return AD8400_AD8402_AD8403_SUCCESS;
}

void test_digital_output_init(test_digital_output_t *digital_output)
{
    assert(digital_output != NULL);

    digital_output->parent.set_low = test_digital_output_set_low;
    digital_output->parent.set_high = test_digital_output_set_high;
}

int test_ad8400_ad8402_ad8403_init(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ad8400_ad8402_ad8403_t *ad8400_ad8402_ad8403;
        ad8400_ad8402_ad8403_spi_main_t *spi_main;
        ad8400_ad8402_ad8403_digital_output_t *shdn;

        ad8400_ad8402_ad8403_error_t expected_ret;
    } test_case_t;

    ad8400_ad8402_ad8403_t ad8400_ad8402_ad8403[8];
    ad8400_ad8402_ad8403_spi_main_t spi_main[8];
    ad8400_ad8402_ad8403_digital_output_t shdn[8];

    test_case_t test_cases[] = {{.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = NULL, /*   */ .shdn = NULL, /*    */ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[1], .spi_main = NULL, /*   */ .shdn = NULL, /*    */ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = &spi_main[2], .shdn = NULL, /*    */ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[3], .spi_main = &spi_main[3], .shdn = NULL, /*    */ .expected_ret = AD8400_AD8402_AD8403_SUCCESS},
                                {.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = NULL, /*   */ .shdn = &shdn[4], /**/ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[5], .spi_main = NULL, /*   */ .shdn = &shdn[5], /**/ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = &spi_main[6], .shdn = &shdn[6], /**/ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[7], .spi_main = &spi_main[7], .shdn = &shdn[7], /**/ .expected_ret = AD8400_AD8402_AD8403_SUCCESS}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ad8400_ad8402_ad8403_error_t actual_ret = ad8400_ad8402_ad8403_init(test_case->ad8400_ad8402_ad8403, test_case->spi_main, test_case->shdn);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
    }

    return test_failure_count;
}

int test_ad8400_ad8402_ad8403_rdac1_set(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ad8400_ad8402_ad8403_t *ad8400_ad8402_ad8403;
        test_spi_main_t *spi_main;
        uint8_t data;

        ad8400_ad8402_ad8403_error_t expected_ret;
        size_t expected_size;
        uint8_t *expected_data;
    } test_case_t;

    ad8400_ad8402_ad8403_t ad8400_ad8402_ad8403[6];
    test_spi_main_t spi_main[6];
    for (size_t i = 0; i < 6; i++)
    {
        test_spi_main_init(&spi_main[i]);
        ad8400_ad8402_ad8403_error_t err = ad8400_ad8402_ad8403_init(&ad8400_ad8402_ad8403[i], (ad8400_ad8402_ad8403_spi_main_t *)&spi_main[i], NULL);
        assert(err == AD8400_AD8402_AD8403_SUCCESS);
    }

    test_case_t test_cases[] = {{.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = &spi_main[0], .data = 0, /*  */ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[1], .spi_main = &spi_main[1], .data = 0, /*  */ .expected_ret = AD8400_AD8402_AD8403_SUCCESS, .expected_size = 2, .expected_data = (uint8_t[]){/* 0b______00'00000000 */ 0x00, 0x00}},
                                {.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = &spi_main[2], .data = 128, /**/ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[3], .spi_main = &spi_main[3], .data = 128, /**/ .expected_ret = AD8400_AD8402_AD8403_SUCCESS, .expected_size = 2, .expected_data = (uint8_t[]){/* 0b______00'10000000 */ 0x00, 0x80}},
                                {.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = &spi_main[4], .data = 255, /**/ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[5], .spi_main = &spi_main[5], .data = 255, /**/ .expected_ret = AD8400_AD8402_AD8403_SUCCESS, .expected_size = 2, .expected_data = (uint8_t[]){/* 0b______00'11111111 */ 0x00, 0xFF}}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ad8400_ad8402_ad8403_error_t actual_ret = ad8400_ad8402_ad8403_rdac1_set(test_case->ad8400_ad8402_ad8403, test_case->data);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
        else if (actual_ret != 0)
        {
            continue;
        }

        size_t actual_size = test_case->spi_main->recent_size;
        if (test_case->expected_size != actual_size)
        {
            fprintf(stderr, "%sindex: %d, expected_size: %d, actual_size: %d%s\n", TEST_TEXT_RED, i, test_case->expected_size, actual_size, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        uint8_t *actual_data = test_case->spi_main->recent_data;
        for (size_t j = 0; j < actual_size; j++)
        {
            if (test_case->expected_data[j] != actual_data[j])
            {
                fprintf(stderr, "%sindex: %d, expected_data[%d]: %d, actual_data[%d]: %d%s\n", TEST_TEXT_RED, i, j, test_case->expected_data[j], j, actual_data[j], TEST_TEXT_RESET);
                test_failure_count++;
                continue;
            }
        }
    }

    return test_failure_count;
}

int test_ad8400_ad8402_ad8403_rdac2_set(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ad8400_ad8402_ad8403_t *ad8400_ad8402_ad8403;
        test_spi_main_t *spi_main;
        uint8_t data;

        ad8400_ad8402_ad8403_error_t expected_ret;
        size_t expected_size;
        uint8_t *expected_data;
    } test_case_t;

    ad8400_ad8402_ad8403_t ad8400_ad8402_ad8403[6];
    test_spi_main_t spi_main[6];
    for (size_t i = 0; i < 6; i++)
    {
        test_spi_main_init(&spi_main[i]);
        ad8400_ad8402_ad8403_error_t err = ad8400_ad8402_ad8403_init(&ad8400_ad8402_ad8403[i], (ad8400_ad8402_ad8403_spi_main_t *)&spi_main[i], NULL);
        assert(err == AD8400_AD8402_AD8403_SUCCESS);
    }

    test_case_t test_cases[] = {{.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = &spi_main[0], .data = 0, /*  */ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[1], .spi_main = &spi_main[1], .data = 0, /*  */ .expected_ret = AD8400_AD8402_AD8403_SUCCESS, .expected_size = 2, .expected_data = (uint8_t[]){/* 0b______01'00000000 */ 0x01, 0x00}},
                                {.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = &spi_main[2], .data = 128, /**/ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[3], .spi_main = &spi_main[3], .data = 128, /**/ .expected_ret = AD8400_AD8402_AD8403_SUCCESS, .expected_size = 2, .expected_data = (uint8_t[]){/* 0b______01'10000000 */ 0x01, 0x80}},
                                {.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = &spi_main[4], .data = 255, /**/ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[5], .spi_main = &spi_main[5], .data = 255, /**/ .expected_ret = AD8400_AD8402_AD8403_SUCCESS, .expected_size = 2, .expected_data = (uint8_t[]){/* 0b______01'11111111 */ 0x01, 0xFF}}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ad8400_ad8402_ad8403_error_t actual_ret = ad8400_ad8402_ad8403_rdac2_set(test_case->ad8400_ad8402_ad8403, test_case->data);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
        else if (actual_ret != 0)
        {
            continue;
        }

        size_t actual_size = test_case->spi_main->recent_size;
        if (test_case->expected_size != actual_size)
        {
            fprintf(stderr, "%sindex: %d, expected_size: %d, actual_size: %d%s\n", TEST_TEXT_RED, i, test_case->expected_size, actual_size, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        uint8_t *actual_data = test_case->spi_main->recent_data;
        for (size_t j = 0; j < actual_size; j++)
        {
            if (test_case->expected_data[j] != actual_data[j])
            {
                fprintf(stderr, "%sindex: %d, expected_data[%d]: %d, actual_data[%d]: %d%s\n", TEST_TEXT_RED, i, j, test_case->expected_data[j], j, actual_data[j], TEST_TEXT_RESET);
                test_failure_count++;
                continue;
            }
        }
    }

    return test_failure_count;
}

int test_ad8400_ad8402_ad8403_rdac3_set(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ad8400_ad8402_ad8403_t *ad8400_ad8402_ad8403;
        test_spi_main_t *spi_main;
        uint8_t data;

        ad8400_ad8402_ad8403_error_t expected_ret;
        size_t expected_size;
        uint8_t *expected_data;
    } test_case_t;

    ad8400_ad8402_ad8403_t ad8400_ad8402_ad8403[6];
    test_spi_main_t spi_main[6];
    for (size_t i = 0; i < 6; i++)
    {
        test_spi_main_init(&spi_main[i]);
        ad8400_ad8402_ad8403_error_t err = ad8400_ad8402_ad8403_init(&ad8400_ad8402_ad8403[i], (ad8400_ad8402_ad8403_spi_main_t *)&spi_main[i], NULL);
        assert(err == AD8400_AD8402_AD8403_SUCCESS);
    }

    test_case_t test_cases[] = {{.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = &spi_main[0], .data = 0, /*  */ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[1], .spi_main = &spi_main[1], .data = 0, /*  */ .expected_ret = AD8400_AD8402_AD8403_SUCCESS, .expected_size = 2, .expected_data = (uint8_t[]){/* 0b______10'00000000 */ 0x02, 0x00}},
                                {.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = &spi_main[2], .data = 128, /**/ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[3], .spi_main = &spi_main[3], .data = 128, /**/ .expected_ret = AD8400_AD8402_AD8403_SUCCESS, .expected_size = 2, .expected_data = (uint8_t[]){/* 0b______10'10000000 */ 0x02, 0x80}},
                                {.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = &spi_main[4], .data = 255, /**/ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[5], .spi_main = &spi_main[5], .data = 255, /**/ .expected_ret = AD8400_AD8402_AD8403_SUCCESS, .expected_size = 2, .expected_data = (uint8_t[]){/* 0b______10'11111111 */ 0x02, 0xFF}}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ad8400_ad8402_ad8403_error_t actual_ret = ad8400_ad8402_ad8403_rdac3_set(test_case->ad8400_ad8402_ad8403, test_case->data);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
        else if (actual_ret != 0)
        {
            continue;
        }

        size_t actual_size = test_case->spi_main->recent_size;
        if (test_case->expected_size != actual_size)
        {
            fprintf(stderr, "%sindex: %d, expected_size: %d, actual_size: %d%s\n", TEST_TEXT_RED, i, test_case->expected_size, actual_size, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        uint8_t *actual_data = test_case->spi_main->recent_data;
        for (size_t j = 0; j < actual_size; j++)
        {
            if (test_case->expected_data[j] != actual_data[j])
            {
                fprintf(stderr, "%sindex: %d, expected_data[%d]: %d, actual_data[%d]: %d%s\n", TEST_TEXT_RED, i, j, test_case->expected_data[j], j, actual_data[j], TEST_TEXT_RESET);
                test_failure_count++;
                continue;
            }
        }
    }

    return test_failure_count;
}

int test_ad8400_ad8402_ad8403_rdac4_set(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ad8400_ad8402_ad8403_t *ad8400_ad8402_ad8403;
        test_spi_main_t *spi_main;
        uint8_t data;

        ad8400_ad8402_ad8403_error_t expected_ret;
        size_t expected_size;
        uint8_t *expected_data;
    } test_case_t;

    ad8400_ad8402_ad8403_t ad8400_ad8402_ad8403[6];
    test_spi_main_t spi_main[6];
    for (size_t i = 0; i < 6; i++)
    {
        test_spi_main_init(&spi_main[i]);
        ad8400_ad8402_ad8403_error_t err = ad8400_ad8402_ad8403_init(&ad8400_ad8402_ad8403[i], (ad8400_ad8402_ad8403_spi_main_t *)&spi_main[i], NULL);
        assert(err == AD8400_AD8402_AD8403_SUCCESS);
    }

    test_case_t test_cases[] = {{.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = &spi_main[0], .data = 0, /*  */ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[1], .spi_main = &spi_main[1], .data = 0, /*  */ .expected_ret = AD8400_AD8402_AD8403_SUCCESS, .expected_size = 2, .expected_data = (uint8_t[]){/* 0b______11'00000000 */ 0x03, 0x00}},
                                {.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = &spi_main[2], .data = 128, /**/ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[3], .spi_main = &spi_main[3], .data = 128, /**/ .expected_ret = AD8400_AD8402_AD8403_SUCCESS, .expected_size = 2, .expected_data = (uint8_t[]){/* 0b______11'10000000 */ 0x03, 0x80}},
                                {.ad8400_ad8402_ad8403 = NULL, /*               */ .spi_main = &spi_main[4], .data = 255, /**/ .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[5], .spi_main = &spi_main[5], .data = 255, /**/ .expected_ret = AD8400_AD8402_AD8403_SUCCESS, .expected_size = 2, .expected_data = (uint8_t[]){/* 0b______11'11111111 */ 0x03, 0xFF}}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ad8400_ad8402_ad8403_error_t actual_ret = ad8400_ad8402_ad8403_rdac4_set(test_case->ad8400_ad8402_ad8403, test_case->data);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
        else if (actual_ret != 0)
        {
            continue;
        }

        size_t actual_size = test_case->spi_main->recent_size;
        if (test_case->expected_size != actual_size)
        {
            fprintf(stderr, "%sindex: %d, expected_size: %d, actual_size: %d%s\n", TEST_TEXT_RED, i, test_case->expected_size, actual_size, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        uint8_t *actual_data = test_case->spi_main->recent_data;
        for (size_t j = 0; j < actual_size; j++)
        {
            if (test_case->expected_data[j] != actual_data[j])
            {
                fprintf(stderr, "%sindex: %d, expected_data[%d]: %d, actual_data[%d]: %d%s\n", TEST_TEXT_RED, i, j, test_case->expected_data[j], j, actual_data[j], TEST_TEXT_RESET);
                test_failure_count++;
                continue;
            }
        }
    }

    return test_failure_count;
}

int test_ad8400_ad8402_ad8403_enter_shutdown_mode(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ad8400_ad8402_ad8403_t *ad8400_ad8402_ad8403;
        test_digital_output_t *digital_output;

        ad8400_ad8402_ad8403_error_t expected_ret;
        test_digital_output_state_t expected_state;
    } test_case_t;

    ad8400_ad8402_ad8403_t ad8400_ad8402_ad8403[2];
    ad8400_ad8402_ad8403_spi_main_t spi_main[2];
    test_digital_output_t digital_output[2];
    for (size_t i = 0; i < 2; i++)
    {
        test_digital_output_init(&digital_output[i]);
        ad8400_ad8402_ad8403_error_t err = ad8400_ad8402_ad8403_init(&ad8400_ad8402_ad8403[i], &spi_main[i], (ad8400_ad8402_ad8403_digital_output_t *)&digital_output[i]);
        assert(err == AD8400_AD8402_AD8403_SUCCESS);
    }

    test_case_t test_cases[] = {{.ad8400_ad8402_ad8403 = NULL, /*               */ .digital_output = &digital_output[0], .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[1], .digital_output = &digital_output[1], .expected_ret = AD8400_AD8402_AD8403_SUCCESS, .expected_state = TEST_DIGITAL_OUTPUT_LOW}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ad8400_ad8402_ad8403_error_t actual_ret = ad8400_ad8402_ad8403_enter_shutdown_mode(test_case->ad8400_ad8402_ad8403);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
        else if (actual_ret != 0)
        {
            continue;
        }

        test_digital_output_state_t actual_state = test_case->digital_output->state;
        if (test_case->expected_state != actual_state)
        {
            fprintf(stderr, "%sindex: %d, expected_state: %d, actual_state: %d%s\n", TEST_TEXT_RED, i, test_case->expected_state, actual_state, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
    }

    return test_failure_count;
}

int test_ad8400_ad8402_ad8403_enter_operational_mode(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ad8400_ad8402_ad8403_t *ad8400_ad8402_ad8403;
        test_digital_output_t *digital_output;

        ad8400_ad8402_ad8403_error_t expected_ret;
        test_digital_output_state_t expected_state;
    } test_case_t;

    ad8400_ad8402_ad8403_t ad8400_ad8402_ad8403[2];
    ad8400_ad8402_ad8403_spi_main_t spi_main[2];
    test_digital_output_t digital_output[2];
    for (size_t i = 0; i < 2; i++)
    {
        test_digital_output_init(&digital_output[i]);
        ad8400_ad8402_ad8403_error_t err = ad8400_ad8402_ad8403_init(&ad8400_ad8402_ad8403[i], &spi_main[i], (ad8400_ad8402_ad8403_digital_output_t *)&digital_output[i]);
        assert(err == AD8400_AD8402_AD8403_SUCCESS);
    }

    test_case_t test_cases[] = {{.ad8400_ad8402_ad8403 = NULL, /*               */ .digital_output = &digital_output[0], .expected_ret = AD8400_AD8402_AD8403_ERROR_NULL_POINTER},
                                {.ad8400_ad8402_ad8403 = &ad8400_ad8402_ad8403[1], .digital_output = &digital_output[1], .expected_ret = AD8400_AD8402_AD8403_SUCCESS, .expected_state = TEST_DIGITAL_OUTPUT_HIGH}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ad8400_ad8402_ad8403_error_t actual_ret = ad8400_ad8402_ad8403_enter_operational_mode(test_case->ad8400_ad8402_ad8403);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
        else if (actual_ret != 0)
        {
            continue;
        }

        test_digital_output_state_t actual_state = test_case->digital_output->state;
        if (test_case->expected_state != actual_state)
        {
            fprintf(stderr, "%sindex: %d, expected_state: %d, actual_state: %d%s\n", TEST_TEXT_RED, i, test_case->expected_state, actual_state, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
    }

    return test_failure_count;
}

int main(void)
{
    int test_failure_count = 0;

    test_failure_count += test_ad8400_ad8402_ad8403_init();
    test_failure_count += test_ad8400_ad8402_ad8403_rdac1_set();
    test_failure_count += test_ad8400_ad8402_ad8403_rdac2_set();
    test_failure_count += test_ad8400_ad8402_ad8403_rdac3_set();
    test_failure_count += test_ad8400_ad8402_ad8403_rdac4_set();
    test_failure_count += test_ad8400_ad8402_ad8403_enter_shutdown_mode();
    test_failure_count += test_ad8400_ad8402_ad8403_enter_operational_mode();

    if (test_failure_count == 0)
    {
        printf("%sOK%s\n", TEST_TEXT_GREEN, TEST_TEXT_RESET);
        return 0;
    }
    else
    {
        fprintf(stderr, "%s%d errors found%s\n", TEST_TEXT_RED, test_failure_count, TEST_TEXT_RESET);
        return 1;
    }
}