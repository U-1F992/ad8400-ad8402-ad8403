#!/bin/bash

TEMPLATE='#ifndef AD8400_AD8402_AD8403{{PREFIX_GUARD}}_H
#define AD8400_AD8402_AD8403{{PREFIX_GUARD}}_H

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

#ifndef {{PREFIX}}_spi_write
#define {{PREFIX}}_spi_write(buffer, count) do { } while (0)
#endif /* {{PREFIX}}_spi_write */

#ifndef {{PREFIX}}_gpio_high
#define {{PREFIX}}_gpio_high() do { } while (0)
#endif /* {{PREFIX}}_gpio_high */

#ifndef {{PREFIX}}_gpio_low
#define {{PREFIX}}_gpio_low() do { } while (0)
#endif /* {{PREFIX}}_gpio_low */

#define {{PREFIX}}_set(address, data) \
    do { \
        unsigned char {{PREFIX}}_internal_buffer[2] = {(unsigned char)(address), (unsigned char)(data)}; \
        {{PREFIX}}_spi_write({{PREFIX}}_internal_buffer, (size_t)2); \
    } while (0)

#define {{PREFIX}}_shutdown() {{PREFIX}}_gpio_low()

#define {{PREFIX}}_startup() {{PREFIX}}_gpio_high()

#ifdef __cplusplus
}
#endif

#endif /* AD8400_AD8402_AD8403{{PREFIX_GUARD}}_H */'

if [ $# -lt 1 ]; then
    PREFIX="ad8400_ad8402_ad8403"
    PREFIX_GUARD=""
else
    PREFIX="$1"
    PREFIX_GUARD="_${1^^}"
fi

OUTPUT=${TEMPLATE//\{\{PREFIX\}\}/$PREFIX}
OUTPUT=${OUTPUT//\{\{PREFIX_GUARD\}\}/$PREFIX_GUARD}
echo "$OUTPUT"
