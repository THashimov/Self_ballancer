#define I2C1_ADDR 0x40005400 
#define I2C2_ADDR 0x40005800 
#define I2C3_ADDR 0x40005C00 

#define I2C_CR2_OFFSET 0x4
#define I2C_CR1_OFFSET 0
#define I2C_CCR_OFFSET 0x1C
#define I2C_TRISE_OFFSET 0x20
#define I2C_SR1_OFFSET 0x14
#define I2C_SR2_OFFSET 0x18
#define I2C_DR_OFFSET 0x10

/// This might be where we set the khz. possibly need to change to 100
#define SCL 1000
#define SCHL 4000

#define ACK 10
#define STOP 9
#define START 8bob
bob

#define TXE 7
#define BTF 2

/// Sequence as follows
/// Start i2c
/// Send addr of slave
/// send addr of reg (where we want to write data to)
/// send the data
/// stop i2c

static int volatile * const i2c_cr1 = (int *) (I2C1_ADDR + I2C_CR1_OFFSET);
static int volatile * const i2c_cr2 = (int *) (I2C1_ADDR + I2C_CR2_OFFSET);
static int volatile * const i2c_ccr = (int *) (I2C1_ADDR + I2C_CCR_OFFSET);
static int volatile * const i2c_trise = (int *) (I2C1_ADDR + I2C_TRISE_OFFSET);
static int volatile * const i2c_sr1 = (int *) (I2C1_ADDR + I2C_SR1_OFFSET);
static int volatile * const i2c_sr2 = (int *) (I2C1_ADDR + I2C_SR2_OFFSET);
static int volatile * const i2c_dr = (int *) (I2C1_ADDR + I2C_DR_OFFSET);

float calc_tpclk1(unsigned char freq) {
    return (1 / freq) * 1000;
}

int calc_trise(float tpclk) {
    return (SCL / tpclk) + 1;
}

/// I2C must be disabled during setup
/// PE is bit 0
void disable_i2c() {
    *i2c_cr1 &= ~(1 << 0);
}

void enable_i2c() {
    *i2c_cr1 |= (1 << 0);
}

/// Reset the i2c before use by writing a 1 to bit 15 then a 0 to release from reset state
void reset_i2c() {
    *i2c_cr1 |= (1 << 15);
    *i2c_cr1 &= ~(1 << 15);
}


/// The MHz value is the same as the decimal value. 
/// To set the frequency to 42MHz, we write a 42 into the lower 5 bits
/// First clear the lower 5 bits 
void set_i2c_freq(unsigned char freq) {
    *i2c_cr2 &= ~(0x1F);
    *i2c_cr2 |= freq;
}

/// Set ccr to SM mode by writing a 0 to bit 15
/// Clear the lower 11 bits
/// Write the ccr_val into the lowest bits
void init_i2c_ccr(int ccr) {
    *i2c_ccr &= ~(1 << 15);
    *i2c_ccr &= ~(0xFFF);
    *i2c_ccr |= ccr;
}

void set_trise(unsigned char trise) {
    *i2c_trise &= ~(0x1F);
    *i2c_trise |= trise;
}

void init_i2c(unsigned char freq) {
    reset_i2c();
    set_i2c_freq(freq);
    float tpclk1 = calc_tpclk1(freq);
    int ccr = (SCL + SCHL) / tpclk1;
    disable_i2c();
    init_i2c_ccr(ccr);
    char trise = calc_trise(tpclk1);
    set_trise(trise);
    enable_i2c();
}

void addr_sent() {
    if ((*i2c_sr1 & (1 << 1)))
    {
		turn_pin_on(gpio_port_b, 3);
    }
}

void send_start() {
    *i2c_cr1 |= (1 << START);
}

void send_slave_addr(char addr) {
    *i2c_dr |= (addr << 1);
}


/// Wait for TXE to be set, this indicates that the DR is ready for data
/// Send data to DR
/// Wait for BTF to be set, this indicates the end of data transmission
// void i2c_write(unsigned char byte_to_write) {
// 	int volatile * const i2c_sr1 = (int *) (I2C1_ADDR + I2C_SR1_OFFSET);
// 	int volatile * const i2c_dr = (int *) (I2C1_ADDR + I2C_DR_OFFSET);
//     while (!(*i2c_sr1 & (1 << TXE)));
//     *i2c_dr &= ~(0xFF);
//     *i2c_dr |= byte_to_write;
//     while (!(*i2c_sr1 & (1 << BTF)));
// }


// cr1 bit 10 is ack
// cr1 bit 9 is stop
// cr1 bit 8 is start
// cr1 bit 0 is pe

// send start
// sb=1 cleared by reading sr1 then writing to dr
// addr
// acknowldge bit
// addr = 1. cleared by reading sr1 then sr2
// data1
// ack bit
// rxne cleared by reading dr
// send data2
// stop bit