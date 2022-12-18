enum GpioPort {
	gpio_port_a = 0x40020000,
	gpio_port_b = 0x40020400,
	gpio_port_c = 0x40020800
};

enum GpioMode {
	gpio_mode_input = 0,
	gpio_mode_output = 1,
	gpio_mode_alt = 2,
	gpio_mode_analog = 3,
};

enum GpioType {
	gpio_type_push_pull = 0,
	gpio_type_open_drain = 1
};

enum GpioSpeed {
	gpio_speed_low = 0,
	gpio_speed_medium = 1,
	gpio_speed_high = 2,
	gpio_speed_very_high = 3
};

enum GpioPupdr {
	gpio_pupdr_no_pull = 0,
	gpio_pupdr_pull_up = 1,
	gpio_pupdr_pull_down = 2
};	