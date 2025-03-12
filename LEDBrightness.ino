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