#include "tm4c123gh6pm.h"
void sleep_tick(){
	NVIC_ST_CTRL_R = 0x0 ; 
	NVIC_ST_CURRENT_R = 0x0 ;
	NVIC_ST_RELOAD_R = 0x27100 ; // tick time will be 1/100 of a second
	NVIC_ST_CTRL_R = 0x5 ; 
	while ((NVIC_ST_CTRL_R &(0x10000)) == 0){}
}
void sleep__(double seconds){
	unsigned int num = seconds*100 ; 
	for (int i = 0 ; i < num ; i++){
		sleep_tick() ; 
	}
}
