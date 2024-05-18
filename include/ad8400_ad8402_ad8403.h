#ifndef AD8400_AD8402_AD8403_H
#define AD8400_AD8402_AD8403_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>

    typedef enum ad8400_ad8402_ad8403_error_t
    {
        AD8400_AD8402_AD8403_SUCCESS,
        AD8400_AD8402_AD8403_ERROR_NULL_POINTER,
        AD8400_AD8402_AD8403_ERROR_I2C_FAILURE,
        AD8400_AD8402_AD8403_ERROR_DIGITAL_OUTPUT_FAILURE
    } ad8400_ad8402_ad8403_error_t;

    /**
     * This naming is derived from the Analog Device's article.
     * https://www.analog.com/resources/analog-dialogue/articles/introduction-to-spi-interface.html
     */
    typedef struct ad8400_ad8402_ad8403_spi_main_t
    {
        ad8400_ad8402_ad8403_error_t (*transmit)(struct ad8400_ad8402_ad8403_spi_main_t *, uint8_t[], size_t);
    } ad8400_ad8402_ad8403_spi_main_t;

    ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_spi_main_transmit(ad8400_ad8402_ad8403_spi_main_t *, uint8_t[], size_t);

    typedef struct ad8400_ad8402_ad8403_digital_output_t
    {
        ad8400_ad8402_ad8403_error_t (*set_high)(struct ad8400_ad8402_ad8403_digital_output_t *);
        ad8400_ad8402_ad8403_error_t (*set_low)(struct ad8400_ad8402_ad8403_digital_output_t *);
    } ad8400_ad8402_ad8403_digital_output_t;

    ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_digital_output_set_high(ad8400_ad8402_ad8403_digital_output_t *);
    ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_digital_output_set_low(ad8400_ad8402_ad8403_digital_output_t *);

    typedef struct ad8400_ad8402_ad8403_t
    {
        ad8400_ad8402_ad8403_spi_main_t *spi_main;
        ad8400_ad8402_ad8403_digital_output_t *shdn;
    } ad8400_ad8402_ad8403_t;

    ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_init(ad8400_ad8402_ad8403_t *, ad8400_ad8402_ad8403_spi_main_t *, ad8400_ad8402_ad8403_digital_output_t *);
    ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_rdac1_set(ad8400_ad8402_ad8403_t *, uint8_t);
    ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_rdac2_set(ad8400_ad8402_ad8403_t *, uint8_t);
    ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_rdac3_set(ad8400_ad8402_ad8403_t *, uint8_t);
    ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_rdac4_set(ad8400_ad8402_ad8403_t *, uint8_t);
    ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_enter_shutdown_mode(ad8400_ad8402_ad8403_t *);
    ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_enter_operational_mode(ad8400_ad8402_ad8403_t *);

#ifdef __cplusplus
}
#endif

#endif /* AD8400_AD8402_AD8403_H */