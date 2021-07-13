#include "tm4c123gh6pm.h"
#include "UART.h"
void UART_setup(void){
	UART0_CTL_R &= ~(0x1) ; //disable UART0
	SYSCTL_RCGCGPIO_R |= 0x1; //enable PORT A;
	while ((SYSCTL_PRGPIO_R & 0x1) == 0){}
	SYSCTL_RCGCUART_R |= 0x1; //enabling UART0
	GPIO_PORTA_PCTL_R = 0x3;
	GPIO_PORTA_DEN_R = 0x3 ; 
	GPIO_PORTA_AFSEL_R = 0x3 ; 	
	GPIO_PORTA_DIR_R = 0x2 ; //PA0 rx, PA1 tx
	UART0_CTL_R |= 0x301 ; //enable UART0 withenabling the Tx,Rx UART0::start of transaction
	UART0_LCRH_R &= 0x70 ; //ENABLE FIFO , 8 bits send recieve
	//setting Baud rate as 9200 b/s, clk = 16 Mega hertz
	UART0_IBRD_R = 104 ;
	UART0_FBRD_R = 0x0A ; 
 }
char UART_recvByte(){
		if ((UART0_FR_R & UART_FR_RXFE) == 0){ //if the fifo is no empty then data exists and it must be read
			return (char)(UART0_DR_R&0xFF) ; //return only the first 8 bits 
		}
	return '\0';
}
void UART_sendByte(char Byte){
		if ((UART0_FR_R & 0x20) == 0){ //if fifo is empty, push data onto fifo
			UART0_DR_R = Byte ; 
		}
	UART0_DR_R = '\0' ; //resetting the DR
	return;
}
