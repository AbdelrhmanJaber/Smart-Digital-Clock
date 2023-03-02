/*Library Layer*/
#include"std_types.h"

/*HAL*/
#include"HEEPROM_interface.h"
#include"HLCD_interface.h"
#include"HKPD_interface.h"
#include"Login_System.h"

#define F_CPU 8000000UL
#include<avr/delay.h>

u8 Byte_1 , Byte_2  , Byte_3 , Byte_4;

void System_Reset(void){
	    /*Start the system*/
		HEEPROM_u8WriteByte(1,1);
		HLCD_u8ClearDisplay();
		HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_2);
		HLCD_u8SendString("Welcome to",2);
		HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_2);
		HLCD_u8SendString("Smart Watch",2);
		_delay_ms(2000);
		HLCD_u8ClearDisplay();
		Get_Password();
}

void Get_Password(void){
	u8 returnedValue,counter=0;
	u8 pass[4];
	while(1){
		HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_0);
		HLCD_u8SendString("Set your pass",0);
		HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
		/*Get the password that user wants*/
	while(1){
		HKPD_u8GetKey(&returnedValue);
		if(returnedValue != HKPD_u8_NO_KEY_PRESSED){
			if(returnedValue == '=') break;
			else{
				HLCD_u8SetCursor(HLCD_u8_LINE_2 , counter);
				HLCD_u8SendChar(returnedValue);
				_delay_ms(50);
				HLCD_u8SetCursor(HLCD_u8_LINE_2 , counter);
				HLCD_u8SendChar('*');
				pass[counter++]=returnedValue;
			}
		}
	}
	if(counter != 4){
		/*user entered unavailable password */
		HLCD_u8ClearDisplay();
		counter = 0;
		HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_0);
		HLCD_u8SendString("your pass should",0);
		HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
		HLCD_u8SendString("4 chars",0);
		_delay_ms(1500);
		HLCD_u8ClearDisplay();
	}
	else{
		/* Save the password on the EEPROM <Non Volatile memory >*/
			Write_Pass_To_Memory(pass);
			_delay_ms(5);
		HLCD_u8ClearDisplay();
		HLCD_u8SendString("your pass saved",0);
		_delay_ms(1000);
		break;
	}
	}
  }

u8 Login_System(void){
	u8 pass[4];
	u8 counter = 0,returnedValue,loginFlag = 0,passFlag=1;
	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
	for(u8 i=0 ; i < 3 ; i++){
		while(1){
			HKPD_u8GetKey(&returnedValue);
			if(returnedValue!= HKPD_u8_NO_KEY_PRESSED){
				if(returnedValue == '=') break;
				else{
					HLCD_u8SetCursor(HLCD_u8_LINE_2,counter);
					HLCD_u8SendChar(returnedValue);
					_delay_ms(50);
					HLCD_u8SetCursor(HLCD_u8_LINE_2,counter);
					HLCD_u8SendChar('*');
					pass[counter++] = returnedValue;
				}
			}
		}
		if(counter != 4){
			/*User entered unavailable password*/
			counter = 0;
			HLCD_u8ClearDisplay();
			HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_0);
			HLCD_u8SendString("Wrong,try again",0);
			HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
			_delay_ms(2000);
		}
		else{
			/*User entered 4 Chars password  */
			/*Check if the password is right or wrong*/
			u8 readValue;
			/*Read the password from EEPROM*/
			 Get_Pass_From_Memory();
			 passFlag =  Check_Pass(pass);
			if(passFlag){
				/*Pass is right*/
				loginFlag = 1;
				counter = 0;
				passFlag = 0;
				HLCD_u8ClearDisplay();
				HLCD_u8SendString("Pass is correct",0);
				_delay_ms(1000);
				break;
			}
			else{
				/*pass is wrong*/
				loginFlag = 0;
				counter = 0;
				HLCD_u8ClearDisplay();
				HLCD_u8SendString("wrong,try again",0);
				HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
				_delay_ms(2000);
			}
		}
	}
	return loginFlag;
}


u8 Modify_Password(void){
	u8 flag=0 , temp ;
	HLCD_u8SetCursor(HLCD_u8_LINE_1 , HLCD_u8_Position_0);
	HLCD_u8SendString("your old pass :",0);
	temp = Login_System();
	if(temp){
		/*Correct old password*/
		flag = 1;
		HLCD_u8ClearDisplay();
		HLCD_u8SetCursor(HLCD_u8_LINE_1 ,HLCD_u8_Position_0);
		HLCD_u8SendString("your new pass:",0);
		HLCD_u8SetCursor(HLCD_u8_LINE_2 , HLCD_u8_Position_0);
		/*set new password*/
		Get_Password();
	}
	else{
		/*user entered wrong old password*/
		flag = 0;
	}
	return flag;
}


void Write_Pass_To_Memory(u8 * pass){
	HEEPROM_u8WriteByte(pass[0] , 3);
	HEEPROM_u8WriteByte(pass[1] , 25);
	HEEPROM_u8WriteByte(pass[2] , 100);
	HEEPROM_u8WriteByte(pass[3] , 125);
}

void Get_Pass_From_Memory(void){
	HEEPROM_u8ReadByte(&Byte_1 , 3);
	HEEPROM_u8ReadByte(&Byte_2 , 25);
	HEEPROM_u8ReadByte(&Byte_3 , 100);
	HEEPROM_u8ReadByte(&Byte_4 , 125);
}

u8 Check_Pass(u8* pass){
	u8 passFlag = 1;
	if(pass[0] != Byte_1) passFlag = 0;
	if(pass[1] != Byte_2) passFlag = 0;
	if(pass[2] != Byte_3) passFlag = 0;
	if(pass[3] != Byte_4) passFlag = 0;
	return passFlag;
}
