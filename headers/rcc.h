#define PORT_A 0x40020000
#define PORT_B 0x40020400
#define PORT_C 0x40020800

#define RCC_ADDR 0x40023800

#define AHB1ENR 0x30
#define APB1ENR 0x40

#define I2C1 21
#define I2C2 22
#define I2C3 23

void init_rcc_gpio(int gpio_port) {
	int volatile * const rcc_ahb1enr = (int *) (RCC_ADDR + AHB1ENR);
	if (gpio_port == PORT_A) {
		*rcc_ahb1enr |= 1; 
	} else if (gpio_port == PORT_B) {
		*rcc_ahb1enr |= (1 << 1); 
	} else if (gpio_port == PORT_C) {
		*rcc_ahb1enr |= (1 << 2); 
	}
}


void init_rcc_apb1enr_i2c(unsigned char i2c_num) {
	int volatile * const rcc_apb1enr = (int *) (RCC_ADDR + APB1ENR);
	if (i2c_num == 1)
	{
		*rcc_apb1enr |= (1 << I2C1);
	} else if (i2c_num == 2)
	{
		*rcc_apb1enr |= (1 << I2C2);
	} else {
		*rcc_apb1enr |= (1 << I2C3);
	}
}