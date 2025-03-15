// physical button press and light on off and  The LED toggles (ON → OFF → ON) on every button press.

void setup() {
    // Set PB4 as output (LED on Port B)
    DDRB |= (1 << PB4); 
    
    // Set PD2 as input (Button on Port D)
    DDRD &= ~(1 << PD2); 
}

void loop() {
    // Check if the button (PD2) is pressed (active low)
    if (!(PIND & (1 << PD2))) {
        PORTB &= ~(1 << PB4);
        // Turn on LED (PB4)
    } else {
         PORTB |= (1 << PB4); // Turn off LED (PB4)
    }
}



========================


// With inbuild functions

void setup()
{
  pinMode(12,OUTPUT);
  pinMode(2,INPUT);
}
void loop()
{
  if(digitalRead(2)==HIGH)
  {
    digitalWrite(12,HIGH);
  }
  else{
    digitalWrite(12,LOW);
  }
}
=======
// bitbanging SPI Code
#define MOSI_PIN 3    // Master Out Slave In pin (PB3)
#define SCK_PIN 5     // Serial Clock pin (PB5)
#define SS_PIN 2      // Slave Select pin (PB2)
#define BUTTON_PIN 2  // Button pin (PB2)
#define LED_PIN 5     // LED pin (PB5)

// Function to initialize pins
void SPI_Init() {
    // Set MOSI, SCK, and SS as output pins
    DDRB |= (1 << MOSI_PIN) | (1 << SCK_PIN) | (1 << SS_PIN);  // PB3 (MOSI), PB5 (SCK), PB2 (SS)

    // Set BUTTON_PIN as input
    DDRB &= ~(1 << BUTTON_PIN);  // Set PB2 (BUTTON) as input

    // Set SS pin high (deselect the slave)
    PORTB |= (1 << SS_PIN);
}

// Function to send a byte using bit-banging SPI
void sendSPI(unsigned char data) {
    for (unsigned char bit = 0; bit < 8; bit++) {
        // Write the bit to the MOSI pin
        if (data & (1 << (7 - bit))) {
            PORTB |= (1 << MOSI_PIN);  // Set MOSI high
        } else {
            PORTB &= ~(1 << MOSI_PIN); // Set MOSI low
        }

        // Pulse the clock (SCK)
        PORTB |= (1 << SCK_PIN);  // Set SCK high
        PORTB &= ~(1 << SCK_PIN); // Set SCK low
    }
}

// Function to toggle LED using SPI
void toggleLED() {
    PORTB &= ~(1 << SS_PIN);  // Select the slave (SS low)
    sendSPI(0xFF);  // Send a dummy byte (toggle LED data)
    PORTB |= (1 << SS_PIN);   // Deselect the slave (SS high)
}

// Debounce delay
void delay() {
    for (volatile long i = 0; i < 30000; i++);
}

int main() {
    SPI_Init();  // Initialize SPI pins

    unsigned char lastButtonState = 1; // Last button state (high)

    while (1) {
        unsigned char buttonState = (PINB & (1 << BUTTON_PIN)) >> BUTTON_PIN; // Read button state (0 if pressed)

        if (buttonState == 0 && lastButtonState == 1) { // Button press detected (falling edge)
            delay();  // Debounce delay

            // Check button state again after delay to ensure it's still pressed
            if ((PINB & (1 << BUTTON_PIN)) == 0) {
                toggleLED();  // Toggle LED state via SPI
            }
        }
        lastButtonState = buttonState;  // Update last button state
    }

    return 0;
}
