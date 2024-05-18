#include <ad8400_ad8402_ad8403.h>

#include <assert.h>

ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_spi_main_transmit(ad8400_ad8402_ad8403_spi_main_t *spi_main, uint8_t data[], size_t size)
{
    if (spi_main == NULL)
    {
        return AD8400_AD8402_AD8403_ERROR_NULL_POINTER;
    }

    return spi_main->transmit(spi_main, data, size);
}

ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_digital_output_set_high(ad8400_ad8402_ad8403_digital_output_t *digital_output)
{
    if (digital_output == NULL)
    {
        return AD8400_AD8402_AD8403_ERROR_NULL_POINTER;
    }

    return digital_output->set_high(digital_output);
}

ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_digital_output_set_low(ad8400_ad8402_ad8403_digital_output_t *digital_output)
{
    if (digital_output == NULL)
    {
        return AD8400_AD8402_AD8403_ERROR_NULL_POINTER;
    }

    return digital_output->set_low(digital_output);
}

ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_init(ad8400_ad8402_ad8403_t *ad8400_ad8402_ad8403, ad8400_ad8402_ad8403_spi_main_t *spi_main, ad8400_ad8402_ad8403_digital_output_t *shdn)
{
    if (ad8400_ad8402_ad8403 == NULL ||
        spi_main == NULL)
    {
        return AD8400_AD8402_AD8403_ERROR_NULL_POINTER;
    }

    ad8400_ad8402_ad8403->spi_main = spi_main;
    ad8400_ad8402_ad8403->shdn = shdn;

    return AD8400_AD8402_AD8403_SUCCESS;
}

typedef enum ad8400_ad8402_ad8403_address_t
{
    AD8400_AD8402_AD8403_RDAC1,
    AD8400_AD8402_AD8403_RDAC2,
    AD8400_AD8402_AD8403_RDAC3,
    AD8400_AD8402_AD8403_RDAC4
} ad8400_ad8402_ad8403_address_t;

static ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_set(ad8400_ad8402_ad8403_t *ad8400_ad8402_ad8403, ad8400_ad8402_ad8403_address_t addr, uint8_t data)
{
    assert(addr == AD8400_AD8402_AD8403_RDAC1 ||
           addr == AD8400_AD8402_AD8403_RDAC2 ||
           addr == AD8400_AD8402_AD8403_RDAC3 ||
           addr == AD8400_AD8402_AD8403_RDAC4);

    if (ad8400_ad8402_ad8403 == NULL)
    {
        return AD8400_AD8402_AD8403_ERROR_NULL_POINTER;
    }

    uint8_t data_[] = {(uint8_t)addr, //
                       data};
    ad8400_ad8402_ad8403_error_t err = ad8400_ad8402_ad8403_spi_main_transmit(ad8400_ad8402_ad8403->spi_main, //
                                                                              data_,                          //
                                                                              2);
    if (err != AD8400_AD8402_AD8403_SUCCESS)
    {
        return AD8400_AD8402_AD8403_ERROR_I2C_FAILURE;
    }

    return AD8400_AD8402_AD8403_SUCCESS;
}

ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_rdac1_set(ad8400_ad8402_ad8403_t *ad8400_ad8402_ad8403, uint8_t data) { return ad8400_ad8402_ad8403_set(ad8400_ad8402_ad8403, AD8400_AD8402_AD8403_RDAC1, data); }
ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_rdac2_set(ad8400_ad8402_ad8403_t *ad8400_ad8402_ad8403, uint8_t data) { return ad8400_ad8402_ad8403_set(ad8400_ad8402_ad8403, AD8400_AD8402_AD8403_RDAC2, data); }
ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_rdac3_set(ad8400_ad8402_ad8403_t *ad8400_ad8402_ad8403, uint8_t data) { return ad8400_ad8402_ad8403_set(ad8400_ad8402_ad8403, AD8400_AD8402_AD8403_RDAC3, data); }
ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_rdac4_set(ad8400_ad8402_ad8403_t *ad8400_ad8402_ad8403, uint8_t data) { return ad8400_ad8402_ad8403_set(ad8400_ad8402_ad8403, AD8400_AD8402_AD8403_RDAC4, data); }

ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_enter_shutdown_mode(ad8400_ad8402_ad8403_t *ad8400_ad8402_ad8403)
{
    if (ad8400_ad8402_ad8403 == NULL)
    {
        return AD8400_AD8402_AD8403_ERROR_NULL_POINTER;
    }

    ad8400_ad8402_ad8403_error_t err = ad8400_ad8402_ad8403_digital_output_set_low(ad8400_ad8402_ad8403->shdn);
    if (err != AD8400_AD8402_AD8403_SUCCESS &&
        err != AD8400_AD8402_AD8403_ERROR_NULL_POINTER)
    {
        return AD8400_AD8402_AD8403_ERROR_DIGITAL_OUTPUT_FAILURE;
    }

    return AD8400_AD8402_AD8403_SUCCESS;
}

ad8400_ad8402_ad8403_error_t ad8400_ad8402_ad8403_enter_operational_mode(ad8400_ad8402_ad8403_t *ad8400_ad8402_ad8403)
{
    if (ad8400_ad8402_ad8403 == NULL)
    {
        return AD8400_AD8402_AD8403_ERROR_NULL_POINTER;
    }

    ad8400_ad8402_ad8403_error_t err = ad8400_ad8402_ad8403_digital_output_set_high(ad8400_ad8402_ad8403->shdn);
    if (err != AD8400_AD8402_AD8403_SUCCESS &&
        err != AD8400_AD8402_AD8403_ERROR_NULL_POINTER)
    {
        return AD8400_AD8402_AD8403_ERROR_DIGITAL_OUTPUT_FAILURE;
    }

    return AD8400_AD8402_AD8403_SUCCESS;
}