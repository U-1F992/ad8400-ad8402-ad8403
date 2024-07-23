#ifndef AD8400_AD8402_AD8403_H
#define AD8400_AD8402_AD8403_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef AD8400_AD8402_AD8403_COMMON
#define AD8400_AD8402_AD8403_COMMON

typedef enum ad8400_ad8402_ad8403_address_t {
    AD8400_AD8402_AD8403_RDAC1,
    AD8400_AD8402_AD8403_RDAC2,
    AD8400_AD8402_AD8403_RDAC3,
    AD8400_AD8402_AD8403_RDAC4
} ad8400_ad8402_ad8403_address_t;

#endif /* AD8400_AD8402_AD8403_COMMON */

#ifndef ad8400_ad8402_ad8403_spi_write
#define ad8400_ad8402_ad8403_spi_write(buffer, count) do { } while (0)
#endif /* ad8400_ad8402_ad8403_spi_write */

#ifndef ad8400_ad8402_ad8403_gpio_high
#define ad8400_ad8402_ad8403_gpio_high() do { } while (0)
#endif /* ad8400_ad8402_ad8403_gpio_high */

#ifndef ad8400_ad8402_ad8403_gpio_low
#define ad8400_ad8402_ad8403_gpio_low() do { } while (0)
#endif /* ad8400_ad8402_ad8403_gpio_low */

#define ad8400_ad8402_ad8403_set(address, data) \
    do { \
        unsigned char ad8400_ad8402_ad8403_internal_buffer[2] = {(unsigned char)(address), (unsigned char)(data)}; \
        ad8400_ad8402_ad8403_spi_write(ad8400_ad8402_ad8403_internal_buffer, (size_t)2); \
    } while (0)

#define ad8400_ad8402_ad8403_shutdown() ad8400_ad8402_ad8403_gpio_low()

#define ad8400_ad8402_ad8403_startup() ad8400_ad8402_ad8403_gpio_high()

#ifdef __cplusplus
}
#endif

#endif /* AD8400_AD8402_AD8403_H */
