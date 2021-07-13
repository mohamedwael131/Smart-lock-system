#include "Keypad.h"
void setup_keypad(void){
	SYSCTL_RCGCGPIO_R |= 0xC;
	while ((SYSCTL_PRGPIO_R & 0xC) != 0xc){}
	GPIO_PORTC_DIR_R = 0xff; //C0,C1,C2 outputs
	GPIO_PORTD_DIR_R = 0 ;  //D0,D1,D2,D3 inputs
	GPIO_PORTC_DEN_R = 0xff ; //PORTC digitally enabled 
	GPIO_PORTD_DEN_R = 0xff ; 
	GPIO_PORTC_AFSEL_R = 0 ;  //Alternative function not selected
	GPIO_PORTD_AFSEL_R = 0 ;  
}
char read_key(void){ 
	char arr_pos[4][3] = {{'1','2','3'},
												{'4','5','6'},
												{'7','8','9'},
												{'*','0','#'}};
	//pooling over the keypad pins
		for (int i = 0 ; i < 3 ; i++){
			GPIO_PORTC_DATA_R = (1 << i) ; 
			for (int j = 0 ; j < 4 ; j++){
				if ((GPIO_PORTD_DATA_R & (1<<j))){
					return arr_pos[j][i]; 
				}
			}
		}
		return '\0' ; 
}
int check_keypad(void){
	//checking start of keypad operation
	GPIO_PORTC_DATA_R = 1 ; 
	if ((GPIO_PORTD_DATA_R & 0x8 &&1) == 1){
		return 1 ; 	
	}
	else{
		return 0 ; 
	}
}
