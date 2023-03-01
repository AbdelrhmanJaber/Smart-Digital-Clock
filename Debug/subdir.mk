################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HEEPROM_programm.c \
../HKPD_programm.c \
../HLCD_programm.c \
../Login_System.c \
../MDIO_programm.c \
../MTMR_programm.c \
../MTWI_programm.c \
../smart_watch.c 

OBJS += \
./HEEPROM_programm.o \
./HKPD_programm.o \
./HLCD_programm.o \
./Login_System.o \
./MDIO_programm.o \
./MTMR_programm.o \
./MTWI_programm.o \
./smart_watch.o 

C_DEPS += \
./HEEPROM_programm.d \
./HKPD_programm.d \
./HLCD_programm.d \
./Login_System.d \
./MDIO_programm.d \
./MTMR_programm.d \
./MTWI_programm.d \
./smart_watch.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


