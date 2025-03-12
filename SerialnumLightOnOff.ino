#define LED_PIN 13  

void setup() {
    // Set LED pin as output
    DDRB |= (1 << PB5);

    // Initialize UART (Serial Communication) Manually
    UBRR0H = 0;
    UBRR0L = 103; // Baud rate 9600 for 16MHz clock
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable RX and TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

char UART_Read() {
    while (!(UCSR0A & (1 << RXC0))); // Wait for data
    return UDR0; // Return received character
}

void loop() {
    char input = UART_Read(); // Read serial input

    if (input == '1') {
        PORTB |= (1 << PB5);  // Turn LED ON
    } else if (input == '0') {
        PORTB &= ~(1 << PB5); // Turn LED OFF
    }
}


=======
// using Bitbanging SPI

#define LED_PIN 13     // Onboard LED pin (PB5)
#define MOSI_PIN 11    // Master Out Slave In pin (PB3)
#define SCK_PIN 13     // Serial Clock pin (PB5)
#define SS_PIN 10      // Slave Select pin (PB2)

void setup() {
    // Set MOSI, SCK, and SS as output pins
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2);  // PB3 (MOSI), PB5 (SCK), PB2 (SS)

    // Set LED pin as output
    DDRB |= (1 << PB5);

    // Initialize UART (Serial Communication) Manually
    UBRR0H = 0;
    UBRR0L = 103; // Baud rate 9600 for 16MHz clock
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable RX and TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

char UART_Read() {
    while (!(UCSR0A & (1 << RXC0))); // Wait for data
    return UDR0; // Return received character
}

// Bit-banging SPI function to send a byte
void sendSPI(uint8_t data) {
    for (uint8_t bit = 0; bit < 8; bit++) {
        // Write the bit to the MOSI pin
        if (data & (1 << (7 - bit))) {
            PORTB |= (1 << PB3);  // Set MOSI high
        } else {
            PORTB &= ~(1 << PB3); // Set MOSI low
        }

        // Pulse the clock
        PORTB |= (1 << PB5);  // Set SCK high
        PORTB &= ~(1 << PB5); // Set SCK low
    }
}

void loop() {
    char input = UART_Read(); // Read serial input

    if (input == '1') {
        PORTB |= (1 << PB5);  // Turn LED ON
        sendSPI(0x01);        // Send a test byte via SPI
    } else if (input == '0') {
        PORTB &= ~(1 << PB5); // Turn LED OFF
        sendSPI(0x00);        // Send a test byte via SPI
    }
}

