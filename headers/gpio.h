#include "enums.h"

#define GPIO_MODE_OFFSET 0
#define GPIO_TYPE_OFFSET 0x04
#define GPIO_SPEED_OFFSET 0x08
#define GPIO_PUPDR_OFFSET 0x0C
#define GPIO_ODR_OFFSET 0x14
#define GPIO_BSRR_OFFSET 0x18
#define GPIO_ALT_LOW_OFFSET 0x20
#define GPIO_ALT_HIGH_OFFSET 0x24

/// Set any gpio registers that use 2 bits
/// Pass in a pointer to the register we want to modify
/// Clear the 2 bits using &= 3 (0b11)
/// Multiply the pin number by 2 because we need to left shift twice for each pin number as we are writing 2 bits
void gpio_set_two_bit_regs(volatile int *gpio_ptr, unsigned char mode, unsigned char pin_num) {
	*gpio_ptr &= ~(3 << (pin_num * 2));
	*gpio_ptr |= (mode << (pin_num * 2));
}

void gpio_set_type(int gpio_port, unsigned char type, unsigned char pin_num) {
	volatile int *gpio_type_ptr = (int *) (gpio_port + GPIO_TYPE_OFFSET);
	*gpio_type_ptr &= ~(1 << pin_num);
	*gpio_type_ptr |= (type << pin_num);
}

/// First we create a pointer to the mode register and call the function to set 2 bits
/// We can do this for all 3 registers that use a 2 bit value
/// Then we can call set type as it only writes one bit
void init_gpio(int gpio_port, unsigned char gpio_mode, unsigned char gpio_type, unsigned char gpio_speed, unsigned char gpio_pupdr, unsigned char pin_num) {
	volatile int *gpio_mode_ptr = (int *) (gpio_port + GPIO_MODE_OFFSET);
	volatile int *gpio_speed_ptr = (int *) (gpio_port + GPIO_SPEED_OFFSET);
	volatile int *gpio_pupdr_ptr = (int *) (gpio_port + GPIO_PUPDR_OFFSET);
	gpio_set_two_bit_regs(gpio_mode_ptr, gpio_mode, pin_num);
	gpio_set_two_bit_regs(gpio_speed_ptr, gpio_speed, pin_num);
	gpio_set_two_bit_regs(gpio_pupdr_ptr, gpio_pupdr, pin_num);
	gpio_set_type(gpio_port, gpio_type, pin_num);
}

void turn_pin_on(int gpio_port, unsigned char pin_num) {
	int volatile * const gpio_odr = (int *) (gpio_port + GPIO_ODR_OFFSET);
	*gpio_odr |= (1 << pin_num);
}

/// To turn off a pin, we will be writing to the top 16 bits
/// To ensure we don't write to the lower half we can left shift by 16 + pin_num
void turn_pin_off(int gpio_port, unsigned char pin_num) {
	int volatile * const gpio_bsrr = (int *) (gpio_port + GPIO_BSRR_OFFSET);
	*gpio_bsrr |= (1 << (16 + pin_num));
}

// Set alt function mode. Each pin take a nibble
// Each AF mode corresponds to a nibble 
// AF0 = 0b0000
// AF2 = 0b0010 etc
// First we clear the nibble
// Then write the AF nibble to register
void set_alt_function(int gpio_port, unsigned char pin_num, unsigned char af_num) {
	if (pin_num > 7) {
		int volatile * const gpio_alt_high = (int *) (gpio_port + GPIO_ALT_HIGH_OFFSET);
		*gpio_alt_high &= ~((af_num) << (pin_num * 4));
    	*gpio_alt_high |= (af_num) << (pin_num * 4);
	} else {
		int volatile * const gpio_alt_low = (int *) (gpio_port + GPIO_ALT_LOW_OFFSET);
		*gpio_alt_low &= ~((af_num) << (pin_num * 4));
    	*gpio_alt_low |= (af_num) << (pin_num * 4);
	}
}