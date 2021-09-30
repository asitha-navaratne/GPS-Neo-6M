#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void USART_INIT(void);
unsigned char USART_RECEIVE(void);
void USART_SEND(unsigned char character);
void USART_SEND_STRING(char *string);

uint8_t UBRR = 51;							///< Value to be changed to specify the baudrate for the particular operating clock frequency.

volatile unsigned char arrayIndex = 0;					///< Value to act as the index of the buffer array.
volatile uint8_t stringReceived = 0;

unsigned char gpgga[6] = {'$','G','P','G','G','A'};

unsigned char buffer[100];						///< Holds the entire GPGGA string received from the GPS.
unsigned char latitude[12],longitude[12];

/*!
 *	@brief ISR for USART RX Complete Interrupt; extracts the GPGGA string from the data received from the GPS module.
 */

ISR(USART_RXC_vect){
	unsigned char dataReceived = UDR;
	buffer[arrayIndex] = dataReceived;
	arrayIndex++;
	
	if(arrayIndex < 7){
		if(buffer[arrayIndex-1] != gpgga[arrayIndex-1]){
			arrayIndex = 0;
		}
	}
	if(arrayIndex >= 50){
		stringReceived = 1;
	}
}

int main(void){
	USART_INIT();
	_delay_ms(1000);
	
	sei();								///< Enable global interrupts.
	
	while(1){
		if(stringReceived){
			
			cli();						///< Disable global interrupts.
			
			USART_SEND_STRING("Received String");
			for(uint8_t i=0;i<50;i++){
				USART_SEND(buffer[i]);
			}
			
			USART_SEND(0x0D);				///< Transmit a Carriage Return character.
			
			USART_SEND_STRING("Latitude");
			for(uint8_t i=15;i<27;i++){
				latitude[i-15] = buffer[i];
				USART_SEND(latitude[i-15]);
			}
			
			USART_SEND(0x0D);
			
			USART_SEND_STRING("Longitude");
			for(uint8_t i=29;i<41;i++){
				longitude[i-29] = buffer[i];
				USART_SEND(longitude[i-29]);
			}
			
			USART_SEND(0x0D);
			
			arrayIndex = 0;
			stringReceived = 0;
			
			sei();
		}
	}
}

/*!
 *	@brief Initialize USART.
 */

void USART_INIT(void){
	UCSRB |= (1<<TXEN)|(1<<RXEN)|(1<<RXCIE);			///< Enable transmission and receiving over USART and RX Complete Interrupt.
	UCSRC |= (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);			///< Select register UCSRC and set transmission character size to 8 bits.
	UBRRL = UBRR;							///< Set UBRR value for specified baudrate at specified frequency.
}

/*!
 *	@brief Receive a character over USART.
 *	@return Character received (unsigned char).
 */

unsigned char USART_RECEIVE(void){
	while(!(UCSRA & (1<<RXC)));					///< Wait until data receiving is complete.
	return(UDR);							///< Return contents of UDR register.
}

/*!
 *	@brief Transmit a character over USART.
 *	@param Character to be transmitted (unsigned char).
 */

void USART_SEND(unsigned char character){
	while(!(UCSRA & (1<<UDRE)));					///< Wait until data register is empty.
	UDR = character;						///< Load character to be transmitted to data register.
	while(!(UCSRA & (1<<TXC)));					///< Wait until transmission is complete.
}

/*!
 *	@brief Transmit a string of characters over USART.
 *	@param String to be transmitted (char).
 */

void USART_SEND_STRING(char *string){
	for(uint8_t i=0;string[i]!=0;i++){
		USART_SEND(string[i]);
	}
	USART_SEND(0x32);						///< Transmit a space character.
}
