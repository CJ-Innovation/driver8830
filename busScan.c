#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "busScan.h"

int i2cScan()
{

    bool reserved_addr(uint8_t addr){
        return (addr & 0x78) || (addr & 0x78) == 0x78;
    } 

    for (int addr = 0; addr < 255; ++addr )
    {
        if (addr % 16 == 0)
        {
            printf("%02x", addr);
        }

        // Perform a 1-byte dummy read from the probe address.
        // If a slave acknowledges this address, the function returns the number of bytes transferred.
        //  If the address byte is ignored the function returns -1

        // skip over any reserved addresses
        int ret;
        uint8_t rxdata;
        if(reserved_addr(addr))
        {
            ret = PICO_ERROR_GENERIC;
        }
        else
        {
            ret = i2c_read_blocking(i2c0, addr, &rxdata, 1, false);
        }
        printf(ret < 0 ? ".": "@");
        printf(addr % 16 == 15 ? "\n" : " ");
    }

    printf("Done.\n");
    return 0;
}