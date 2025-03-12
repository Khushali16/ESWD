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