// switch light on and off with time delay of 5 seconds


#define LED_PIN 13  // Arduino Uno's onboard LED

void setup() {
    DDRB |= (1 << PB4); // Set pin 13 as output
}

void loop() {
    // Turn LED ON
    PORTB |= (1 << PB4);  
    delayLoop(); // Custom delay
    
    // Turn LED OFF
    PORTB &= ~(1 << PB4);
    delayLoop(); // Custom delay
}
// Custom software delay (replaces delay() function)
void delayLoop() {
    for (volatile long i = 0; i < 500000; i++); // Adjust for Wokwi speed
}


======
// using SPI Bigbang

#define MOSI_PIN 11  // Master Out Slave In pin
#define SCK_PIN 13   // Serial Clock pin
#define SS_PIN 10    // Slave Select pin

void setup() {
    // Set MOSI, SCK, and SS as output pins
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2);  // PB3(MOSI), PB5(SCK), PB2(SS)
    
    // Optionally set SS pin high to disable the SPI slave
    PORTB |= (1 << PB2);
}

void loop() {
    // Start SPI communication (manually drive SS low)
    PORTB &= ~(1 << PB2);  // Set SS low to select the slave
    
    // Send a byte via SPI (MOSI and SCK)
    sendSPI(0xAA);  // Sending a test byte (0xAA)
    
    // Stop SPI communication (set SS high)
    PORTB |= (1 << PB2);  // Set SS high to deselect the slave
    
    delayLoop();  // Custom delay
    
    // Turn LED ON to indicate operation (optional for debugging)
    PORTB |= (1 << PB5);  
    delayLoop();  // Custom delay
    
    // Turn LED OFF
    PORTB &= ~(1 << PB5);  
    delayLoop();  // Custom delay
}

// Custom software delay (replaces delay() function)
void delayLoop() {
    for (volatile long i = 0; i < 500000; i++); // Adjust for Wokwi speed
}

// Bit-banging SPI function to send a byte
void sendSPI(uint8_t data) {
    for (uint8_t bit = 0; bit < 8; bit++) {
        // Write the bit to the MOSI pin
        if (data & (1 << (7 - bit))) {
            PORTB |= (1 << PB3);  // Set MOSI high
        } else {
            PORTB &= ~(1 << PB3);  // Set MOSI low
        }

        // Pulse the clock
        PORTB |= (1 << PB5);  // Set SCK high
        PORTB &= ~(1 << PB5); // Set SCK low
    }
}
