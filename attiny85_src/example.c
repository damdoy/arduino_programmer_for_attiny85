#include <avr/io.h>
#include <util/delay.h>

void main(void)
{
    //set pin PB3 as output (either source or sink)
    DDRB = (1<<DDB3);

    while (1)
    {
        //turn led off
        PORTB = (1<<PB3);
        _delay_ms(500);

        //turn led on (sink)
        PORTB = (0<<PB3);
        _delay_ms(500);
    }
}
