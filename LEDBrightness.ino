//LED Brightness
//This code will make the LED connected to PB3 to glow at 50% brightness.

#define TCCR0A (*(volatile unsigned char*)0x44)
#define TCCR0B (*(volatile unsigned char*)0x45)
#define OCR0A  (*(volatile unsigned char*)0x47)
#define DDRB   (*(volatile unsigned char*)0x24)

void PWM_Init() {
    DDRB |= (1 << 3); // PB3 as output
    TCCR0A = (1 << 7) | (1 << 1) | (1 << 0); // Fast PWM, Non-inverting
    TCCR0B = (1 << 1); // Prescaler 8
}

int main() {
    PWM_Init();
    while (1) {
        OCR0A = 128; // 50% brightness
    }
}


======
//bitbanging SPI Code

#define MOSI_PIN 3    // Master Out Slave In pin (PB3)
#define SCK_PIN 5     // Serial Clock pin (PB5)
#define SS_PIN 2      // Slave Select pin (PB2)

// LED Brightness value (0-255)
#define LED_BRIGHTNESS 128  // 50% brightness

void SPI_Init() {
    // Set MOSI, SCK, and SS as output pins
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2);  // PB3 (MOSI), PB5 (SCK), PB2 (SS)

    // Set SS pin high initially (deselect the slave)
    PORTB |= (1 << PB2);
}

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

// Function to set LED brightness using SPI
void setLEDBrightness(unsigned char brightness) {
    // Select the slave (set SS low)
    PORTB &= ~(1 << PB2);

    // Send the brightness value via SPI
    sendSPI(brightness);

    // Deselect the slave (set SS high)
    PORTB |= (1 << PB2);
}

int main(void) {
    SPI_Init();  // Initialize SPI pins

    while (1) {
        setLEDBrightness(LED_BRIGHTNESS);  // Set LED brightness to 50%
    }

    return 0;
}
