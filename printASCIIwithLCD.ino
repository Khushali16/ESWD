// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>

// // Set the LCD address (change to 0x3F if needed)
// LiquidCrystal_I2C lcd(0x27, 16, 2);

// void setup() {
//     lcd.init();       // Initialize the LCD
//     lcd.backlight();  // Turn on the LCD backlight
//     Serial.begin(9600);

//     lcd.setCursor(0, 0);
//     lcd.print("ASCII Table:");
// }

// void loop() {
//     for (char c = 32; c <= 126; c++) { // Print ASCII characters from 32 to 126
//         lcd.clear();
//         lcd.setCursor(0, 0);
//         lcd.print("Char: ");
//         lcd.print(c);
        
//         lcd.setCursor(0, 1);
//         lcd.print("ASCII: ");
//         lcd.print((int)c); // Print ASCII value
        
//         Serial.print("Character: ");
//         Serial.print(c);
//         Serial.print(" | ASCII: ");
//         Serial.println((int)c);

//         delay(1000); // Delay for visibility
//     }
// }
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD (I2C Address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned char memory[128];
unsigned char writeEnabled = 0;

// Function to initialize UART manually
void UART_Init() {
    unsigned int ubrr = 103; // Baud rate setting for 9600 baud
    UBRR0H = (unsigned char)(ubrr >> 8);  // High byte of the baud rate
    UBRR0L = (unsigned char)ubrr;  // Low byte of the baud rate
    UCSR0B = (1 << TXEN0);  // Enable the transmitter
    UCSR0C = (3 << UCSZ00); // 8-bit data format
    Serial.begin(9600);  // Also initialize Serial for the monitor
}

// Function to send a character over UART manually
void UART_Transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer
    UDR0 = data; // Load data into the UART data register
}

// Enable or disable writing to memory
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

// Function to display characters in the requested format on LCD & Serial
void printChar(unsigned char c) {
    lcd.clear();
    lcd.setCursor(0, 0);

    // Display on UART via UART_Transmit
    if (c < 32 || c == 127) {  // Non-printable characters
        UART_Transmit('[');
        
        // Print only as two digits for numbers below 32 or 127
        if (c < 10) {
            UART_Transmit('0');
        }
        
        UART_Transmit(c + '0');  // Print the single character
        
        UART_Transmit(']');
        
        lcd.print("[");
        lcd.print(c);
        lcd.print("]");

    } else { // Printable characters
        UART_Transmit(c);
        lcd.print((char)c);
    }
    
    delay(300); // Small delay for readability on both LCD and Serial Monitor
}

void setup() {
    UART_Init();  // Initialize UART communication
    lcd.init();
    lcd.backlight();

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
        printChar(readByte(addr)); // Read and display characters in the correct format
    }
    UART_Transmit('\n'); // Send newline after the loop completes
}

