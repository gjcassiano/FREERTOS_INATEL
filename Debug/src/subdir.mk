################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/acc.c \
../src/eeprom.c \
../src/flash.c \
../src/font5x7.c \
../src/joystick.c \
../src/led7seg.c \
../src/light.c \
../src/lpc17xx_adc.c \
../src/lpc17xx_can.c \
../src/lpc17xx_clkpwr.c \
../src/lpc17xx_dac.c \
../src/lpc17xx_emac.c \
../src/lpc17xx_gpio.c \
../src/lpc17xx_i2c.c \
../src/lpc17xx_i2s.c \
../src/lpc17xx_libcfg_default.c \
../src/lpc17xx_mcpwm.c \
../src/lpc17xx_nvic.c \
../src/lpc17xx_pinsel.c \
../src/lpc17xx_pwm.c \
../src/lpc17xx_qei.c \
../src/lpc17xx_rit.c \
../src/lpc17xx_rtc.c \
../src/lpc17xx_spi.c \
../src/lpc17xx_ssp.c \
../src/lpc17xx_timer.c \
../src/lpc17xx_uart.c \
../src/lpc17xx_wdt.c \
../src/oled.c \
../src/pca9532.c \
../src/rgb.c \
../src/rotary.c \
../src/temp.c \
../src/uart2.c 

OBJS += \
./src/acc.o \
./src/eeprom.o \
./src/flash.o \
./src/font5x7.o \
./src/joystick.o \
./src/led7seg.o \
./src/light.o \
./src/lpc17xx_adc.o \
./src/lpc17xx_can.o \
./src/lpc17xx_clkpwr.o \
./src/lpc17xx_dac.o \
./src/lpc17xx_emac.o \
./src/lpc17xx_gpio.o \
./src/lpc17xx_i2c.o \
./src/lpc17xx_i2s.o \
./src/lpc17xx_libcfg_default.o \
./src/lpc17xx_mcpwm.o \
./src/lpc17xx_nvic.o \
./src/lpc17xx_pinsel.o \
./src/lpc17xx_pwm.o \
./src/lpc17xx_qei.o \
./src/lpc17xx_rit.o \
./src/lpc17xx_rtc.o \
./src/lpc17xx_spi.o \
./src/lpc17xx_ssp.o \
./src/lpc17xx_timer.o \
./src/lpc17xx_uart.o \
./src/lpc17xx_wdt.o \
./src/oled.o \
./src/pca9532.o \
./src/rgb.o \
./src/rotary.o \
./src/temp.o \
./src/uart2.o 

C_DEPS += \
./src/acc.d \
./src/eeprom.d \
./src/flash.d \
./src/font5x7.d \
./src/joystick.d \
./src/led7seg.d \
./src/light.d \
./src/lpc17xx_adc.d \
./src/lpc17xx_can.d \
./src/lpc17xx_clkpwr.d \
./src/lpc17xx_dac.d \
./src/lpc17xx_emac.d \
./src/lpc17xx_gpio.d \
./src/lpc17xx_i2c.d \
./src/lpc17xx_i2s.d \
./src/lpc17xx_libcfg_default.d \
./src/lpc17xx_mcpwm.d \
./src/lpc17xx_nvic.d \
./src/lpc17xx_pinsel.d \
./src/lpc17xx_pwm.d \
./src/lpc17xx_qei.d \
./src/lpc17xx_rit.d \
./src/lpc17xx_rtc.d \
./src/lpc17xx_spi.d \
./src/lpc17xx_ssp.d \
./src/lpc17xx_timer.d \
./src/lpc17xx_uart.d \
./src/lpc17xx_wdt.d \
./src/oled.d \
./src/pca9532.d \
./src/rgb.d \
./src/rotary.d \
./src/temp.d \
./src/uart2.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__CODE_RED -D__REDLIB__ -I"C:\Users\Giovani\Documents\LPCXpresso_6.1.4_194\workspace\CMSISv1p30_LPC17xx\inc" -I"C:\Users\Giovani\Documents\LPCXpresso_6.1.4_194\workspace\Lib_EaBaseBoard\Debug" -I"C:\Users\Giovani\Documents\LPCXpresso_6.1.4_194\workspace\Lib_EaBaseBoard\inc" -I"C:\Users\Giovani\Documents\LPCXpresso_6.1.4_194\workspace\Lib_MCU\Debug" -I"C:\Users\Giovani\Documents\LPCXpresso_6.1.4_194\workspace\Lib_MCU\inc" -I"C:\Users\Giovani\Documents\LPCXpresso_6.1.4_194\workspace\FreeRTOS_Library\demo_code" -I"C:\Users\Giovani\Documents\LPCXpresso_6.1.4_194\workspace\FreeRTOS_Library\include" -I"C:\Users\Giovani\Documents\LPCXpresso_6.1.4_194\workspace\FreeRTOS_Library\portable" -O0 -g3 -fsigned-char -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


