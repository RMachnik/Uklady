#include "HD44780.h"
#define E PA5
#define RS PA4


void WriteNibble(unsigned char nibbleToWrite)
{
	PORTA |=(1<<E);
	PORTA &=0xF0;
	PORTA |=(nibbleToWrite & 0xFF);
	PORTA&=~(1<<E);	
}


void WriteByte(unsigned char dataToWrite)
{
	WriteNibble(dataToWrite>>4);
	WriteNibble(dataToWrite);
	_delay_us(50);
}


void LCD_Command(unsigned char data)
{
	PORTA&=~(1<<RS);
	WriteByte(data);
};

void LCD_Text(char *tab)
{
	PORTA|=(1<<RS);
	while(*tab){
		WriteByte(*tab++);
	}
};
void LCD_GoToXY(unsigned char first, unsigned char second)
{
};

void LCD_Clear(void)
{
};

void LCD_Home(void)
{
};

void send_4_zatrzask(uint8_t val)
{
	PORTB |= 1 << PINB4;

	PORTB = (PORTB & 0xF0) | (val&0x0F);
	PORTB &= ~(1 << PINB4);

}


void LCD_Initalize(void)
{
	
		_delay_ms(45);

		PORTB &= ~(1 << E);

		send_4_zatrzask(3);

		_delay_ms(5);

		send_4_zatrzask(3);

		_delay_ms(5);

		send_4_zatrzask(3);

		_delay_ms(5);

		send_4_zatrzask(2);

		send_4_zatrzask(2);
		send_4_zatrzask(8);
		_delay_ms(2);
		send_4_zatrzask(0);
		send_4_zatrzask(8);
		_delay_ms(2);
		send_4_zatrzask(0);
		send_4_zatrzask(1);
		_delay_ms(2);
		send_4_zatrzask(0);
		send_4_zatrzask(6);
		_delay_ms(2);
		WriteByte(0x0F);


};