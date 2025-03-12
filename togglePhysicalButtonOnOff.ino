// physical button press and light on off and  The LED toggles (ON → OFF → ON) on every button press.

#define DDRB  (*(volatile unsigned char*)0x24)
#define PORTB (*(volatile unsigned char*)0x25)
#define PINB  (*(volatile unsigned char*)0x23)

void delay() {
    for (volatile long i = 0; i < 30000; i++);
}

int main() {
    DDRB &= ~(1 << 2); // Button as input
    DDRB |= (1 << 5);  // LED as output

    while (1) {
        if (!(PINB & (1 << 2))) { // Button pressed
            delay(); // Debounce
            if (!(PINB & (1 << 2))) {
                PORTB ^= (1 << 5); // Toggle LED
            }
        }
    }
}
