#include <Wire.h>

// Function to initialize UART manually
void UART_Init() {
    unsigned int ubrr = 103; // Baud rate setting for 9600 baud
    UBRR0H = (unsigned char)(ubrr >> 8);  // High byte of the baud rate
    UBRR0L = (unsigned char)ubrr;  // Low byte of the baud rate
    UCSR0B = (1 << TXEN0);  // Enable the transmitter
    UCSR0C = (3 << UCSZ00); // 8-bit data format
    
    Serial.begin(9600);  // Initialize Serial for the monitor
}

// Function to send a character over UART manually
void UART_Transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer
    UDR0 = data; // Load data into the UART data register
}

// Enable or disable writing to memory
unsigned char memory[128];
unsigned char writeEnabled = 0;

void sendWriteEnable() { writeEnabled = 1; }
void sendWriteDisable() { writeEnabled = 0; }

// Function to write a byte into memory
void writeByte(unsigned char address, unsigned char data) {
    if (writeEnabled && address < 128) {
        memory[address] = data;
    }
}

// Function to read a byte from memory
unsigned char readByte(unsigned char address) {
    if (address < 128) return memory[address];
    return 0;
}

// Function to print characters on Serial Monitor
void printChar(unsigned char c) {
    // Display on UART via UART_Transmit
    if (c < 32 || c == 127) {  // Non-printable characters
        UART_Transmit('[');
        
        // Print only as two digits for numbers below 32 or 127
        if (c < 10) {
            UART_Transmit('0');
        }
        
        UART_Transmit(c + '0');  // Print the single character
        
        UART_Transmit(']');
    } else { // Printable characters
        UART_Transmit(c);
    }
    
    delay(300); // Small delay for readability on Serial Monitor
}

void setup() {
    UART_Init();  // Initialize UART communication

    // Initialize memory
    for (int i = 0; i < 128; i++) {
        memory[i] = 0xFF;
    }

    sendWriteEnable();
    for (unsigned char addr = 0; addr < 128; addr++) {
        writeByte(addr, addr); // Store ASCII values
    }
    sendWriteDisable();
}

void loop() {
    for (unsigned char addr = 0; addr < 128; addr++) {
        printChar(readByte(addr)); // Read and print characters on Serial Monitor
    }
    UART_Transmit('\n'); // Send newline after the loop completes
}
