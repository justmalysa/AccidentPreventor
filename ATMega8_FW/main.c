/*
 * ATMega8_FW.c
 *
 * Created: 11.12.2020 14:40:56
 * Author : Justyna
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"
#define input_A1 PINC0
#define input_A2 PINC1
#define input_A3 PINC2
#define input_A4 PINC3
#define S1 (1<<PINC4)
#define S2 (1<<PINC5)

volatile uint8_t manual_mode;
volatile uint8_t button;
uint8_t front_sensor;
uint8_t counter = 0;

void drive_straight(void)
{
	PORTC &= ~((1 << input_A2) | (1 << input_A3));
	PORTC |= ((1 << input_A1) | (1 << input_A4));
}

void turn_right(void)
{
	PORTC &= ~((1 << input_A1) | (1 << input_A3));
	PORTC |= ((1 << input_A2) | (1 << input_A4));
}

void turn_left(void)
{
	PORTC &= ~((1 << input_A2) | (1 << input_A4));
	PORTC |= ((1 << input_A1) | (1 << input_A3));
}

void stop(void)
{
	PORTC &= ~((1 << input_A1) | (1 << input_A2) | (1 << input_A3) | (1 << input_A4));
}

void drive_back(void)
{
	PORTC &= ~((1 << input_A1) | (1 << input_A4));
	PORTC |= ((1 << input_A2) | (1 << input_A3));
}

uint16_t distance_f(uint8_t S)
{
	PORTC |= S;
	PORTB |= (1<<PB0);
	_delay_us(10);
	PORTB &= ~(1<<PB0);
	while( !(PINB & (1<<PB1)));
	TCNT1=0x00;
	while( PINB & (1<<PB1));
    uint16_t distance = TCNT1/56;
	PORTC &= ~(S);
	_delay_us(1000);
	return distance;
}

ISR(USART_RX_vect)
{
	manual_mode = 1;
	button = receiveByte();
}

int main(void)
{
	initUSART();
	UCSR0B |= ( 1 << RXCIE0);
	sei();
	
    DDRB = 0x01;
    DDRC = 0x3F;
    TCNT1 = 0x00;
    TCCR1B = (1<<CS10);
    drive_straight();
	while (1) 
    {
		if (manual_mode == 0)
		{
			if ( distance_f(S1) < 20)
			{
				do
				{
					turn_left();
				}
				while (distance_f(S1) < 30);
			}
			drive_straight();
			
			if ( distance_f(S2) < 20)
			{
				do
				{
					turn_right();
				}
				while (distance_f(S2) < 30);
			}
			drive_straight();
			
			if ( distance_f((S1|S2)) < 20)
			{
				uint16_t distance_S1 = distance_f(S1);
				uint16_t distance_S2 = distance_f(S2);
				if(distance_S1 > distance_S2)
				{
					do
					{
						turn_right();
					}
					while (distance_f((S1|S2)) < 30);
				}
				else
				{
					do
					{
						turn_left();
					}
					while (distance_f((S1|S2)) < 30);
				}
			}
			drive_straight();
		}
		else
		{
			counter++;
			front_sensor = distance_f(S1|S2);
			if(counter == 20)
			{
				transmitByte(front_sensor);
				counter = 0;
			}
			if(front_sensor < 10)
			{
				stop();
				button = '0';
			}
			if (button == 'A')
			{
				drive_straight();
			}
			else if (button == 'B')
			{
				turn_left();
			}
			else if (button == 'C')
			{
				turn_right();
			}
			else if (button == 'D')
			{
				drive_back();
			}
			else if (button == 'E')
			{
				stop();
			}
			else if (button == 'F')
			{
				manual_mode = 0;
			}
		}
    }
	return 0;
}



