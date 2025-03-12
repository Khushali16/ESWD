// Light on off frequently by 5 sec delay

void loop_delay()
{
	for (volatile long i = 0; i<500000;i++); //loop delay for 5 sec
}
int main()
{
	DDRB |= 1<<PB5; //set pin 13 as output DDRB Means Data Direction Register B, PB5 is the pin number

	while(1)
	{
		PORTB ^= (1<<PB5); //toggle the LED state , PORTB is the output register, PB5 is the pin number
		loop_delay();
	}
}