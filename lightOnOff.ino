// switch light on and off with time delay of 5 seconds


#define LED_PIN 13  // Arduino Uno's onboard LED

void setup() {
    DDRB |= (1 << PB5); // Set pin 13 as output
}

void loop() {
    // Turn LED ON
    PORTB |= (1 << PB5);  
    delayLoop(); // Custom delay
    
    // Turn LED OFF
    PORTB &= ~(1 << PB5);
    delayLoop(); // Custom delay
}

// Custom software delay (replaces delay() function)
void delayLoop() {
    for (volatile long i = 0; i < 500000; i++); // Adjust for Wokwi speed
}