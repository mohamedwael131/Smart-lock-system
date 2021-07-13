#include "tm4c123gh6pm.h"
#include "Solonoid.h"
void solo_setup(void){
	SYSCTL_RCGCGPIO_R |= 0x20 ; 
	while ((SYSCTL_PRGPIO_R & 0x20) == 0){}
	GPIO_PORTF_DEN_R = 0x8 ; 
	GPIO_PORTF_DIR_R =0x8 ;
	GPIO_PORTF_DATA_R = 0; 
}
void solo_change(int status){
	if (status){
		GPIO_PORTF_DATA_R = 0x8 ; //opening the lock (enabling the green led)
	}
	else{
		GPIO_PORTF_DATA_R = 0 ; 
	}
}
