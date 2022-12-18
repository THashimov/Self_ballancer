#include "headers/rcc.h"
#include "headers/gpio.h"
#include "headers/lcd.h"

#define SDA_PIN 7
#define SCL_PIN 6
#define AF_MODE 4
#define I2C_FREQ 16

int main(void) {
	// Init RCC gpio banks and initialise SDL and SDA pins
	init_rcc_gpio(gpio_port_b);
    init_gpio(gpio_port_b, gpio_mode_alt, gpio_type_open_drain, gpio_speed_high, gpio_pupdr_pull_up, SDA_PIN);
    init_gpio(gpio_port_b, gpio_mode_alt, gpio_type_open_drain, gpio_speed_high, gpio_pupdr_pull_up, SCL_PIN);

	// // Init I2C on RCC
    init_rcc_apb1enr_i2c(1);

	// // Assign the correct alternate function to SDA and SCL
    set_alt_function(gpio_port_b, SDA_PIN, AF_MODE);
    set_alt_function(gpio_port_b, SCL_PIN, AF_MODE);
	
	init_lcd_i2c(I2C_FREQ);

    init_gpio(gpio_port_b, gpio_mode_output, gpio_type_push_pull, gpio_speed_high, gpio_pupdr_pull_up, 3);
    // init_gpio(gpio_port_b, gpio_mode_output, gpio_type_push_pull, gpio_speed_high, gpio_pupdr_pull_up, SCL_PIN);
    
	int delay = 500000;

	send_start();
	send_slave_addr(0x3F);
	
	while (1) {
	}
	
}
