# [AD8400](https://www.analog.com/products/ad8400.html)/[AD8402](https://www.analog.com/products/ad8402.html)/[AD8403](https://www.analog.com/products/ad8403.html)

```c
typedef struct spi_main_t
{
    ad8400_ad8402_ad8403_spi_main_t parent;
    /* ... */
} spi_main_t;

ad8400_ad8402_ad8403_error_t spi_main_transmit(ad8400_ad8402_ad8403_spi_main_t *parent, uint8_t data[], size_t size)
{
    spi_main_t *spi_main = (spi_main_t *)parent;
    /* ... */
    return AD8400_AD8402_AD8403_SUCCESS;
}

void spi_main_init(spi_main_t *spi_main)
{
    spi_main->parent.transmit = spi_main_transmit;
    /* ... */
}

typedef struct digital_output_t
{
    ad8400_ad8402_ad8403_digital_output_t parent;
    /* ... */
} digital_output_t;

ad8400_ad8402_ad8403_error_t digital_output_set_low(ad8400_ad8402_ad8403_digital_output_t *parent)
{
    digital_output_t *digital_output = (digital_output_t *)parent;
    /* ... */
    return AD8400_AD8402_AD8403_SUCCESS;
}

ad8400_ad8402_ad8403_error_t digital_output_set_high(ad8400_ad8402_ad8403_digital_output_t *parent)
{
    digital_output_t *digital_output = (digital_output_t *)parent;
    /* ... */
    return AD8400_AD8402_AD8403_SUCCESS;
}

void digital_output_init(digital_output_t *digital_output)
{
    digital_output->parent.set_low = digital_output_set_low;
    digital_output->parent.set_high = digital_output_set_high;
    /* ... */
}

int main(void)
{
    spi_main_t spi_main;
    spi_main_init(&spi_main);

    digital_output_t shdn;
    digital_output_init(&shdn);

    ad8400_ad8402_ad8403_t ad8400_ad8402_ad8403;
    ad8400_ad8402_ad8403_init(&ad8400_ad8402_ad8403, (ad8400_ad8402_ad8403_spi_main_t *)&spi_main, (ad8400_ad8402_ad8403_digital_output_t *)&shdn);

    /* ... */
}
```