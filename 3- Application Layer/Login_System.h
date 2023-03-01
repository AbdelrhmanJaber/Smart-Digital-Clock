
#ifndef LOGIN_SYSTEM_H_
#define LOGIN_SYSTEM_H_

void System_Reset(void);
void Get_Password(void);
u8 Login_System(void);
void Get_Pass_From_Memory(void);
void Write_Pass_To_Memory(u8 * pass);
u8 Check_Pass(u8* pass);
u8 Modify_Password(void);

#endif /* LOGIN_SYSTEM_H_ */
