#ifndef MTWI_INTERFACE_H_
#define MTWI_INTERFACE_H_

typedef enum
{
	TWI_OK = 1,
	TWI_StartCondition_ERROR,
	TWI_RepeatedStartCondioton_ERROR,
	TWI_Send_SLAVE_W_ERROR,
	TWI_Send_SLAVE_R_ERROR,
	TWI_Send_DATA_ERROR,
	TWI_NULL_POINTER,
	TWI_Recieve_DATA_ERROR
}TWI_ErrorState;


#define FREQ_100K     1

#define FREQ_200K     2

#define FREQ_300K     3

#define FREQ_400K     4

void MTWI_MasterInit(u8 Copy_u8Frequency);

void MTWI_SlaveInit(u8 Copy_u8SlaveAddress);

TWI_ErrorState MTWI_SendStartCondition(void);

TWI_ErrorState MTWI_SendRepeatedStartCondition(void);

TWI_ErrorState MTWI_SendSlaveAddressWithWrite(u8 Copy_u8SlaveAddress);

TWI_ErrorState MTWI_SendSlaveAddressWithRead(u8 Copy_u8SlaveAddress);

TWI_ErrorState MTWI_SendDataByte(u8 Copy_u8Data);

TWI_ErrorState MTWI_RecieveDataByte(u8* Copy_pu8RecievedData);

void MTWI_SendStopCondition(void);

#endif /* MTWI_INTERFACE_H_ */
