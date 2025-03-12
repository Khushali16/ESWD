

unsigned char memory[128];
unsigned char writeEnabled = 0;

// Function to initialize UART manually
void UART_Init() {
    unsigned int ubrr = 103;
    UCSR0B = (1 << TXEN0);  // Enable transmitter
    UCSR0C = (3 << UCSZ00);  // 8-bit data format
}

// Function to send a character over UART manually
void UART_Transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer
    UDR0 = data; // Load data into register
}


void sendWriteEnable() { writeEnabled = 1; }
void sendWriteDisable() { writeEnabled = 0; }

void writeByte(unsigned char address, unsigned char data) {
    if (writeEnabled && address < 128) {
        memory[address] = data;
    }
}

unsigned char readByte(unsigned char address) {
    if (address < 128) return memory[address];
    return 0;
}

void printChar(unsigned char c) {
    if (c >= 32 && c <= 126) {
        UART_Transmit(c);  // Print ASCII character
    } else {
        UART_Transmit('[');
        if (c >= 100) UART_Transmit('0' + (c / 100));
        if (c >= 10) UART_Transmit('0' + ((c / 10) % 10));
        UART_Transmit('0' + (c % 10));
        UART_Transmit(']');
    }
}

int main(void) {
    UART_Init(); // Initialize UART with 9600 baud rate

    // Initialize memory
    for (int i = 0; i < 128; i++) {
        memory[i] = 0xFF;
    }
    
    sendWriteEnable();
    for (unsigned char addr = 0; addr < 128; addr++) {
        writeByte(addr, addr); // Write ASCII 0-127
    }
    sendWriteDisable();

    for (unsigned char addr = 0; addr < 128; addr++) {
        printChar(readByte(addr));
    }
    
    UART_Transmit('\n');
    return 0;
}


======
// SPI Bitbanging program

#define MOSI_PIN 11    // Master Out Slave In pin (PB3)
#define SCK_PIN 13     // Serial Clock pin (PB5)
#define SS_PIN 10      // Slave Select pin (PB2)
#define MEMORY_SIZE 128

unsigned char memory[MEMORY_SIZE];
unsigned char writeEnabled = 0;

// Bit-banging SPI function to send a byte
void sendSPI(unsigned char data) {
    for (unsigned char bit = 0; bit < 8; bit++) {
        // Write the bit to the MOSI pin
        if (data & (1 << (7 - bit))) {
            PORTB |= (1 << PB3);  // Set MOSI high
        } else {
            PORTB &= ~(1 << PB3); // Set MOSI low
        }

        // Pulse the clock (SCK)
        PORTB |= (1 << PB5);  // Set SCK high
        PORTB &= ~(1 << PB5); // Set SCK low
    }
}

// Function to initialize the ports for bit-banging SPI
void SPI_Init() {
    // Set MOSI, SCK, and SS as output pins
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2);  // PB3 (MOSI), PB5 (SCK), PB2 (SS)

    // Set SS pin high initially (deselect the slave)
    PORTB |= (1 << PB2);
}

// Function to enable write mode (simulating chip select)
void sendWriteEnable() {
    writeEnabled = 1;
    PORTB &= ~(1 << PB2);  // Set SS low to select the slave
}

// Function to disable write mode
void sendWriteDisable() {
    writeEnabled = 0;
    PORTB |= (1 << PB2);  // Set SS high to deselect the slave
}

// Function to write a byte to memory
void writeByte(unsigned char address, unsigned char data) {
    if (writeEnabled && address < MEMORY_SIZE) {
        memory[address] = data;
        // Send the write command and data via SPI
        sendSPI(address);  // Send the address
        sendSPI(data);     // Send the data
    }
}

// Function to read a byte from memory
unsigned char readByte(unsigned char address) {
    if (address < MEMORY_SIZE) {
        // Send the read command and address via SPI
        sendSPI(address);  // Send the address
        return memory[address];  // Return the memory content
    }
    return 0;
}

// Function to transmit a character over SPI (simulating UART)
void printChar(unsigned char c) {
    if (c >= 32 && c <= 126) {
        sendSPI(c);  // Print ASCII character
    } else {
        sendSPI('[');
        if (c >= 100) sendSPI('0' + (c / 100));
        if (c >= 10) sendSPI('0' + ((c / 10) % 10));
        sendSPI('0' + (c % 10));
        sendSPI(']');
    }
}

int main(void) {
    // Initialize SPI and memory
    SPI_Init();
    
    // Initialize memory
    for (unsigned char i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = 0xFF;
    }

    // Enable write mode
    sendWriteEnable();
    
    // Write data (address 0 to 127 with values 0 to 127)
    for (unsigned char addr = 0; addr < MEMORY_SIZE; addr++) {
        writeByte(addr, addr); // Write ASCII 0-127
    }

    // Disable write mode
    sendWriteDisable();

    // Read and print data from memory
    for (unsigned char addr = 0; addr < MEMORY_SIZE; addr++) {
        printChar(readByte(addr));
    }
    
    sendSPI('\n');  // Send newline character via SPI (simulating UART transmit)
    
    return 0;
}
