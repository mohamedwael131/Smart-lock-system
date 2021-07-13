#include "Keypad.h"
#include "UART.h"
#include "Solonoid.h"
#include "Time.h"
#define MAXROOMS 10
unsigned short limit;
struct room{
	unsigned short id , status ; 
	char *password ;
};
struct room room_arr[MAXROOMS] ;
void rooms_setup(){
	limit = (unsigned short)UART_recvByte(); //Recieving the number of rooms on UART0 
	while (limit == 0){
		UART_sendByte('L'); // The L returned value will signal an error to the user to enter the number of rooms.
		limit = (unsigned short) UART_recvByte();
	}
	char room_id = '\0' ; 
	for (int i = 0 ; i < limit ; i++){
		while (room_id == '\0'){
			room_id = UART_recvByte() ; // room id must be set and must be unique, checking on the uniquness should be done on the PC software side
		}
		room_arr[i].id = (unsigned short)room_id ; 
		room_arr[i].password = "0000" ; //default password for all rooms until they are set
		room_arr[i].status = 0; //0 for locked	"default value of a room"
	}
	UART_sendByte('D');//operation done succeffuly . 
}
int check_roomChange(){
	char recvd = UART_recvByte();
	if (recvd == 'S'){ //room's status is needed to change
		UART_sendByte('S'); //confirmation and resseting the DR
		return 1 ; 
	}
	else if (recvd == 'P'){ //room's password is needed to be changed
		UART_sendByte('P');
		return 2 ; 
	}
	else{
		return 0 ; 
	}	
}
void change_roomStatus(){
	//function can not be called after the check_roomChange has returned 1
	//data is sent through two bytes: first bytes to set the room number, second byte to supply the new status 
	char room_req = '\0' ; 
	while (room_req == '\0'){
		room_req = UART_recvByte() ; 
	}
	char status = '\0' ; 
	while (status == '\0'){
		status = UART_recvByte() ; 
	}
	for (int i = 0 ; i < limit ; i++){
		if (room_arr[i].id == (unsigned int)room_req){
			room_arr[i].status = (unsigned short) status;// status of room
			solo_change(room_arr[i].status) ;
			UART_sendByte('D');	//operation done succefully 		
			return;
		}
	}
	UART_sendByte('F'); // Room id request was not found 
}
void change_roomPassword(){
	//function must be called after the check_roomChange has return 2 
	char room = '\0' ; 
	while (room == '\0'){
		room = UART_recvByte() ;
	}	
	int count = 0; char temp_chr ='\0' ; 
	for (int i = 0; i < limit ; i++){
		if (room_arr[i].id == (unsigned short)room){
			while (count < 4){
				while (temp_chr == '\0'){
					temp_chr = UART_recvByte() ; 
				}
				room_arr[i].password[count] = temp_chr ; 
				temp_chr = '\0' ; 
			}
			UART_sendByte('D'); // operation done succefully
			return;
		}
	}
	UART_sendByte('F'); //Failure
}
int input_pass(){
	int count = 0 ; 
		while (count < 4){
			if (room_arr[0].password[count] != read_key()){
				return 0 ;
			}
			count += 1 ; 
			sleep__(.3); //this will allow the function to sleep for .3 of a second enough time for the user to remove his hand of one button to the next
		}
	return 1 ; 
}
int __main(){
	// it is assumed the first room in the rooms array is the one connected to the keypad room_arr[0]
	UART_setup();
	rooms_setup() ; 
	solo_setup() ;
	setup_keypad() ; 
	while (1){
		if (check_roomChange() == 1){
			change_roomStatus() ; 
		}
		else if (check_roomChange() == 2){
			change_roomPassword() ; 
		}
		if (check_keypad()){
			if (input_pass()){
				room_arr[0].status = 1; //change status
				solo_change(1) ; //open room 
			}
		}
		
	}
}
