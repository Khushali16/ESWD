

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
