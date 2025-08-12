#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "busScan.h"


// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart0
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5


//Define the Values for the DRV8830

#define DRV8830_I2CADDR_DEFAULT_WRITE 0xC0 /// DEFAULT I2C address IF PINS A0 AND A1 ARE PULLED HIGH
#define DRV8830_I2CADDR_DEFAULT_READ 0xC1 /// DEFAULT I2C address IF PINS A0 AND A1 ARE PULLED HIGH

#define DRV8830_I2CADDR_PIMORONI_WRITE 0x60 ///  The Pimoroni DRV8830 has A0 and A1 pins to Gnd setting Address to 0xC0
#define DRV8830_I2CADDR_PIMORONI_READ 0x61 
#define DRV8830_CONTROL 0x00         /// Control register
#define DRV8830_FAULT 0x01           /// Fault Condition register

#define DRV8830_FAULT_ILIMIT 0x10    /// Current limiting
#define DRV8830_FAULT_OVERTEMP 0x08  /// Over Temperature Shutdown
#define DRV8830_FAULT_UVFLOW 0x04    /// Undervoltage lockout
#define DRV830_FAULT_OVERCURRENT 0x02 /// Overcurrent protection

/** Motor direction **/
typedef enum _drv8830_dir_t {
RELEASE = 0,
BACKWARD = 1,
FORWARD = 2,
BRAKE = 3,
} DRV8830_Direction;

void setDirection()
{

    //Clear the Faults
    //Write 0x80 to register 0x01

    uint8_t clearFaults = 0x80;
    i2c_write_blocking(i2c0, 0x01, &clearFaults,1, false );

    //Set Commanded output voltage to 0x3D representing 4.9 volts
    uint8_t CommandVoltage = 0x3D;
    //Shift 2 places to left and set the right most bits to 1
    CommandVoltage = CommandVoltage << 2;
    CommandVoltage = CommandVoltage | 2;
   

    uint8_t commandVoltagerBuffer[] = {CommandVoltage};
    
    i2c_write_blocking(i2c0,  0x00 , &CommandVoltage, 1, false );
   

   //printf("Commanded voltage value = %x\n", readControlVoltageValueBuffer[0]);

}



int getFaults()
{

    //We want to get the status of the faults so need to shift the default address to left one place

    
    
    uint8_t faultCommandBuffer[] = {0x80};
    uint8_t faultReadBuffer[] = {0};
    i2c_write_blocking(i2c0, DRV8830_I2CADDR_PIMORONI_WRITE , faultCommandBuffer, 1, false );
    i2c_read_blocking(i2c0,  DRV8830_I2CADDR_PIMORONI_READ , faultReadBuffer,1, true);
    
    printf("Errors = %X\n", faultReadBuffer[0]);

    return 0;


}

void clearFaults()
{
     uint8_t faultCommandBuffer[] = {0x1};
    i2c_write_blocking(i2c0,  DRV8830_I2CADDR_PIMORONI_WRITE , faultCommandBuffer, 1, false );

}






int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART
    
    // Send out a string, with CR/LF conversions
    uart_puts(UART_ID, " Hello, UART!\n");

    
    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart



    i2cScan();

    while (true) {

        setDirection();
        sleep_ms(10000);


    




        
    }
}
