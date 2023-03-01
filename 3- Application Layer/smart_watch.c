/*********  Smart Digital Clock ********/

/*Library Layer*/
#include"std_types.h"
#include"Bit_Math.h"

/*MCAL Layer*/
#include"MDIO_interface.h"
#include"MTMR_interface.h"

/*HAL Layer*/
#include"HEEPROM_interface.h"
#include"HLCD_interface.h"
#include"HKPD_interface.h"

/*Applications Layer*/
#include"Login_System.h"

#define F_CPU 8000000UL
#include<avr/delay.h>

u8 Get_Pressed(void);
void Clock_12Hours(void);
void Start_Clock_12Hours(u8 time);
void Set_Clock(u8 timeSystem);
void Clock_24Hours(void);
void Start_Clock_24Hours(void);

/*Macro for the register that contain global interrupt*/
#define SREG_REG         *((volatile u8*)0x5f)

/* Macros for more Readable code */
#define AM                1
#define PM                2
#define SECOND            1
#define MINUTES           2
#define HOURS             3
#define HOURS_12          1
#define HOURS_24          2

/* Global variables to set the clock*/
u8 onesSeconds = 5  , tensSeconds = 5 , onesMinutes =2 , tensMinutes = 3 ;
u8 onesHours = 1 , tensHours = 1, time;
int main(void){
	/* Enable the global interrupt */
	SET_BIT(SREG_REG,7);
	/* Init Function*/
	MDIO_u8INIT();
	HLCD_voidINIT();
	HEEPROM_voidInit(FREQ_400K);

     while(1){
	  u8 loginTest = 0 , returnedValue ;
	  static u8 systemResetFlag = 0 , breakFlag = 0 , systemBreak = 0;
	  HEEPROM_u8ReadByte(&systemResetFlag,1);
	  /*Check if this is the first time or system is reseted*/
	  if(systemResetFlag != 1){
	  	System_Reset();
	  }

	  HLCD_u8ClearDisplay();
	  HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_0);
	  /*Getting the password from the user and check it*/
	  HLCD_u8SendString("Enter your pass",0);
	  loginTest = Login_System();
	  HLCD_u8ClearDisplay();
	  if(!loginTest){ //Falied Login
		  HLCD_u8SendString("no more trials",0);
		  HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
		  HLCD_u8SendString("wait 3 seconds",0);
		  _delay_ms(3000);
	  }
	  else{
		  while(1){
		  HLCD_u8ClearDisplay();
		  HLCD_u8SendString("1-Clock",0);
		  HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
		  HLCD_u8SendString("2-settings",0);
		  returnedValue = Get_Pressed();
		  if(returnedValue == '1'){
			  while(1){
			  HLCD_u8ClearDisplay();
			  HLCD_u8SendString("1- 12 Hours",0);
			  HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
			  HLCD_u8SendString("2- 24 Hours",0);
			  /*Get the user option*/
			  returnedValue = Get_Pressed();
			    if(returnedValue == '1'){
			    	/*Clock mode*/
			    	while(1){
			    	u8 test;
			    	HLCD_u8ClearDisplay();
			    	HLCD_u8SendString("1- AM   2- PM",0);
			    	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
			    	/*Get the user Option*/
			    	returnedValue = Get_Pressed();
			    	HLCD_u8ClearDisplay();
			    	HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_0);
			    	HLCD_u8SendString("hrs",1);
			    	HLCD_u8SetCursor(HLCD_u8_LINE_1 , HLCD_u8_Position_4);
			    	HLCD_u8SendString("mins",5);
			    	HLCD_u8SetCursor(HLCD_u8_LINE_1 , HLCD_u8_Position_9);
			    	HLCD_u8SendString("secs",10);
			    	/*Set Clock in 12 Hours mode*/
			    	Set_Clock(HOURS_12);
			    	HLCD_u8ClearDisplay();
			    	if(returnedValue == '1'){
			    		time = AM;
			    		Start_Clock_12Hours(time);
			    		while(1){
			    			/*Check if the user want to return*/
			    		test = Get_Pressed();
			    		if(test == 'C'){
			    			breakFlag = 1;
			    			MTMR_u8StopTimer();
			    			break;
			    		}
			    	  }
			    		if(breakFlag){
			    			breakFlag = 0;
			    			break;
			    		}
			    	}
			    	else if(returnedValue == '2') {
			    		time = PM;
			    		Start_Clock_12Hours(time);
			    		while(1){
			    			/*Check if the user want to return*/
			    		test = Get_Pressed();
			    		if(test == 'C'){
			    			breakFlag = 1;
			    			MTMR_u8StopTimer();
			    			break;
			    		}
			    	  }
			    		if(breakFlag){
			    			/*return to the main options*/
			    			breakFlag = 0;
			    			break;
			    		}
			    	}
			    }
			  }
			  else if(returnedValue == 'C') break;
			  else{
				  /*Clock in 24 Hours Mode*/
				  while(1){
				  u8 test;
				  HLCD_u8ClearDisplay();
				  HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_0);
				  HLCD_u8SendString("hrs",1);
				  HLCD_u8SetCursor(HLCD_u8_LINE_1 , HLCD_u8_Position_4);
				  HLCD_u8SendString("mins",5);
				  HLCD_u8SetCursor(HLCD_u8_LINE_1 , HLCD_u8_Position_9);
				  HLCD_u8SendString("secs",10);
				  /*Set Clock in 24 Hours Mode*/
				  Set_Clock(HOURS_24);
				  HLCD_u8ClearDisplay();
				  /*Start Timing and the clock*/
				  Start_Clock_24Hours();

				  while(1){
					  /* Check if the user wants to return*/
					  test = Get_Pressed();
					  if(test == 'C'){
					  	breakFlag = 1;
					  	MTMR_u8StopTimer();
					  	break;
					  }
				  }
				  if(breakFlag){
					  /*if the user wants to return to the main menu*/
				  	breakFlag = 0;
				  	break;
				  }
			  }
			 }
		  }
		 }
		else if(returnedValue == 'C') break;
		else if(returnedValue == '2'){
			/*Setting mode*/
			  while(1){
			  u8 flag = 0;
			  HLCD_u8ClearDisplay();
			  HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_0);
			  HLCD_u8SendString("1- Change pass",0);
			  HLCD_u8SetCursor(HLCD_u8_LINE_2 , HLCD_u8_Position_0);
			  HLCD_u8SendString("2- Reset System",0);
			  returnedValue = Get_Pressed();
			  if(returnedValue == '1'){
				  /*Changing the old password*/
				  HLCD_u8ClearDisplay();
				  flag = Modify_Password();
				  if(flag){
					  /*User Entered the old password correctly*/
					  HLCD_u8ClearDisplay();
					  systemBreak = 1;
				  }
				  else{
					  /*User Entered the old password wrong*/
					  systemBreak = 1;
					  break;
				  }
			  }
			  else if(returnedValue == '2'){
				  /*reset the system*/
				  HEEPROM_u8WriteByte(0,1);
				  systemBreak = 1;
				  break;
			  }
			  else if(returnedValue == 'C') break;
		  }
			  if(systemBreak){
				  /*reset the system and begin from the start*/
			    systemBreak = 0;
			    break;
			  }
		 }
	  }
	 }
    }
	return 0;
 }


u8 Get_Pressed(void){
	u8 returnedValue;
	while(1){
		/*Get user input */
	  HKPD_u8GetKey(&returnedValue);
	  if(returnedValue != HKPD_u8_NO_KEY_PRESSED){
		  HLCD_u8SendChar(returnedValue);
		  break;
	  }
	}
	  return returnedValue;
}

void Set_Clock(u8 timeSystem){
	/*Set Hours*/

	/*Hours shouldn't exceed 12 or 24 according to the chosen system*/
	if(timeSystem == HOURS_12){ /*For 12Hours System*/
		/*Check if the user didn't enter a wrong value */
	   do{
	   HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
	   tensHours = (Get_Pressed()-48);
	   }while(tensHours > 1);

	   if(tensHours == 0){
	    HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_1);
	    onesHours = (Get_Pressed()-48);
	   }
	   else{
		   /*Check if the user didn't enter a wrong value */
	   	do{
	   		HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_1);
	   		onesHours = (Get_Pressed()-48);
	   	}while(onesHours > 2);
	   }
	}
	else{ /*For 24Hours System*/
		/*Check if the user didn't enter a wrong value */
		  do{
		  HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
		  tensHours = (Get_Pressed()-48);
		  }while(tensHours > 2);

		  if(tensHours < 2){
		   HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_1);
		   onesHours = (Get_Pressed()-48);
		  }
		  else{
		  	do{
		  		HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_1);
		  		onesHours = (Get_Pressed()-48);
		  	}while(onesHours > 4);
		  }
	}

	/* Set Minutes */
	/*Minute should be <= 60*/
	/*Check if the user didn't enter a wrong value */

	do{
	 HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_4);
	 tensMinutes = (Get_Pressed()-48);
	}while(tensMinutes > 6);

	if(tensMinutes <6){
	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_5);
	onesMinutes = (Get_Pressed()-48);
	}
	else{
		/*Check if the user didn't enter a wrong value */
		do{
			HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_5);
			onesMinutes = (Get_Pressed()-48);
		}while(onesMinutes != 0);
	}


	/*Set Seconds*/
	/*Seconds should be <= 60*/
	/*Check if the user didn't enter a wrong value */

	do{
	 HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_9);
	 tensSeconds = (Get_Pressed()-48);
	}while(tensSeconds > 6);

	if(tensSeconds < 6){
	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_10);
	onesSeconds = (Get_Pressed()-48);
	}
	else{
		/*Check if the user didn't enter a wrong value */
		do{
			HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_10);
			onesSeconds = (Get_Pressed()-48);
		}while(onesSeconds != 0);
	}
}


void Start_Clock_12Hours(u8 time){
    HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_0);
    HLCD_u8SendString("hrs",1);
    HLCD_u8SetCursor(HLCD_u8_LINE_1 , HLCD_u8_Position_4);
    HLCD_u8SendString("mins",5);
    HLCD_u8SetCursor(HLCD_u8_LINE_1 , HLCD_u8_Position_9);
    HLCD_u8SendString("secs",10);
    HLCD_u8SetCursor(HLCD_u8_LINE_1 , HLCD_u8_Position_14);
    if(time == AM)
      HLCD_u8SendString("AM",14);
    else
    	HLCD_u8SendString("PM",14);
    /*Start Clock and enable timer*/
	MTMR_u8Timer0Init();
    MTMR_u8Timer0SetCallBack(0,&Clock_12Hours);
    MTMR_u8Timer0NormalModeTimeMapping(0,0,1,0,0);
}


void Start_Clock_24Hours(void){
	 HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_0);
	 HLCD_u8SendString("hrs",1);
	 HLCD_u8SetCursor(HLCD_u8_LINE_1 , HLCD_u8_Position_4);
	 HLCD_u8SendString("mins",5);
	 HLCD_u8SetCursor(HLCD_u8_LINE_1 , HLCD_u8_Position_9);
	 HLCD_u8SendString("secs",10);
	 HLCD_u8SetCursor(HLCD_u8_LINE_1 , HLCD_u8_Position_14);
	 MTMR_u8Timer0Init();
	 MTMR_u8Timer0SetCallBack(0,&Clock_24Hours);
	 MTMR_u8Timer0NormalModeTimeMapping(0,0,1,0,0);
}


void Clock_12Hours(void){
	static u8 flag = 0;
	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
	HLCD_u8SendNumber(tensHours);
	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_1);
	HLCD_u8SendNumber(onesHours);
	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_4);
	HLCD_u8SendNumber(tensMinutes);
	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_5);
	HLCD_u8SendNumber(onesMinutes);
	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_9);
	HLCD_u8SendNumber(tensSeconds);
	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_10);
	HLCD_u8SendNumber(onesSeconds);
	if(flag){
		/*Change the AM or PM if the hours = 12 */
		flag = 0;
		HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_14);
		if(time == AM){
			HLCD_u8SendString("PM",14);
		}
		else{
			HLCD_u8SendString("AM",14);
		}
	}
	onesSeconds++;
	/*handle the increment of seconds , minutes and hours */

	if(onesSeconds == 10){
		onesSeconds = 0;
		if(tensSeconds == 5){
			onesMinutes++;
			tensSeconds = 0;
		}
		else{
			tensSeconds++;
		}
	}
	if(onesMinutes == 10){
		onesMinutes = 0;
		if(tensMinutes == 5){
			onesHours++;
			tensMinutes = 0;
		}
		else{
			tensMinutes++;
		}
	}
	if(onesHours == 10 && tensHours == 0){
		onesHours = 0;
		tensHours++;
	}
	else if(onesHours == 2 && tensHours == 1){
		tensHours = 0;
		onesHours = 0;
		flag = 1;
	}
}

void Clock_24Hours(void){
	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
	HLCD_u8SendNumber(tensHours);
	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_1);
	HLCD_u8SendNumber(onesHours);
	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_4);
	HLCD_u8SendNumber(tensMinutes);
	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_5);
	HLCD_u8SendNumber(onesMinutes);
	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_9);
	HLCD_u8SendNumber(tensSeconds);
	HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_10);
	HLCD_u8SendNumber(onesSeconds);
	onesSeconds++;
	if(onesSeconds == 10){
		onesSeconds = 0;
		if(tensSeconds == 5){
			onesMinutes++;
			tensSeconds = 0;
		}
		else{
			tensSeconds++;
		}
	}
	if(onesMinutes == 10){
		onesMinutes = 0;
		if(tensMinutes == 5){
			onesHours++;
			tensMinutes = 0;
		}
		else{
			tensMinutes++;
		}
	}
	if(onesHours == 10 && tensHours < 2){
		onesHours = 0;
		tensHours++;
	}
	else if(onesHours == 4 && tensHours == 2){
		tensHours = 0;
		onesHours = 0;
	}
}

