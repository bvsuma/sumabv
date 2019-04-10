
#include <avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
uint16_t ReadADC(uint8_t ch);

volatile int Flag1 = 0, Flag2 = 0, Flag3 = 0;
int main(void)
{

    DDRD &= ~(1<<2);
    PORTD |= (1<<2);

    DDRB |= (1<<PB1);
    EICRA |= (1<<ISC00);
    EIMSK |= (1<<INT0);

    DDRD &= ~(1<<3);
    PORTD |= (1<<3);



     DDRB |= (1<<PB1);
    EICRA |= (1<<ISC10);
    EIMSK |= (1<<INT1);

    PCICR |= (1<<PCIE0);
    PCMSK0 |= (1<< PCINT0);

    sei();


    while(1)
    {
        if (Flag1 == 1)
        {
            PORTB |= (1<<PB1);
            if(Flag3 == 1)
            {
                Check_rpm();
            }
            Flag1 = 0;
        }


        if (Flag2 == 1)
        {
            PORTB &= ~(1<<PB1);
            Flag2 = 0;

        }

    }

    return 0;
}
ISR(INT0_vect)
{
    Flag1 = 1;
}
ISR(INT1_vect)
{
    Flag2 = 1;
}
ISR(PCINT0_vect)
{
    Flag3 = 1;
}

void InitADC()
    {
        ADMUX = (1<<REFS0);
        ADCSRA = (1<<ADEN) | (7<<ADPS0);
    }
uint16_t ReadADC(uint8_t ch)
    {
        ADMUX &= 0XF8;
        ch = ch & 0b00000111;
        ADMUX |= ch;
        ADCSRA |= (1<<ADSC);
        while(!(ADCSRA & (1<<ADIF)));
        ADCSRA |= (1<<ADIF);
        return ADC;
    }

void Check_rpm(void)
{
     TCCR1A|=((1<<COM1A1)|(1<< WGM10)|(1<<WGM11));
    TCCR1B|=((1<<WGM12)|(1<<CS11)|(1<<CS10));
    TCNT1=0x00;
    InitADC();
    uint16_t x = 0;


    DDRB|=(1<<PB1);

    while(1)
    {
      x = ReadADC(0);

          OCR1A= x;
          _delay_ms(500);

    }
}
