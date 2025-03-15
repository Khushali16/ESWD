
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